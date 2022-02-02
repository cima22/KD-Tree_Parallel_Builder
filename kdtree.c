#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

typedef struct {
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
 
 for(int i = 0; i < N; i++){
 printf("%f - %f\n", points[i][0], points[i][1]);
 }
 
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
 
}

int choose_splitting_dimension(int axis, int ndim){ return (axis + 1) % ndim; }

kpoint* choose_splitting_point(kpoint* points, int n, int ndim, int axis){
 
 //sort points
 //
 //choose median [n/2] or [floor(n/2)]
 //
 //return median

 return NULL;
}
