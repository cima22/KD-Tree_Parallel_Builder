#!/bin/bash

#PBS -q dssc_gpu
#PBS -l nodes=2:ppn=48
#PBS -l walltime=02:00:00
#PBS -N tReE_gPu

N=100000000
module load openmpi-4.1.1+gnu-9.3.0

cd $PBS_O_HOME/hpc/Assignment2
echo "timing,dim,threads" > times/omp_gpu.csv
echo "timing,dim,threads" > times/mpi_gpu.csv
cd src
gcc -fopenmp -std=gnu99 -o ../bin/kdtree_omp kdtree_omp.c
mpicc -std=gnu99 -o ../bin/kdtree_mpi kdtree_mpi.c -lm

cd ../bin

for t in 1 2 4 8 16 32 48 64 128 256
do
 export OMP_NUM_THREADS=$t
 ./kdtree_omp $N >> ../times/omp_gpu.csv
done

for p in 1 2 4 8 16 32 64
do
 mpirun -np $p ./kdtree_mpi $N >> ../times/mpi_gpu.csv
done
