from models.model_MMURP import MMURPmodel
import sys
import os

from util_logging import write_to_csv

from time import perf_counter_ns
from time import localtime, strftime

import argparse
from util_argparse import dir_path

from mip import INT_MAX

def sim_time(v=1000):
    a = 0
    for i in range(10000):
        for i in range(v):
            a = a + 1

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Given a MMURP instance, run the pareto to each one.")
    
    parser.add_argument("instanceFile", type=argparse.FileType('r'))
    parser.add_argument("-l", "--logDir", type=dir_path, required=False, default=".", help="Path to a dir to save the log file.")
    parser.add_argument("--threads", type=int, required=False, default=1, help="Number of threads that the solver could use.")
    parser.add_argument("-n", "--steps", type=int, required=True, help="Number of differents single objetive function that will be run.")
    parser.add_argument("--run_per_step", type=int, required=False, default=1, help="Number of times that a single OF will be run.")
    parser.add_argument("-t", "--time", type=int, required=True, help="Time limit that each single objective model could run.")
    parser.add_argument("--max_nodes", type=int, required=False, default=INT_MAX, help="Maximum number of nodes on model optimization.")
    
    args = parser.parse_args()

    log_folderpath = args.logDir    
    threads = args.threads
    pareto_steps = args.steps
    run_per_step = args.run_per_step
    max_sec_per_run = args.time
    max_nodes = args.max_nodes

    with args.instanceFile as filepath:
        
        filename = os.path.basename(filepath.name)
        instance_name = os.path.splitext(filename)[0]

        exp_log_filename = f"{log_folderpath}/exp_{strftime('%Y%m%d', localtime())}.log"

        with open(exp_log_filename, mode='a') as out:
            log = lambda message : print(message, file=out)

            log(f"Starting on instance {instance_name}")
            
            time = strftime("%Y-%m-%d %H:%M:%S %z", localtime())
            log(f"{time}")

            log(f"threads: {threads}")
            log(f"pareto_steps: {pareto_steps}")
            log(f"run_per_step: {run_per_step}")
            log(f"max_sec_per_run: {max_sec_per_run}")
            log(f"max_nodes: {max_nodes}")

            tic = perf_counter_ns()
            
            # sim_time(10)
            a = MMURPmodel(filepath=filepath.name, threads=threads)
            toc = perf_counter_ns()
            log(f"build_model: {(toc - tic)/1e9} secs")

            tic = perf_counter_ns()
            # sim_time()
            list_map_obj, solutions_x = a.pareto_front(pareto_steps, max_sec_per_run, run_per_step, max_nodes=max_nodes)
            
            toc = perf_counter_ns()
            log(f"build_pareto: {(toc - tic)/1e9} secs")
            log('*'*80)

            write_to_csv(f"{instance_name}_sol", list_map_obj)
            write_to_csv(f"{instance_name}_x", solutions_x)