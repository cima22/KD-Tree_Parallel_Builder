#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N 9

typedef float kpoint[2];

int comp_x(const void * el1, const void * el2){

 float val1 = *((kpoint *) el1)[0];
 float val2 = *((kpoint *) el2)[0];
 return val1 > val2 ? 1 : val1 < val2 ? -1 : 0;
 
}

void provetta(int n){

printf("pointer inside fun: %p - value 10: %d\n", &n, n);
}

int main(){

 kpoint * points = (kpoint *) malloc(N * sizeof(kpoint));

 srand48((int) getpid());

 for(int i = 0; i < N; i++){
  points[i][0] = drand48();
  points[i][1] = drand48();
 }

int n = 10;
printf("pointer outside fun: %p - value 10: %d\n", &n, n);
printf("\n\n%d\n\n", time(NULL));
provetta(n);

/*
 float * nums = (float *) points;
 for(int i = 0; i < N * 2; i++)
  printf("%d - %f\n", i, nums[i]);


 for(int i = 0; i < N; i++){
  printf("%d - (%f, %f)\n", i, points[i][0], points[i][1]);
 }

 qsort(points, N, sizeof(kpoint*), comp_x);

 int median_index = (int) N/2;
 
 kpoint * left_points, * right_points;

 left_points = (kpoint *) points;
 right_points = (kpoint * ) points + median_index + 1;

 printf("\n");

 for(int i = 0; i < median_index; i++)
  printf("%d - (%f, %f)\n", i, left_points[i][0], left_points[i][1]);

 printf("\nm - (%f, %f)\n", points[median_index][0], points[median_index][1]);

 printf("\n");

 for(int i = 0; i < median_index; i++)
  printf("%d - (%f, %f)\n", i, right_points[i][0], right_points[i][1]);
*/
return 0;
}
