from dataclasses import dataclass
from typing import Dict, List

@dataclass
class MDOVRP:
    clients: List[int]
    depots: List[int]
    all_locations: List[int]

    capacity: float
    user_demand: List[float]
    distance_dict: Dict

    def __post_init__(self):
        assert len(self.clients) == len(self.user_demand)
        assert len(self.all_locations) == (len(self.clients) + len(self.depots))

@dataclass
class MMURP(MDOVRP):
    vehicles_per_depot: List[int]
    max_dist_locations: float

    def __post_init__(self):
        assert len(self.clients) == len(self.user_demand)
        assert len(self.all_locations) == (len(self.clients) + len(self.depots))
        #TODO check this https://github.com/samuelcolvin/pydantic/issues/739, try to dont use previously 2 lines

        assert len(self.depots) == len(self.vehicles_per_depot)

if __name__ == '__main__':
    a = MMURP([0, 1,2], [3], [0, 1,2,3],10, [1.2,2,3], {(1,2):1.2, (1,3):1.5}, [7], 180)
    print(a)
    print(a.clients)    