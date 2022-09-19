from mip import GUROBI

from read_instance import MDOVRP
from models.model_MDOVRP_MO import model_MDOVRP_MO

filepath = "../Instancias/ES-n78-m2-Q10138.txt"
# filepath = "Instancias/Vrp-Set-A/A-n80-m2-Q60.vrp"

N, D, V, Q, q, c, coord_x, coord_y = MDOVRP(filepath)


num_vehicles = 10
max_distance = 180

distance_cost = 1
demand_cost = 10

model, x, u, y, w = model_MDOVRP_MO(N, D, V, Q, q, c, max_distance, num_vehicles, 'v0', GUROBI, distance_cost, demand_cost)

# Resolve o modelo

model.optimize(max_seconds=30)
print("status: {} objective value : {} best possible: {}".format(model.status, model.objective_value, model.objective_bound))