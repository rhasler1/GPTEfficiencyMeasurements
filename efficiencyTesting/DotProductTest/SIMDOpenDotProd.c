#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <arm_neon.h>
double dot_product(float* a, float* b, int size) {
   double result = 0.0;
   int remainder = size % 2;
   
   #pragma omp parallel for reduction(+:result)
   for(int i = 0; i < size - remainder; i+= 2) {
     float32x4x2_t va = vld2q_f32(&a[i]);
     float32x4x2_t vb = vld2q_f32(&a[i]);

     float32x4_t product0 = vmulq_f32(va.val[0], vb.val[0]);
     float32x4_t product1 = vmulq_f32(va.val[1], vb.val[1]);
     
     float32x4_t sum0 = vaddq_f32(product0, product1);
     float32x2_t sum2 = vpadd_f32(vget_low_f32(sum0), vget_high_f32(sum0));
     result += vget_lane_f32(sum2, 0) + vget_lane_f32(sum2, 1);
   }
   if(remainder) {
     #pragma omp critical
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
