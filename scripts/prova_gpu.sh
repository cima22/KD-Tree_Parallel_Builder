#!/bin/bash

#PBS -q dssc_dssc_gpu
#PBS -l nodes=1:ppn=48
#PBS -l walltime=02:00:00
#PBS -N tReE_gPu

N=100000000

cd $PBS_O_HOME/hpc/Assignment2
echo "timing,dim,threads" > times/omp_gpu.csv
cd src
gcc -fopenmp -std=gnu99 -o kdtree_omp kdtree_omp.c

export OMP_NUM_THREADS=1
./kdtree_omp $N >> ../times/omp_gpu.csv
export OMP_NUM_THREADS=2
./kdtree_omp $N >> ../times/omp_gpu.csv
export OMP_NUM_THREADS=4
./kdtree_omp $N >> ../times/omp_gpu.csv
export OMP_NUM_THREADS=8
./kdtree_omp $N >> ../times/omp_gpu.csv
export OMP_NUM_THREADS=16
./kdtree_omp $N >> ../times/omp_gpu.csv
export OMP_NUM_THREADS=24
./kdtree_omp $N >> ../times/omp_gpu.csv
export OMP_NUM_THREADS=48
./kdtree_omp $N >> ../times/omp_gpu.csv
export OMP_NUM_THREADS=96
./kdtree_omp $N >> ../times/omp_gpu.csv
export OMP_NUM_THREADS=128
./kdtree_omp $N >> ../times/omp_gpu.csv
