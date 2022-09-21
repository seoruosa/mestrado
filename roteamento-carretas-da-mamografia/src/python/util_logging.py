import csv
from time import gmtime, strftime

def write_to_csv(name:str, list_map) -> None:
    time = strftime("%Y%m%d%H%M%S", gmtime())
    filepath = f"{name}_{time}.csv"

    headers = list(list_map[0].keys())
    with open(filepath, 'w', newline='') as myfile:
        writer = csv.DictWriter(myfile, fieldnames=headers)
        writer.writeheader()
        writer.writerows(list_map)