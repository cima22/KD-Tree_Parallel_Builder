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
#define N 10

//-------------------- Data Structures --------------------------------------------------------

typedef float_t kpoint[NDIM];

typedef struct knode{
	int axis;
	kpoint split;
	struct knode *left, *right;
} knode;

//--------------------- Functions Declaration -------------------------------------------------

knode * build_kdtree(kpoint * points, int n, int ndim, int axis);

int choose_splitting_dimension(int axis, int ndim);

kpoint* choose_splitting_point(kpoint * points, int n, int ndim, int axis);

kpoint * initialize();

//---------------------- Main -----------------------------------------------------------------

int main(int argc, char* argv[]){

 kpoint * points = initialize();
 
 //knode * kd_tree = build_kdtree(points, 1, N, DIM, -1);
 printf("points[0] = %p, points[0][0] = %f, points[0][1] = %f", points[0],  points[0][0], points[0][1]);
 //printf("Axis = %d, Split = (%f,%f)\n", kd_tree[0].axis, kd_tree[0].split[0], kd_tree[0].split[1]);

 return 0;
}

//----------------------- Functions -----------------------------------------------------------

kpoint * initialize(){
 
 kpoint * points = (kpoint *) malloc(N * sizeof(kpoint));

 if(points == NULL){
  printf("Problem with malloc()");
  return NULL;
 }

 srand48((int) getpid());

 for(int i = 0; i < N; i++){
  points[i][0] = drand48();
  points[i][1] = drand48();
 }

 return points;
}

knode * build_kdtree(kpoint * points, int n, int ndim, int axis){
 
 if(n == 1){
  knode * leaf = (knode *) malloc(sizeof(knode));
  leaf -> axis = axis;
  memcpy(leaf->split, points, sizeof(kpoint *));
  leaf -> left = NULL;
  leaf -> right = NULL;
  return leaf;
 }

 knode * node = (knode *) malloc(sizeof(knode));
 if(node == NULL){
  printf("Problem with malloc()");
  return NULL;
 }

 int my_axis = choose_splitting_dimension(axis, ndim);
 kpoint * my_point = choose_splitting_point(points, n, ndim, axis);

 kpoint * left_points, *right_points;

 int N_left, N_right;
 int median_index = (int) (n/2)
 N_left = median_index; // #points before median
 N_right = ndim % 2 == 0 ? median_index - 1 : median_index; // points after median 

 kpoint * left_points, * right_points;
 left_points = (kpoint *) points;
 right_points = (kpoint *) (points) + N_left + 1;

 node -> axis = my_axis;
 memcpy(node -> split, my_point, sizeof(my_point));

 node -> left = build_kdtree(left_points, N_left,:w ndim, my_axis);
 node -> right = build_kdtree(right_points, N_right, ndim, my_axis);
 
 return node;
}

int choose_splitting_dimension(int axis, int ndim){ return (axis + 1) % ndim; }

kpoint* choose_splitting_point(kpoint* points, int n, int ndim, int axis){
 
 //sort points
  
 kpoint * median = (kpoint*) points[(int) floor(n/2)]; // median of the sorted points

 return median;

 return NULL;
}
