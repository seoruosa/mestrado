#!/bin/bash


# A partir das instancias do problema MDOVRP cria instâncias do problema MMURP

DIA=`date +%Y%m%d`
NAME_FOLDER='metodo_exato'

INSTANCES_FOLDER=$1
RESULTADOS_FOLDER=$2${NAME_FOLDER}/$DIA

LOG_FILE=$RESULTADOS_FOLDER/commit.txt

mkdir -p ${RESULTADOS_FOLDER}

# echo $(git log --name-status HEAD^..HEAD) >> $LOG_FILE
# echo `date` >> $LOG_FILE

for instance in $(ls "$INSTANCES_FOLDER"); do
    echo $instance
    python3 src/python/run_mmurp_model.py "$INSTANCES_FOLDER/$instance" --logDir $RESULTADOS_FOLDER -n 30 -t 600
done