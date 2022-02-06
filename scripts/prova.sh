#!/bin/bash

#PBS -q dssc
#PBS -l nodes=2:ppn=24
#PBS -l walltime=01:40:00
#PBS -N ringostarr

module load openmpi-4.1.1+gnu-9.3.0
cd $PBS_O_HOME/hpc/Assigment1/section_1_measurements
echo "processors,timing" > ringdo3.csv
for i in {1..48..1}
do
  mpirun -np $i ./ring 2>/dev/null >> ringdo3.csv
done
