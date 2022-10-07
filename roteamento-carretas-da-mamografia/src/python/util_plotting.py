import matplotlib as plt
import matplotlib.colors as mcolors

import mo
from util_experiments import clean_sol_csv_file, read_out_file

def plt_scatter_dom_not_dom(pop, title="", figsize=(9, 9)):
    """
    Plot points of pop, divided by dominated and not dominated
    Usage:
        df_model = clean_sol_csv_file(model_path)
        pop = df_model[['dist', 'demand']].values*[1, -1]
        fig, ax = plt_scatter_dom_not_dom(pop, title="Model", figsize=(5, 5))
    """
    no_dom_idx, dom_idx = mo.no_dominated(pop)
    
    fig, ax = plt.subplots(figsize=figsize)
    
    plt.xlabel('Distância')
    plt.ylabel('- Demanda')
    plt.title(title)
    
    plt_idx = lambda idx, color, alpha: ax.scatter(pop[idx, 0], pop[idx, 1], c=color, alpha=alpha)
    
    plt_idx(no_dom_idx, 'tab:green', 0.7)
    plt_idx(dom_idx, 'tab:gray', 0.5)
    
    ax.legend(['Not dominated', 'Dominated'])
    return (fig, ax)

def plot_status_nsga(model_path=None, nsga_path=None, title='', figsize=(9, 9)):
    """
    Plot the points of model and nsga result on the same plot
    Usage:
        model_path = "../project/resultados/exp_20220927/A-n50-m4-Q80-v4-s100-d1_1_1_1_sol_20220927135621.csv"
        nsga_path = "../project/src/cpp/A-n50-m4-Q80-v4-s100-d1_1_1_1-20221005103016.out"

        fig, ax = plot_status_nsga(model_path, nsga_path, figsize=(5, 5))
    """
    if model_path == None and nsga_path == None:
        raise Exception("Both paths are None")
        
    fig, ax = plt.subplots(figsize=figsize)
    color_list = mcolors.TABLEAU_COLORS
    
    plt.xlabel('Distância')
    plt.ylabel('- Demanda')
    plt.title(title)
    
    if model_path != None:
        df_model = clean_sol_csv_file(model_path)

        all_status = list(set(df_model.status))
        color_sequence = {i:color for i, color in enumerate(color_list)}

        color_map = {el:color_sequence[i] for i, el in enumerate(all_status)}

        for status in all_status:
            df_ = df_model[df_model.status == status]

            x = df_.dist
            y = -df_.demand
            colors = [color_map[status] for status in df_.status]

            ax.scatter(x, y, c=colors, alpha=0.5)
    
    if nsga_path != None:
        df_, params = read_out_file(nsga_path)
        
        ax.scatter(df_.dist, -df_.demand, c='tab:red', marker='*', alpha=0.5)
        ax.legend([*all_status, 'NSGAII'])
    
    return (fig, ax)