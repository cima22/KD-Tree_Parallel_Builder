#!/bin/bash

#PBS -q dssc_gpu
#PBS -l nodes=2:ppn=48
#PBS -l walltime=03:00:00
#PBS -N tReE_gPu_WeAk

N=10000000
module load openmpi-4.1.1+gnu-9.3.0

cd $PBS_O_HOME/hpc/Assignment2
echo "timing,dim,threads" > times/omp_gpu_weak.csv
echo "timing,dim,procs" > times/mpi_gpu_weak.csv
echo "timing,dim,procs" > times/weak_serial.csv 

make
make serial

cd bin

for t in 1 2 4 8 16 32 64
do
 export OMP_NUM_THREADS=$t
 ./kdtree_omp $((N * t)) >> ../times/omp_gpu_weak.csv
done

for p in 1 2 4 8 16 32 64
do
 mpirun -np $p ./kdtree_mpi $((N * p)) >> ../times/mpi_gpu_weak.csv
done

for p in 1 2 4 8 16 32 64
do
 ./kdtree_serial $((N * p)) >> ../times/weak_serial.csv
done
