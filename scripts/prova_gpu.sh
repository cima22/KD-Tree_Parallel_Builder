#!/bin/bash

#PBS -q dssc_gpu
#PBS -l nodes=1:ppn=48
#PBS -l walltime=02:00:00
#PBS -N tReE_gPu

N=100000000

cd $PBS_O_HOME/hpc/Assignment2
echo "timing,dim,threads" > times/omp_gpu.csv
cd src
gcc -fopenmp -std=gnu99 -o kdtree_omp kdtree_omp.c

for t in 1 2 4 8 16 32 48 64 128 256
do
 export OMP_NUM_THREADS=$t
 ./kdtree_omp $N >> ../times/omp_gpu.csv
done
