from mip import Model, BINARY, CONTINUOUS, minimize, xsum
import numpy as np

def model_MDOVRP_MO(N, D, V, Q, q, c, max_distance, num_vehicles, name, solver_name, distance_cost, demand_cost):
    # CONSTANTES
    # limite de carros total
    NUM_VEICULOS = num_vehicles

    # Funcao objetivo
    custo_distancia = distance_cost
    custo_demanda = demand_cost

    #cria o modelo
    model = Model(name, solver_name = solver_name)

    # Variaveis de decisao
    x = [[model.add_var(var_type=BINARY) if i!=j else model.add_var(lb=0, ub=0) for i in V] for j in V]
    u = [[model.add_var(var_type=CONTINUOUS) if i!=j else model.add_var(lb=0, ub=0) for i in V] for j in V]
    y = [model.add_var(var_type=BINARY) for i in N]
    w = [model.add_var(var_type=BINARY) for i in N]

    # -------------------------------------------------------
    model.objective = minimize(custo_distancia*xsum(x[i][j] * c[i,j] for i in V for j in V if i != j) - custo_demanda*xsum(q[j]*w[j] for j in N))

    # Restricao 2
    for j in N:
        model += xsum(x[i][j] for i in V if i != j) == w[j]

    # Restricao 18

    for j in N:
        model += (xsum(x[i][j] for i in V if i != j) - xsum(x[j][i] for i in N if i != j)) >= 0

    # Restricao 19
    for i in V:
        for j in V:
            if i != j:
                model += x[i][j] + x[j][i] <= 1

    # Restricao 20
    # Ninguem volta para o depósito
    model += xsum(x[j][k] for k in D for j in V) == 0

    # Restricao 21 modificada -> se cidade não é atendida, restrição é esquecida
    for j in N:
        model += (xsum(u[i][j] for i in V if i != j) - xsum(u[j][i] for i in V if i != j) - q[j]) >= -Q*(1 - w[j])

    # restrição que liga x e w
    for j in N:
        xsum(x[i][j] for i in V if i != j) == w[j]

    #Restricao 22
    for i in N:
        for j in N:
            model += (Q - q[i])*x[i][j] >= u[i][j]

    # Restricao 23
    for k in D:
        for j in N:
            model += Q * x[k][j] >= u[k][j]


    # # Restricao 8
    # menor distancia de algum deposito ate a cidade
    d = [min([c[j, i] for j in D]) for i in N]
    # menor distancia entre cidade i e quaisquer outra cidade
    r = [min([c[j, i] for j in N if i != j]) for i in N]
    # máxima menor distancia entre duas cidades
    M = max(r)

    # d[i] >= r[i] <=> é mais longe ir de algum depósito do q de outra cidade => y in {0, 1}
    # d[i] < r[i] <=> é mais perto sair de algum depósito do q de qualquer outra cidade => y = 1
    for i in N:
        model += d[i] + M * y[i] >= r[i]*w[i]

    # Restricao 9

    for i in N:
        k_l = D[np.argmin([c[j, i] for j in D], axis=0)]
        model += x[k_l][i] >= y[i]

    model += xsum(x[k][i] for k in D for i in N) <= NUM_VEICULOS

    # Restricao na demanda relacionado ao limite de caminhões
    model += xsum(q[j]*w[j] for j in N) <= NUM_VEICULOS*Q

    # Restricao de distancia maxima entre duas cidades

    for i in V:
        for j in V:
            if i != j:
                model += c[i,j]*x[i][j] <= max_distance
    return (model, x, u, y, w)