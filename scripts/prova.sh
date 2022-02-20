#!/bin/bash

#PBS -q dssc
#PBS -l nodes=2:ppn=16
#PBS -l walltime=02:00:00
#PBS -N tReE_r

module load openmpi-4.1.1+gnu-9.3.0

N=100000000

cd $PBS_O_HOME/hpc/Assignment2
echo "timing,dim,threads" > times/omp.csv
echo "timing,dim,procs" > times/mpi.csv
cd src
gcc -fopenmp -std=gnu99 -o ../bin/kdtree_omp kdtree_omp.c
mpicc -std=gnu99 -o ../bin/kdtree_mpi kdtree_mpi.c -lm

cd ../bin
for t in 1 2 4 8 16 32 64 128 256
do
 export OMP_NUM_THREADS=$t
 ./kdtree_omp $N >> ../times/omp.csv
done
for p in 1 2 4 8 16 32
do
 mpirun -np $p ./kdtree_mpi $N >> ../times/mpi.csv
done
