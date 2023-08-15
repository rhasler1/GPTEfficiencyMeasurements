#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>
double dot_product(float* a, float* b, int size) {
  float result_array[4];
  double result = 0.0;
  
  #pragma omp parallel
  {  
    float32x4_t a_vec, b_vec, result_vec = vdupq_n_f32(0.0f);

    #pragma omp for
      for(int i = 0; i < size; i+= 4)
      {
        a_vec = vld1q_f32(a + i);
        b_vec = vld1q_f32(b + i);
        result_vec = vmlaq_f32(result_vec, a_vec, b_vec);
      }
    vst1q_f32(result_array, result_vec);

    #pragma omp atomic 
    for(int i = 0; i < 4; i++) 
    {
      result += result_array[i];
    }
  }
  
  return(result);
}


int main(int argc, char* argv[]) {
  int size = atoi(argv[1]);
  int numDots = atoi(argv[2]);
  double finalResult = 0.0;
  size = (size + 3) / 4 * 4;
  float* a = (float*)malloc(size * sizeof(double));
  float* b = (float*)malloc(size * sizeof(double));
  for(int i = 0; i < size; i++) {
    a[i] = (double)i;
    b[i] = (double)(size - i);
  }
  for(int i = 0; i < numDots; i++)
  {
    //double temp = dot_product(a,b,size);
    finalResult+= dot_product(a,b,size);
    //printf("Partial Result: %lf\n", temp);
  }
  printf("Result: %lf\n", finalResult);
  free(a);
  free(b);
  
  return(0);
}
