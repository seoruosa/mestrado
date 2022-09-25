#!/bin/bash

# A partir das instancias do problema MDOVRP cria instâncias do problema MMURP

FOLDER_TEMP=Instancias/Vrp-Set-A_temp
FOLDER_VRP=Instancias/Vrp-Set-A
FOLDER_MMURP=Instancias/Vrp-Set-A_MMURP

mkdir $FOLDER_TEMP -p
mkdir $FOLDER_MMURP -p

cp $FOLDER_VRP/A-n50* $FOLDER_TEMP
cp $FOLDER_VRP/A-n100-m2-Q100.vrp $FOLDER_TEMP
cp $FOLDER_VRP/A-n100-m4-Q100.vrp $FOLDER_TEMP
cp $FOLDER_VRP/A-n249-m2-Q500.vrp $FOLDER_TEMP

source mmurp_env/bin/activate

for p in 20 40 60 80 100; do
    for m in 100; do
        python3 src/python/build_instances.py $FOLDER_TEMP $FOLDER_MMURP -p $p -m $m
        echo "Created instances: p: $p m: $m"
    done
done

rm -Rf $FOLDER_TEMP

deactivate

echo "DONE"