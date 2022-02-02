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

typedef float_t kpoint[NDIM];

typedef struct {
	int axis;
	kpoint split;
	struct knode *left, *right;
} knode;

knode * build_kdtree(kpoint * points, int N, int ndim, int axis);

int choose_splitting_dimension(int axis);

kpoint* choose_splitting_point(kpoint * points, int N, int ndim, int axis);

int main(int argc, char* argv[]){

 

 return 0;
}
