#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>

#if !defined(DOUBLE_PRECISION)
typedef float float_t;
#else
typedef double float_t;
#endif
#define NDIM 2

//-------------------- Data Structures --------------------------------------------------------

typedef float_t kpoint[NDIM];

typedef struct knode{
	int axis;
	kpoint split;
	struct knode *left, *right;
} knode;

//--------------------- Functions Declaration -------------------------------------------------

knode * build_kdtree(kpoint * points, int n, int ndim);

knode * build_kdtree_ric(kpoint * points, int n, int ndim, int axis);

int choose_splitting_dimension(int axis, int ndim);

kpoint* choose_splitting_point(kpoint * points, int n, int ndim, int axis);

kpoint * initialize(int n);

void my_qsort(kpoint * points, int n, int el_len, int axis);

int comp_x(const void * el1, const void * el2);

int comp_y(const void * el1, const void * el2);

void print_tree(knode * tree);

//---------------------- Main -----------------------------------------------------------------

int main(int argc, char* argv[]){

 int N = 100000000;
 int threads = 1;
 double start, end; 

 if(argc == 2)
  sscanf(argv[1], "%d", &N);

 kpoint * points = initialize(N);

 start = omp_get_wtime();
 knode * kd_tree = build_kdtree(points, N, NDIM);
 end = omp_get_wtime() - start;
 //print_tree(kd_tree);
 
 #pragma omp parallel
 #pragma omp master
 threads = omp_get_num_threads();
 
 printf("%f,%d,%d\n", end, N, threads);
 
 return 0;
}

//----------------------- Functions -----------------------------------------------------------

// initialize() ----------------------------------------------------------------

kpoint * initialize(int n){
 
 kpoint * points = (kpoint *) malloc(n * sizeof(kpoint));

 if(points == NULL){
  printf("Problem with malloc()");
  return NULL;
 }

 srand48((int) getpid());

 for(int i = 0; i < n; i++){
  points[i][0] = drand48();
  points[i][1] = drand48();
 }

 return points;
}

// build_kdtree() -------------------------------------------------------------

knode * build_kdtree(kpoint * points, int n, int ndim){
 knode * tree;
 #pragma omp parallel
 #pragma omp master
 tree = build_kdtree_ric(points, n, ndim, -1);
 return tree;
}

knode * build_kdtree_ric(kpoint * points, int n, int ndim, int axis){

 if(n == 0){ return NULL;}

 if(n == 1){
  knode * leaf = (knode *) malloc(sizeof(knode));
  leaf -> axis = axis;
  memcpy(leaf->split, points, sizeof(kpoint *));
  leaf -> left = NULL;
  leaf -> right = NULL;
 // printf("\n\n(%f, %f)\n\n", points[0][0], points[0][1]);
  return leaf;
 }

 kpoint * temp = (kpoint *) malloc(n * sizeof(kpoint));
 memcpy(temp, points, n * sizeof(kpoint));

 knode * node = (knode *) malloc(sizeof(knode));
  if(node == NULL){
  printf("Problem with malloc()");
  return NULL;
 }

 int my_axis = choose_splitting_dimension(axis, ndim);

 kpoint * my_point = choose_splitting_point(temp, n, ndim, my_axis);

//  printf("\n\n%d\n", n);
//  for(int i = 0; i < n; i++)
//   printf("(%f, %f)\n", points[i][0], points[i][1]);
//  printf("\n\n");

 int N_left, N_right;
 int median_index = (int) (n/2);

 N_left = median_index; // #points before median
 N_right = n % 2 == 0 ? median_index - 1 : median_index; // points after median 

 kpoint * left_points, * right_points;
 left_points  = (kpoint *) temp;
 right_points = (kpoint *) (temp) + N_left + 1;

 node -> axis = my_axis;
 memcpy(node -> split, my_point, sizeof(kpoint *));

// printf("\n%d\n", omp_get_num_threads());

 #pragma omp task
 node -> left  = build_kdtree_ric(left_points, N_left, ndim, my_axis);
 #pragma omp task
 node -> right = build_kdtree_ric(right_points, N_right, ndim, my_axis);

 //free(temp);

//#pragma omp taskwait
 return node;
}

// choose_splitting_dimension() --------------------------------------------------------

int choose_splitting_dimension(int axis, int ndim){ return (axis + 1) % ndim; }

// choose_splitting_point() -----------------------------------------------------------

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