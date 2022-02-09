#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

#ifndef DOUBLE_PRECISION
typedef float float_t;
#else
typedef double float_t;
#endif
#define NDIM 2

//------------- Data Structures --------------------------------------------------------------


typedef float_t kpoint[NDIM];

typedef struct knode{
	int axis;
	kpoint split;
	struct knode *left, *right;
} knode;

//------------- Function Declaration ---------------------------------------------------------


knode * build_kdtree(kpoint * points, int n, int ndim, int axis);

int choose_splitting_dimension(int axis, int ndim);

kpoint* choose_splitting_point(kpoint * points, int n, int ndim, int axis);

kpoint * initialize(int n);

void my_qsort(kpoint * points, int n, int el_len, int axis);

int comp_x(const void * el1, const void * el2);

int comp_y(const void * el1, const void * el2);

void print_tree(knode * tree);

//---------------- Main -----------------------------------------------------------------------

int main(int argc, char* argv[]){
 
 MPI_Init(%argc, &argv);

 double start, end;
 int N = argc >= 2 ? atoi(argv[1]) : 100000000;

 kpoint * points;

 int rank, size;
 MPI_Comm_size(MPI_COMM_WORLD, &size);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank;

 if(rank == 0){ 
  points = initialize(N);

  start = MPI_Wtime();
  knode * kd_tree = build_kdtree(points, N, NDIM, -1);
  end = MPI_Wtime() - start;

  printf("%.5f,%d,%d\n", end, N, size);

  free(points);
 }

 return 0;
}

// ---------------------- Functions -----------------------------------------------------------
//
// initialize() ------------------------------------------------------------------------------


kpoint * initialize(int n){

 kpoint * points = (kpoint *) malloc(n * sizeof(kpoint));

 if(points == NULL){
  printf("Problem with malloc()");
  return NULL;
 }

 srand((int) getpid());

 for(int i = 0; i < n; i++){
  points[i][0] = drand48();
  points[i][1] = drand48();
 }

 return points;
}

// build_kdtree() ---------------------------------------------------------------------------

knode * build_kdtree(kpoint * points, int n, int ndim){
 if(n == 0) return NULL;

 if(n == 1){
  knode * leaf = (knode *) malloc(sizeof(knode));
  leaf -> axis  = 0;
  memcpy(leaf->split, points, sizeof(kpoint*));
  leaf -> left = NULL;
  leaf -> right = NULL;
  #ifdef DEBUG
   printf("\n\n(%f, %f)\n\n", points[0][0], points[0][1]);
  #endif
  return leaf;
 }
 
 knode * node = (knode *) malloc(sizeof(knode));
 if(node == NULL){
  printf("Problem with malloc()");
  return NULL;
 }

 int my_axis = choose_splitting_dimension(axis, ndim);

 kpoint * my_point = choose_splitting_point(points, n, ndim, my_axis);

 #ifdef DEBUG
  for(int i = 0; i < n; i++)
   printf("(%f, %f)\n", points[i][0], points[i][1]);
  printf("\n\n");
 #endif

 int N_left, N_right;
 int median_index = (int) (n/2);

 N_left = median_index;
 N_right = n % 2 == 0 ? median_index - 1 : median_index;

 kpoint * left_points, right_points;
 left_points = (kpoint *) points;
 right_points = (kpoint *) (points) + N_left + 1;

 node -> axis = my_axis;
 memcpy(node -> split, my_point, sizeof(kpoint *));

 if(size != 1){
  MPI_Send(right_points, N_right * 2, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
 }
 
}

 //free(temp);
 //

return node;
}
