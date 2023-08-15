#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>
double dot_product(float* a, float* b, int size) {
  double result = 0.0;
  int remainder = size % 2;
  
  for(int i = 0; i < size - remainder; i += 2) 
  {
    float32x2_t va = vld1_f32(&a[i]);
    float32x2_t vb = vld1_f32(&b[i]);
    float32x2_t product = vmul_f32(va,vb);
    float32x2_t sum = vpadd_f32(product, product);
    result += vget_lane_f32(sum,0);
  }
  if(remainder) {
    result += a[size - 1] * b[size - 1];
  }
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
