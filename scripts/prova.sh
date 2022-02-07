#!/bin/bash

#PBS -q dssc
#PBS -l nodes=1:ppn=24
#PBS -l walltime=01:40:00
#PBS -N tReE

export OMP_NUM_THREADS=128
export OMP_NESTED=TRUE

cd $PBS_O_HOME/hpc/Assignment2
echo "nested,timing" > times/omp.csv
cd src
gcc -fopenmp -std=gnu99 -o kdtree_omp kdtree_omp.c
./kdtree_omp 100000000 >> ../times/omp.csv
