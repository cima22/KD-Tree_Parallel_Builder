# Assignment 2 - Foundations of High Performance Computing
## Parallel build of a kdtree
Project related to the course "Foundations of High Performance Computing" of the Master's Degree in Computationl Science and Engineering @ UniTS. The purpose of this assignment is to develop both OpenMP and MPI versions of a program that builds a kdtree.
## Compile
In the root folder there is a Makeefile available. Just type
```bash
make
```
to build the objects files in the obj directory and the executables in the bin directory.
### Serial version
To build the executable of the serial version just type
```bash
make serial
```
### Debug
To get debuggable executables just type
```bash
make debug
```
the executables will be created in the debug folder.
### View
To compile the executable version that prints the workflow done by the program just type
```bash
make view
```
## Execute
### Input
Both versions expect as arguments the size of the dataset. The dataset of 2-dimensional points will then be randomly generated. If no size is given, the program will build the kdtree with a size of N = 10000000.
### Output
The output of the program will be the time taken to build the tree, the size of the set and the number of processes/threads used; e.g.:
```bash
0.1703,100000,4
```
tells that the build took 0.1703 seconds, with a set size of 100000 using 4 processes or threads.
### Output - view
If the program has been compiled using
```bash
make view
```
Every process/thread will print, at every split, which node has been generated and based on which points.
### OpenMP version
On the root folder, just go with
```bash
bin/kdtree_omp N
```
where N is the size of the dataset.
### MPI version
Be sure to have a propper implementation for the MPI Standard, e.g. OpenMPI. Then go with
```bash
mpirun -np p bin/kdtree_mpi N
```
where p is the number of processes you want to use.
