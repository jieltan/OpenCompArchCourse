#!/bin/bash 

for i in 4 8 15 16 23 42
do
	echo "Running simulation of CAM with size $i"
	export CAM_SIZE=$i
	make clean simv &> /dev/null
	./simv | grep "@@@"
done

echo "Running synthesis"
export CAM_SIZE=8
make nuke syn_simv &> /dev/null
./syn_simv | grep "@@@"

