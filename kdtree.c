#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#if !defined(DOUBLE_PRECISION)
typedef float float_t;
#else
typedef double float_t;
#endif
#define NDIM 2

//-------------------- Data Structures --------------------------------------------------------

typedef float_t kpoint[NDIM];

typedef struct {
	int axis;
	kpoint split;
	struct knode *left, *right;
} knode;

//--------------------- Functions Declaration -------------------------------------------------

knode * build_kdtree(kpoint * points, int N, int ndim, int axis);

int choose_splitting_dimension(int axis, int ndim);

kpoint* choose_splitting_point(kpoint * points, int N, int ndim, int axis);

//---------------------- Main -----------------------------------------------------------------

int main(int argc, char* argv[]){

 printf("Hemlo\n"); 

 return 0;
}

//----------------------- Functions -----------------------------------------------------------

int choose_splitting_dimension(int axis, int ndim){ return (axis + 1) % ndim; }

kpoint* choose_splitting_point(kpoint* points, int N, int ndim, int axis){
 return NULL;
}
