#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
double dot_product(float* a, float* b, int size) {
  double result = 0.0;
  #pragma omp parallel for reduction(+:result)
  for(int i = 0; i < size; i++)
  {
    result += a[i] * b[i];
  }
 // printf("Partial Result: %lf\n", result);
  return(result);
}


int main(int argc, char* argv[]) {
  int size = atoi(argv[1]);
  int numDots = atoi(argv[2]);
  size = (size + 3) / 4 *4;
  double finalResult = 0.0;
  float* a = (float*)malloc(size * sizeof(double));
  float* b = (float*)malloc(size * sizeof(double));
  for(int i = 0; i < size; i++) {
    a[i] = (double)i;
    b[i] = (double)(size - i);
  }
/*
  printf("a: [");
  for(int i = 0; i < size; i++)
  {
    printf("%lf, ", a[i]);
  }
  printf("]\n");
  printf("b: [");
  for(int i = 0; i < size; i++)
  {
    printf("%lf, ", b[i]);
  }
  printf("]\n");
*/
  for(int i = 0; i < numDots; i++)
  {
    finalResult+= dot_product(a,b,size);
  }
  printf("Result: %lf\n", finalResult);
  free(a);
  free(b);
  
  return(0);
}
