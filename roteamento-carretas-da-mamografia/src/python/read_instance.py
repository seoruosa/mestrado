from math import ceil
import numpy as np
import argparse
import os

def MDOVRP(filepath):
    # --- LE AS INSTANCIAS ---
    # guarda o objeto do arquivo
    DIST = "DIST"
    EUC_2D = "EUC_2D"
    N, D, V, Q, q, c, coord_x, coord_y = ([None for a in range(8)])

    coord_x = []
    coord_y = []
    q = []
    c = {}

    with open(filepath, "r") as f:  
        while(True):
            #le a proxima linha
            line = f.readline()

            if "EOF" in line:
                break

            if "NODES" in line:
                n = int(line.split()[2]) 

            if "DEPOTS" in line:
                m = int(line.split()[2]) 

            if "CAPACITY" in line:
                Q = int(line.split()[2])
            
            if "EDGE_WEIGHT_TYPE" in line:
                edge_weight_type = line.split()[2]

            if "NODE_COORD_SECTION" in line:
                if edge_weight_type == EUC_2D:
                    for i in range(n):
                        line = f.readline()
                        coord_x.append( int(line.split()[1]) )
                        coord_y.append( int(line.split()[2]) )
                if edge_weight_type == DIST:
                    for i in range(n):
                        line = f.readline()
                        print(f"LINE => {i} from {n}")
                        for j, value in enumerate(line.split()):
                            c[(i,j)] = float(value)
                        
            if "DEMAND_SECTION" in line: 
                for i in range(n):
                    line = f.readline()
                    q.append( int(line.split()[1]) )

            if "DEPOT_SECTION" in line: 
                if edge_weight_type == EUC_2D:
                    for i in range(m):
                        line = f.readline()
                        coord_x.append( int(line.split()[1]) )
                        coord_y.append( int(line.split()[2]) )
                        q.append( 0 )
                if edge_weight_type == DIST:
                    for i in range(m):
                        line = f.readline()
                        for j, value in enumerate(line.split()):
                            c[(n + i,j)] = float(value)
                        q.append( 0 )

        N = [i for i in range(n)]
        D = [i for i in range(n, n+m)]
        V = [i for i in range(n+m)]

        if edge_weight_type == EUC_2D:
            c = {(i, j) : np.hypot(coord_x[i] - coord_x[j], coord_y[i] - coord_y[j]) for i in V for j in V }
        
    return (N, D, V, Q, q, c, coord_x, coord_y)

def MMURP(filepath):
    # --- LE AS INSTANCIAS ---
    # guarda o objeto do arquivo
    N, D, V, Q, max_dist_nodes, q, c, vehicles_depot, coord_x, coord_y = ([None for a in range(10)])

    DIST = "DIST"
    EUC_2D = "EUC_2D"

    coord_x = []
    coord_y = []
    q = []
    c = {}
    vehicles_depot = []

    with open(filepath, "r") as f:  
        while(True):
            #le a proxima linha
            line = f.readline()

            if "EOF" in line:
                break

            if "NODES" in line:
                n = int(line.split()[2]) 

            if "DEPOTS" in line:
                m = int(line.split()[2]) 

            if "CAPACITY" in line:
                Q = int(line.split()[2])
            
            if "EDGE_WEIGHT_TYPE" in line:
                edge_weight_type = line.split()[2]
            
            if "MAX_TRAVEL_DIST" in line:
                max_dist_nodes = int(line.split()[2])

            if "NODE_COORD_SECTION" in line:
                if edge_weight_type == EUC_2D:
                    for i in range(n):
                        line = f.readline()
                        coord_x.append( int(line.split()[1]) )
                        coord_y.append( int(line.split()[2]) )
                if edge_weight_type == DIST:
                    for i in range(n):
                        line = f.readline()
                        for j, value in enumerate(line.split()):
                            c[(i,j)] = float(value)
                        

            if "DEMAND_SECTION" in line: 
                for i in range(n):
                    line = f.readline()
                    q.append( int(line.split()[1]) )

            if "DEPOT_SECTION" in line: 
                if edge_weight_type == EUC_2D:
                    for i in range(m):
                        line = f.readline()
                        coord_x.append( int(line.split()[1]) )
                        coord_y.append( int(line.split()[2]) )
                        q.append( 0 )
                if edge_weight_type == DIST:
                    for i in range(m):
                        line = f.readline()
                        for j, value in enumerate(line.split()):
                            c[(n + i,j)] = float(value)
                        q.append( 0 )
            if "DEPOT_VEHICLES_SECTION" in line:
                for i in range(m):
                    line = f.readline()
                    vehicles_depot.append(int(line))

            #print(line.strip())         

        N = [i for i in range(n)]
        D = [i for i in range(n, n+m)]
        V = [i for i in range(n+m)]

        if edge_weight_type == EUC_2D:
            c = {(i, j) : np.hypot(coord_x[i] - coord_x[j], coord_y[i] - coord_y[j]) for i in V for j in V }
        
    return (N, D, V, Q, max_dist_nodes, q, c, vehicles_depot, coord_x, coord_y)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog="READ_INSTANCE")
    parser.add_argument("instance", type=argparse.FileType('r'))
    
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('--mmurp', action='store_true')
    group.add_argument('--mdovrp', action='store_true')

    args = parser.parse_args()

    filepath = args.instance.name
    filename = os.path.basename(filepath)
    instance_name = os.path.splitext(filename)[0]
    
    if args.mmurp:
        N, D, V, Q, max_dist_nodes, q, c, vehicles_depot, coord_x, coord_y = MMURP(filepath)
        print('MMURP')
    elif args.mdovrp:
        N, D, V, Q, q, c, coord_x, coord_y = MDOVRP(filepath)
        print('MDOVRP')
    else:
        pass

    # print(args)

    print(f"Instancia: {instance_name}")
    print(f"Demanda Total: {sum(q)}")
    print(f"Capacidade veículo: {Q}")
    print(f"Demanda/Capacidade(numero minimo de carros para atender 100% da demanda) : {ceil(sum(q)/Q)}")
    
    if args.mmurp:
        print(f"Veiculos por deposito: {', '.join((str(a) for a in vehicles_depot))}")
    
    print('*'*80)