#!/bin/bash


# A partir das instancias do problema MDOVRP cria instâncias do problema MMURP


INSTANCES_FOLDER=Instancias/Vrp-Set-A_MMURP
RESULTADOS_FOLDER=resultados/

for instance in $(ls "$INSTANCES_FOLDER"); do
    echo $instance
    python3 src/python/run_mmurp_model.py "$INSTANCES_FOLDER/$instance" --logDir $RESULTADOS_FOLDER
done