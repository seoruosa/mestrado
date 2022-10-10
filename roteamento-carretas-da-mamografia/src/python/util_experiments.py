import pandas as pd
import numpy as np

import os
import glob

import pandas as pd
import pygmo as pg

# import mo
import src.mo as mo # somente para funcionar no notebook

def clean_sol_csv_file(filepath):
    """
    Return a dataframe with the points of pareto front 
        from model solution csv file 
    Usage:
        filepath = "../project/resultados/exp_20220927/A-n50-m4-Q80-v4-s100-d1_1_1_1_sol_20220927135621.csv"
        df = clean_sol_csv_file(filepath)
    """
    df = pd.read_csv(filepath)
    df['status'] = [a.split('.')[1] for a in df.status.values]
    
    return df


def read_out_file(filepath):
    """
    Return a dataframe with the points of pareto front and 
        a dict with parameters of output of NSGAII run
    Usage:
        df, params = read_out_file(nsga_path)
    """
    skiprows = 0
    param_map = {}
    
    with open(filepath) as file:
        for line in file:
            if ':' in line:
                key, value = line.split(':')
                param_map[key.strip()] = value.strip()
            
            if ';' in line:
                break
            
            skiprows += 1
                
    df = pd.read_csv(filepath, sep=';', skiprows=skiprows)
    df.columns = [col.strip() for col in df.columns]

    df['f_1'] = -df['f_1']
    df = df[['f_0', 'f_1']]

    df.columns = ['dist', 'demand']
    
    # convert to number if it is possible
    format_key = lambda key: key.replace(' ', '_').lower()

    param_map = {format_key(key):conv_to_number(value) for key, value in param_map.items()}
        
    return (df, param_map)

def read_pos_processed_csv(filepath):
    """
        Read a csv file with all objectives values found (after a post processing), 
        given the big tour solutions, clean the dominated solutions and returns a 
        pandas.DataFrame
        Usage:
            nsga_pp_path = "../project/resultados/exp_20221005/n50/A-n50-m4-Q160-v4-s100-d1_1_1_1-20221005194618.csv"
            df_nd = read_pos_processed_csv(nsga_pp_path)
    """
    COLUMNS = ['dist', 'demand']
    
    df = pd.read_csv(filepath)
    df.columns = COLUMNS

    pop = np.array(list(set([(v[0], v[1]) for v in df.values])))
    non_dom, _ = mo.no_dominated(pop*[1, -1])

    return pd.DataFrame(pop[non_dom], columns=COLUMNS)

def conv_to_number(value):
    """
        Convert to number when it's possible, if it's not return the value
        Usage:
            value = 'A-n50-m4-Q80-v4-s100-d1_1_1_1'
            print(conv_to_number(value))
    """
    try:
        return float(value)
    except:
        return value


def nsga_output_to_df(folderpath):
    """
        Iterate over files with extension '.out' and 
        returns a DataFrame with configurations and 
        output results
        Usage:
            folderpath = "../project/resultados/exp_20221005/n50/"
            df = nsga_output_to_df(folderpath)
    """
    
    all_results = []
    for filepath in glob.glob(f"{folderpath}/*.out"):
        if os.path.isfile(filepath):
            df, params = read_out_file(filepath)

            pop = df[['dist', 'demand']].values * [1, -1]        
            ref_point = [float(a) for a in params['ref_point'].split(',')]
            hv = mo.hypervolume(pop, ref_point)
            non_dominated, _ = mo.no_dominated(pop)

            params = {**params, 'hv':hv, 'non_dominated': len(non_dominated)}

            csv_path = filepath.replace('.out', '.csv')
            if os.path.isfile(csv_path):
                df_pp = read_pos_processed_csv(csv_path)
                
                pop_pp = df_pp[['dist', 'demand']].values * [1, -1]        
                hv_pp = mo.hypervolume(pop_pp, ref_point)
                non_dominated_pp, _ = mo.no_dominated(pop_pp)
                
                params = {**params, 'hv_post_proc': hv_pp , 'non_dominated_post_proc': len(non_dominated_pp)}
            
            all_results.append(params)

    return pd.DataFrame(all_results)