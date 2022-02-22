#!/bin/bash

#PBS -q dssc
#PBS -l nodes=1:ppn=48
#PBS -l walltime=02:00:00
#PBS -N omp_strrrrong

N=100000000

cd $PBS_O_HOME/hpc/Assignment2
echo "timing,dim,threads" > times/omp.csv
cd src
gcc -fopenmp -std=gnu99 -o ../bin/kdtree_omp kdtree_omp.c

cd ../bin
for t in 1 2 4 8 16 32 64
do
 export OMP_NUM_THREADS=$t
 ./kdtree_omp $N >> ../times/omp.csv
done
