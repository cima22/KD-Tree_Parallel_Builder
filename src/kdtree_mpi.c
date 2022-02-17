#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

#ifndef DOUBLE_PRECISION
typedef float float_t;
#else
typedef double float_t;
#endif
#define NDIM 2
//#define DEBUG 0

//------------- Data Structures --------------------------------------------------------------


typedef float_t kpoint[NDIM];

typedef struct knode{
	int axis;
	kpoint split;
	struct knode *left, *right;
} knode;

typedef struct array {
	int dim;
	float_t * start;
} array;

typedef struct des {
	knode * tree;
	int index;
} des;

int size, rank;

knode * kd_tree;

//------------- Function Declaration ---------------------------------------------------------


knode * build_kdtree(kpoint * points, int n, int ndim, int axis, int depth);

knode * start_build();

int choose_splitting_dimension(int axis, int ndim);

kpoint * choose_splitting_point(kpoint * points, int n, int ndim, int axis);

kpoint * initialize(int n);

void glue_trees(knode * tree);

array serialize(knode * tree, float_t * sequence, int dim);

knode * deserialize(float_t * ser);

void my_qsort(kpoint * points, int n, int el_len, int axis);

int comp_x(const void * el1, const void * el2);

int comp_y(const void * el1, const void * el2);

void print_tree(knode * tree);

int find_depth(knode * tree);

//---------------- Main -----------------------------------------------------------------------

int main(int argc, char* argv[]){
 
 MPI_Init(&argc, &argv);

 double start, end;
 int N = argc >= 2 ? atoi(argv[1]) : 100000000;

 kpoint * points;

 MPI_Comm_size(MPI_COMM_WORLD, &size);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);

 if(rank == 0){ 
  points = initialize(N);

  start = MPI_Wtime();
  kd_tree = build_kdtree(points, N, NDIM, -1, 0);
  end = MPI_Wtime() - start;
  printf("%.5f,%d,%d\n", end, N, size);
  
  //free(points);
 }

 else {
  kd_tree = start_build();
  }

 MPI_Barrier(MPI_COMM_WORLD);
 glue_trees(kd_tree);

 MPI_Barrier(MPI_COMM_WORLD);
 if(rank == 0){
 // print_tree(kd_tree);
 printf("\nFinito\n");
 }
 MPI_Barrier(MPI_COMM_WORLD);
 printf("%d the last barrier\n", rank);
 MPI_Finalize();
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

 srand(time(NULL));

 for(int i = 0; i < n; i++){
  points[i][0] = drand48();
  points[i][1] = drand48();
 }

 return points;
}

// build_kdtree() ---------------------------------------------------------------------------

knode * build_kdtree(kpoint * points, int n, int ndim, int axis, int depth){
 if(n == 0) return NULL;

 if(n == 1){
  knode * leaf = (knode *) malloc(sizeof(knode));
  leaf -> axis  = 0;
  memcpy(leaf->split, points, sizeof(kpoint*));
  leaf -> left = NULL;
  leaf -> right = NULL;
  #ifdef DEBUG
   printf("\nrank:%d\n\n(%f, %f)\n\n", rank, points[0][0], points[0][1]);
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
  printf("\n%d:\n", rank);
  for(int i = 0; i < n; i++)
   printf("(%f, %f)\n", points[i][0], points[i][1]);
  printf("\n\n");
 #endif

 int N_left, N_right;
 int median_index = (int) (n/2);

 N_left = median_index;
 N_right = n % 2 == 0 ? median_index - 1 : median_index;

 kpoint * left_points, * right_points;
 left_points = (kpoint *) points;
 right_points = (kpoint *) (points) + N_left + 1;

 node -> axis = my_axis;
 memcpy(node -> split, my_point, sizeof(kpoint *));

//sending right branch and the other paramteres to the right process
 if(size != 1 && (depth < log2(size))){
  int dest_rank = rank + (size / pow(2,depth + 1));
  int params[] = {N_right, ndim, my_axis, depth + 1};
  MPI_Send(params, 4, MPI_INT, dest_rank, rank * 100, MPI_COMM_WORLD); // # params
  MPI_Send((float_t *) right_points, N_right * 2, MPI_FLOAT, dest_rank, rank * 100, MPI_COMM_WORLD); // points as coordinates

  node -> left = build_kdtree(left_points, N_left, ndim, my_axis, depth + 1); //left branch continues on same process
  
  knode * missing = malloc(sizeof(knode));
  missing -> axis = -1;
  missing -> left = NULL;
  missing -> right = NULL;
  node -> right = missing;
 }
 
 // if there are no left processes, the processes continues indipentendly
 else if(depth >= log2(size)){
  right_points = (kpoint *) right_points;
  node -> left = build_kdtree(left_points, N_left, ndim, my_axis, log2(size));
  node -> right = build_kdtree(right_points, N_right, ndim, my_axis, log2(size));
 }

 //free(temp);

return node;
}

// start_build() -----------------------------------------------------------------------------------

knode * start_build(){
 int params[4];
 MPI_Status * status;
 float * coords;
 kpoint * points;

 MPI_Recv(params, 4, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, status); // Receive number of points
 coords = malloc(params[0] * 2 * sizeof(float));
 points = malloc(params[0] * sizeof(kpoint));
 MPI_Recv(coords, params[0] * 2, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, status); // Receive coordinates

 // Initialize the points
 for(int i = 0; i < params[0]; i++){
  points[i][0] = coords[2*i];
  points[i][1] = coords[2*i + 1];
 }
 free(coords);

 knode * sub_tree = build_kdtree(points, params[0], params[1], params[2], params[3]);
 //print_tree(sub_tree);
 return sub_tree;
}

// choose_splitting_dimension() ------------------------------------------------------------------------------
int choose_splitting_dimension(int axis, int ndim){ return (axis + 1) % ndim; }

// choose_splitting_point() ---------------------------------------------------------------------------------
kpoint* choose_splitting_point(kpoint* points, int n, int ndim, int axis){
 my_qsort(points, n, sizeof(kpoint), axis);
 kpoint * median = (kpoint*) points[(int) (n/2)]; // median of the sorted points
 return median;
}

// my_qsort() ---------------------------------------------------------------------------

void my_qsort(kpoint * points, int n, int el_len, int axis){
 if(axis == 0)
  qsort(points, n, el_len, comp_x);
 else if (axis == 1)
  qsort(points, n, el_len, comp_y);
 }

int comp_x(const void * el1, const void * el2){
 float_t val1 = (*((kpoint *) el1))[0];
 float_t val2 = (*((kpoint *) el2))[0];
 return val1 > val2 ? 1 : val1 < val2 ? -1 : 0;
}

int comp_y(const void * el1, const void * el2){
 float_t val1 = (*((kpoint *) el1))[1];
 float_t val2 = (*((kpoint *) el2))[1];
 return val1 > val2 ? 1 : val1 < val2 ? -1 : 0;
}

//print_tree() ------------------------------------------------------------------------------------

void print_tree(knode * tree){
 printf("\n(%f, %f) - Axis = %d", tree->split[0], tree->split[1], tree->axis);
 if(tree->left != NULL){
  printf("\nLeft Branch:");
  print_tree(tree->left);
 }
 if(tree->right != NULL){
  printf("\nRight branch:");
  print_tree(tree->right);
 }
}
