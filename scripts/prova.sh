#!/bin/bash

#PBS -q dssc
#PBS -l nodes=1:ppn=24
#PBS -l walltime=02:00:00
#PBS -N tReE

export OMP_NESTED=TRUE
N=1000000000

cd $PBS_O_HOME/hpc/Assignment2
echo "timing,dim,threads" > times/omp.csv
cd src
gcc -fopenmp -std=gnu99 -o kdtree_omp kdtree_omp.c

export OMP_NUM_THREADS=32
./kdtree_omp $N >> ../times/omp.csv
export OMP_NUM_THREADS=64
./kdtree_omp $N >> ../times/omp.csv
export OMP_NUM_THREADS=128
./kdtree_omp $N >> ../times/omp.csv
export OMP_NUM_THREADS=256
./kdtree_omp $N >> ../times/omp.csv
