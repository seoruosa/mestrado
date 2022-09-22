from mip import Model, Var, GUROBI, BINARY, \
    CONTINUOUS, xsum, minimize, OptimizationStatus
from typing import List, Set, Tuple

from read_instance import MMURP
from numpy import argmin, linspace


class MMURPmodel:
    # This class implements a MO model as a monoobjective model with the objective 
    # function as a convex combination of the MO obj functions

    def __init__(self, filepath, solver_name = GUROBI, threads = 1) -> None:
        self.__N, self.__D, self.__V, self.__Q, self.__max_dist_nodes, \
            self.__q, self.__c, self.__vehicles_depot, _, _ = MMURP(filepath)
        # talvez não seja necessário todas esses parametros na classe
        
        self.model = Model('MMURP', solver_name = solver_name)
        self.__start_decision_vars(self.__V, self.__N)
        self.__start_constraints()
        self.update_obj(1)

        self.model.threads = threads


    
    def __start_decision_vars(self, V, N) -> None:
        # Variaveis de decisao
        self.x = [[self.model.add_var(var_type=BINARY) if i!=j else self.model.add_var(lb=0, ub=0) for i in V] for j in V]
        self.u = [[self.model.add_var(var_type=CONTINUOUS) if i!=j else self.model.add_var(lb=0, ub=0) for i in V] for j in V]
        self.y = [self.model.add_var(var_type=BINARY) for i in N]
        self.w = [self.model.add_var(var_type=BINARY) for i in N]
    
    def update_obj(self, alfa:float) -> None:
        A = alfa
        B = 1 - alfa
        
        norm_dist = sum([self.__c[i,j] for i in self.__V for j in self.__V if j>i])
        norm_dem = sum(self.__q)
        
        
        new_objective = minimize(A/norm_dist*xsum(self.x[i][j] * self.__c[i,j] \
            for i in self.__V for j in self.__V if i != j) -
                        B/norm_dem*xsum(self.__q[j]*self.w[j] for j in self.__N))

        self.model.objective = new_objective

    def __start_constraints(self) -> None:
        # Restricao 3
        for j in self.__N:
            self.model += xsum(self.x[i][j] for i in self.__V if i != j) == self.w[j]
        
        # Restricao 4 - quantidade veiculos por deposito

        for pos, k in enumerate(self.__D):
            self.model += xsum(self.x[k][i] for i in self.__N) <= self.__vehicles_depot[pos]

        # Restricao 5 -> na demanda relacionado ao limite de caminhões

        self.model += xsum(self.__q[j]*self.w[j] for j in self.__N if self.__q[j]>0) <= sum(self.__vehicles_depot)*self.__Q

        # Restricao 6 -> de distancia maxima entre duas cidades

        for i in self.__V:
            for j in self.__V:
                if i != j:
                    self.model += self.__c[i,j]*self.x[i][j] <= self.__max_dist_nodes
        
        # Restricao 7

        for j in self.__N:
            self.model += (xsum(self.x[i][j] for i in self.__V if i != j) - \
                xsum(self.x[j][i] for i in self.__N if i != j)) >= 0

        # Restricao 8
        for i in self.__V:
            for j in self.__V:
                if i != j:
                    self.model += self.x[i][j] + self.x[j][i] <= 1

        # Restricao 9
        # Ninguem volta para o depósito
        self.model += xsum(self.x[j][k] for k in self.__D for j in self.__V) == 0

        # Restricao 10 -> se cidade não é atendida, restrição é esquecida
        for j in self.__N:
            self.model += (xsum(self.u[i][j] for i in self.__V if i != j) - \
                xsum(self.u[j][i] for i in self.__V if i != j) - self.__q[j]) >= \
                    -self.__Q*(1 - self.w[j])

        #Restricao 11
        for i in self.__N:
            for j in self.__N:
                self.model += (self.__Q - self.__q[i])*self.x[i][j] >= self.u[i][j]
        
        # Restricao 12
        for k in self.__D:
            for j in self.__N:
                self.model += self.__Q * self.x[k][j] >= self.u[k][j]

        # # Restricao 13
        # menor distancia de algum deposito ate a cidade
        d = [min([self.__c[j, i] for j in self.__D]) for i in self.__N]
        # menor distancia entre cidade i e quaisquer outra cidade
        r = [min([self.__c[j, i] for j in self.__N if i != j]) for i in self.__N]
        # máxima menor distancia entre duas cidades
        M = max(r)

        # d[i] >= r[i] <=> é mais longe ir de algum depósito do q de outra cidade => y in {0, 1}
        # d[i] < r[i] <=> é mais perto sair de algum depósito do q de qualquer outra cidade => y = 1
        for i in self.__N:
            self.model += d[i] + M * self.y[i] >= r[i]*self.w[i]

        # Restricao 14

        for i in self.__N:
            k_l = self.__D[argmin([self.__c[j, i] for j in self.__D], axis=0)]
            self.model += self.x[k_l][i] >= self.y[i]
    
    # def bool_var_value(self, x : Var, k : int) -> int:
    #     return (1 if x.xi(k) > 0.98 else 0)

    # return the travelled distance of the k-esime found solution on actual model
    def dist(self, k:int) -> float:
        return sum([(1 if self.x[i][j].xi(k)>0.98 else 0) * self.__c[i,j] \
            for i in self.__V for j in self.__V if i != j])

    def demand(self, k:int) -> float:    
        return sum((self.__q[j] if self.w[j].xi(k)>0.98 else 0) for j in self.__N)
    
    def pareto_front(self, steps=30, max_seconds=60, runs_by_step=5) -> Tuple[Set, List]:
        solutions_obj_set = set()
        solutions_x = []
        solutions_obj = []
        solutions_gap = []

        x_to_integer_map = lambda k: {(i, j):(1 if self.x[i][j].xi(k)>0.98 else 0) \
            for i in self.__V for j in self.__V if i != j}

        for alfa in linspace(0, 1, steps, endpoint=True):
            self.update_obj(alfa)
            
            num_solutions = 0
            
            for i in range(runs_by_step):
                status = self.model.optimize(max_seconds=max_seconds)
            
                if status in (OptimizationStatus.INFEASIBLE, OptimizationStatus.ERROR):
                    break
                    
                if self.model.num_solutions > num_solutions:
                    num_solutions = self.model.num_solutions
                    
                    for k in range(num_solutions):
                        obj_calc = (self.dist(k), self.demand(k))
                        
                        if obj_calc not in solutions_obj_set:
                            solutions_obj_set.add(obj_calc)
                            x_new_sol = x_to_integer_map(k)
                            
                            solutions_x.append(x_new_sol)
                            solutions_obj.append(obj_calc)
                            solutions_gap.append(self.model.gap)
                            # print(x_new_sol)
                        
                            
                if status == OptimizationStatus.OPTIMAL:
                    break
                
                print(f"number of solutions: {len(solutions_obj_set)}")

        list_map_obj = [{'dist':el[0], 'demand':el[1], 'gap':solutions_gap[k]} for k, el in enumerate(solutions_obj)]


        return (list_map_obj, solutions_x)