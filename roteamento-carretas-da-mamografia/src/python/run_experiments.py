#!/usr/bin/env python3

from time import gmtime, strftime
from time import perf_counter_ns

from multiprocessing.pool import ThreadPool
import subprocess
import os
import argparse

from util_argparse import dir_path

def program_argument(instance_path, output_path):
    program_name = "./src/cpp/build/NSGAII_MMURP"
    configurations = "--sizePop 100 --numGen 20 --mutationRate 0.4"
    out_folder_path = "resultados/exp_test"

    return f"{program_name} {instance_path} {configurations} > {out_folder_path}/{output_path}"

def work(sample):
    i, filepath = sample

    instance_name = (filepath.split('/')[-1]).split('.')[0]

    time = strftime("%Y%m%d%H%M%S", gmtime())
    output_file = f"{instance_name}_{i}_{time}.result"
    
    tic = perf_counter_ns()

    prog_str = program_argument(filepath, output_file)
    # my_tool_subprocess = subprocess.Popen(f"./build/NSGAII_MMURP {filepath} --sizePop 100 --numGen 1500 --mutationRate 0.15 > ../tests/{output_file}",shell=True, stdout=subprocess.PIPE)
    
    my_tool_subprocess = subprocess.Popen(prog_str,shell=True, stdout=subprocess.PIPE)

    # my_tool_subprocess = subprocess.Popen(f"python3 ./src/python/playground.py",shell=True, stdout=subprocess.PIPE)
    my_tool_subprocess.wait()
    toc = perf_counter_ns()

    period = (toc - tic)/1e9

    print(f"{instance_name} - {i} - {period:.2e} s")

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
    parser.add_argument("testPath", type=dir_path)
    parser.add_argument("-n", "--nRuns", type=float, required=True, help="Select the number of runs per instance")
    args = parser.parse_args()

    RUNS_PER_INSTANCE = int(args.nRuns)
    testPath = args.testPath

    test_instances = list_test_instances(testPath)
    
    all_runs = [(i, os.path.join(testPath, instance)) for i in range(RUNS_PER_INSTANCE) for instance in test_instances]
    
    for r in all_runs:
        print(r)

    run(all_runs)