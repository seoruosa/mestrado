import pygmo as pg
import numpy as np

def ref_point(pop):
    hv = pg.hypervolume(pop)
    
    return hv.refpoint()

def hypervolume(pop, ref_point=None):
    hv = pg.hypervolume(pop)
    
    if ref_point is None:
        ref_point_ = hv.refpoint()
    else:
        ref_point_ = ref_point
    
    return hv.compute(ref_point_)

def no_dominated(pontos):
    """"
    By Gladston Moreira

    Non-dominated dynamic programming  -  O(n log n)

    Input: 
    pontos = conjunto de pontos com duas posições

    Output:
    (idx_non_dominated, idx_dominated) = (indice dos pontos não dominados, indice dos pontos dominados)

    """
    
    n = pontos.shape[0] # quantidade  de pontos
    
    dominated = np.zeros(n,dtype=bool)
    
    ip = np.lexsort(pontos[:,::-1].T, axis=-1)
    pontos = pontos[ip] 
  
    D = True # Decision Variable

    for i in range(n-1):
        # store the time and the last node visited
        if D:
            z = pontos[i,1]
    
        if  pontos[i+1,1] < z: # i+1 is non-dominated
            D = True
        else:
            dominated[i+1] = True
            D = False
      
    return (ip[~dominated], ip[dominated])