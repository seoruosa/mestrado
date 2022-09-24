from models.model_MMURP import MMURPmodel
import sys
import os

from util_logging import write_to_csv

from time import perf_counter_ns
from time import localtime, strftime

def sim_time(v=1000):
    a = 0
    for i in range(10000):
        for i in range(v):
            a = a + 1

if len(sys.argv) > 1:
    
    filepath = sys.argv[1]
    filename = os.path.basename(filepath)
    instance_name = os.path.splitext(filename)[0]

    exp_log_filename = f"exp_{strftime('%Y%m%d', localtime())}.log"

    threads = 1
    
    pareto_steps = 30
    run_per_step = 1
    max_sec_per_run = 120
    

    with open(exp_log_filename, mode='a') as out:
        log = lambda message : print(message, file=out)

        log(f"Starting on instance {instance_name}")
        
        time = strftime("%Y-%m-%d %H:%M:%S %z", localtime())
        log(f"{time}")

        log(f"threads: {threads}")
        log(f"pareto_steps: {pareto_steps}")
        log(f"run_per_step: {run_per_step}")
        log(f"max_sec_per_run: {max_sec_per_run}")

        tic = perf_counter_ns()
        
        sim_time(10)
        # a = MMURPmodel(filepath=filepath, threads=threads)
        toc = perf_counter_ns()
        log(f"build_model: {(toc - tic)/1e9} secs")

        tic = perf_counter_ns()
        sim_time()
        # list_map_obj, solutions_x = a.pareto_front(pareto_steps, max_sec_per_run, run_per_step)
        
        toc = perf_counter_ns()
        log(f"build_pareto: {(toc - tic)/1e9} secs")
        
        log('*'*80)

        
        # write_to_csv(f"{instance_name}_sol", list_map_obj)
        # write_to_csv(f"{instance_name}_x", solutions_x)