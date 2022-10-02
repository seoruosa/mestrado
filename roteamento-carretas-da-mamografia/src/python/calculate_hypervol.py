#!/usr/bin/env python3

import sys
import pygmo as pg

if __name__ == "__main__":
    ref_point = []
    pareto = []
    for line in sys.stdin:
        if "REF_POINT" in line:
            ref_point_str = line.split(":")[1]
            ref_point = [float(el.strip()) for el in ref_point_str.split(',')] 
        else:
            try:
                point = [float(el.strip()) for el in line.split(";")]
                pareto.append(point)
            except:
                pass

    hv = pg.hypervolume(pareto)

    print(-hv.compute(ref_point))
