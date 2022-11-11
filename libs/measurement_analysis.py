import re
import pandas as pd
import sys
import matplotlib.pyplot as plt
import os

def read_nsga_measurements(filepath):
    re_two_points = re.compile(r'\s\:\s')

    dict_list = []
    i = 0
    with open(filepath, mode='r') as f:
        dict_el = {}
        for a in f.readlines():
            if ':' in a:
                b = re_two_points.split(a)
                dict_el[b[0]] = float(b[1])
            if ('*' in a) and (i > 0):
                dict_list.append(dict_el)
                dict_el = {}
            i = i + 1
    df = pd.DataFrame(dict_list)
    df.drop(columns=['generation'], inplace=True)
    
    return df

if __name__ == '__main__':
    if len(sys.argv) > 1:
        filepath = sys.argv[1]
        name = os.path.basename(filepath).split('.')[0]
        
        df = read_nsga_measurements(filepath)

        print("*"*15)
        print(f"** {name} **")
        print(df.sum())
        print(f"iteracoes: {df.shape[0]}")

        df.boxplot(rot=45, fontsize=15, figsize=(18, 12))
        plt.title(name)
        plt.savefig(f"{name}.png")