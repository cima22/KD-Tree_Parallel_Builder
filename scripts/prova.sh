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

export OMP_NUM_THREADS=1
./kdtree_omp $N >> ../times/omp.csv
export OMP_NUM_THREADS=2
./kdtree_omp $N >> ../times/omp.csv
export OMP_NUM_THREADS=4
./kdtree_omp $N >> ../times/omp.csv
export OMP_NUM_THREADS=8
./kdtree_omp $N >> ../times/omp.csv
export OMP_NUM_THREADS=16
./kdtree_omp $N >> ../times/omp.csv
export OMP_NUM_THREADS=24
./kdtree_omp $N >> ../times/omp.csv
export OMP_NUM_THREADS=48
./kdtree_omp $N >> ../times/omp.csv
export OMP_NUM_THREADS=96
./kdtree_omp $N >> ../times/omp.csv
