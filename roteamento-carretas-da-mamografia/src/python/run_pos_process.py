#!/usr/bin/env python3

from time import gmtime, strftime
from time import perf_counter_ns

from multiprocessing.pool import ThreadPool
import subprocess
import os
import argparse
import glob

from util_argparse import dir_path

def program_argument(instances_folderpath, output_filepath):
    
    csv_name = f"{os.path.basename(output_filepath).split('.out')[0]}.csv"
    instance_name = '-'.join(os.path.basename(output_filepath).split('-')[:-1])
    instance_path = os.path.join(instances_folderpath, f"{instance_name}.vrp")
    out_folderpath = os.path.dirname(output_filepath)
    
    program_name = "./src/cpp/build/mmurp_post_process"
    
    return f"{program_name} {output_filepath} {instance_path} > {out_folderpath}/{csv_name}"

def work(sample):
    filepath, instances_folderpath = sample

    instance_name = os.path.basename(filepath)
    
    tic = perf_counter_ns()

    prog_str = program_argument(instances_folderpath, filepath)
    
    my_tool_subprocess = subprocess.Popen(prog_str,shell=True, stdout=subprocess.PIPE)

    my_tool_subprocess.wait()
    toc = perf_counter_ns()

    period = (toc - tic)/1e9

    print(f"{instance_name} - {period:.2e} s")

def run(all_samples):
    num = 3  # set to the number of workers you want (it defaults to the cpu count of your machine)
    tp = ThreadPool(num)
    
    for sample in all_samples:
        tp.apply_async(work, (sample,))

    tp.close()
    tp.join()

def list_test_instances(testFolderPath):
    filesList = lambda path: [name for name in os.listdir(path) if os.path.isfile(os.path.join(path, name))]

    test_instances = filesList(testFolderPath)
    
    return test_instances


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("testFolderPath", type=dir_path)
    parser.add_argument("instancesFolderPath", type=dir_path)
    args = parser.parse_args()

    RUNS_PER_INSTANCE = 1
    testFolderPath = args.testFolderPath
    instancesFolderPath = args.instancesFolderPath

    all_runs = []
    for filepath in glob.glob(f"{testFolderPath}/*.out"):
        if os.path.isfile(filepath):
            # print(filepath)
            all_runs.append((filepath, instancesFolderPath))

    run(all_runs)