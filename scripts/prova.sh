#!/bin/bash

#PBS -q dssc
#PBS -l nodes=1:ppn=24
#PBS -l walltime=02:00:00
#PBS -N tReE

N=100000000

cd $PBS_O_HOME/hpc/Assignment2
echo "timing,dim,threads" > times/omp.csv
cd src
gcc -fopenmp -std=gnu99 -o kdtree_omp kdtree_omp.c

for t in 1 2 4 8 16 24 32 64 128 256
do
 export OMP_NUM_THREADS=$t
 ./kdtree_omp $N >> ../times/omp.csv
done
