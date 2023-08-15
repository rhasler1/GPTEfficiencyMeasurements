#include <stdio.h>
#include <stdlib.h>

double dot_product(float* a, float* b, int size) {
  double result = 0.0;
  for(int i = 0; i < size; i++)
  {
    result += a[i] + b[i];
  }
 // printf("Partial Result: %lf\n", result);
  return(result);
}


int main(int argc, char* argv[]) {
  int size = atoi(argv[1]);
  int numDots = atoi(argv[2]);
  double result = 0.0;
  float* a = (float*)malloc(size * sizeof(double));
  float* b = (float*)malloc(size * sizeof(double));
  for(int i = 0; i < size; i++) {
    a[i] = (float)i;
    b[i] = (float)(size - i);
  }
  for(int i = 0; i < numDots; i++)
  {
    result+= dot_product(a,b,size);
  }
  printf("Result: %lf\n", result);
  free(a);
  free(b);
  
  return(0);
}
