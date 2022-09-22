from models.model_MMURP import MMURPmodel
import sys
import os

from util_logging import write_to_csv


if len(sys.argv) > 1:
    filepath = sys.argv[1]

    a = MMURPmodel(filepath=filepath, threads=4)
    
    list_map_obj, solutions_x = a.pareto_front(30, 30, 2)

    filename = os.path.basename(filepath)
    instance_name = os.path.splitext(filename)[0]
    
    write_to_csv(f"{instance_name}_sol", list_map_obj)
    write_to_csv(f"{instance_name}_x", solutions_x)