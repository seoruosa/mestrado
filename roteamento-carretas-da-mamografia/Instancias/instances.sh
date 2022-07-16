#!/bin/bash
INSTANCES_PATH=$1

for FILE in $(ls $INSTANCES_PATH)
do
    python3 read_instance.py $INSTANCES_PATH/$FILE
done