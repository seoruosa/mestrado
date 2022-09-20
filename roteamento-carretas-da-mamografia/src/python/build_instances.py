from read_instance import MDOVRP
from math import ceil
from numpy import percentile

import os
from io import StringIO

import argparse
from util_argparse import dir_path


def distribute_cars_depot(n_cars, n_depot):
    n_ref = n_cars//n_depot
    n_rest = n_cars % n_depot

    depot = [(n_ref + 1 if i < n_rest else n_ref)
             for i in range(n_depot)]

    return depot


def build_mmurp_instance_from_mdovrp_str(filepath, perc, maxDist):
    output = ''

    N, D, V, Q, q, c, coord_x, coord_y = MDOVRP(filepath)

    if c != None:
        if maxDist < 100:
            max_distance = ceil(percentile(
                [v for v in c.values() if v > 0], maxDist))
        else:
            max_distance = ceil(max(c.values()))

        number_vehicles = ceil(perc / 100 * sum(q) / Q)
        dist_cars_depot = distribute_cars_depot(number_vehicles, len(D))

        with StringIO() as out:
            with open(filepath, mode='r') as f:
                for line in f.readlines():
                    if "EDGE_WEIGHT_TYPE" in line:
                        print(f"MAX_TRAVEL_DIST : {max_distance}", file=out)

                    if "EOF" in line:
                        print("DEPOT_VEHICLES_SECTION", file=out)
                        for n_cars in dist_cars_depot:
                            print(n_cars, file=out)
                    print(line, end='', file=out)
            output = out.getvalue()

    return (output, dist_cars_depot)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Given a folder with instances of MDOVRP, creates \
        a MMURP instance for each one and save on destination folder.")
    parser.add_argument("instanceDirPath", type=dir_path,
                        help="Folder of original instances.")
    parser.add_argument("outDirPath", type=dir_path,
                        help="Destination of new instances.")
    parser.add_argument("-p", "--perc", type=int, help="Percentage of total vehicles that will be \
        used on the new instance.")
    parser.add_argument("-m", "--maxDist", type=int, default=100, help="Given the maximum distance \
        between localities, the maximum distance that a MMU could travel will be the percentile \
            equal to maxDist.")

    args = parser.parse_args()

    folderpath = args.instanceDirPath
    out_folderpath = args.outDirPath
    perc = args.perc
    maxDist = args.maxDist

    for filename in os.listdir(folderpath):
        filepath = os.path.join(folderpath, filename)

        if os.path.isfile(filepath):
            instance_str, dist_cars_depot = build_mmurp_instance_from_mdovrp_str(
                filepath, perc, maxDist)

            out_filename = f"-v{sum(dist_cars_depot)}-s{maxDist}-d{'_'.join([str(a) for a in dist_cars_depot])}.".join(
                filename.split('.'))

            with open(os.path.join(out_folderpath, out_filename), mode='x') as out:
                print(instance_str, file=out, end='')
