import numpy as np

def MDOVRP(filepath):
    # --- LE AS INSTANCIAS ---
    # guarda o objeto do arquivo
    DIST = "DIST"
    EUC_2D = "EUC_2D"

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

            #print(line.strip())         

        N = [i for i in range(n)]
        D = [i for i in range(n, n+m)]
        V = [i for i in range(n+m)]

        if edge_weight_type == EUC_2D:
            c = {(i, j) : np.hypot(coord_x[i] - coord_x[j], coord_y[i] - coord_y[j]) for i in V for j in V }
        
        return (N, D, V, Q, q, c, coord_x, coord_y)