#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>
void multiplyMatrices(float** first, float** second, float** result, int r1, int c1, int r2, int c2)
{
  #pragma omp parallel for
  for(int i = 0; i < r1; ++i) {
    for(int j = 0; j < c2; ++j) {
      float32x4_t sum = vdupq_n_f32(0.0f);
      for(int k = 0; k < c1; k += 4) {
        float32x4_t a = vld1q_f32(&first[i][k]);
        float32x4_t b = vld1q_f32(&second[k][j]);
        sum = vmlaq_f32(sum, a, b);
      }
      float32x2_t sum_low = vadd_f32(vget_low_f32(sum), vget_high_f32(sum));
      sum_low = vpadd_f32(sum_low, sum_low);
      result[i][j] = vget_lane_f32(sum_low,0);
    }
  }
    
}

int main(int argc, char* argv[]) {

  int r1, c1, r2, c2;
  int numMult = atoi(argv[1]);
  int numDim = atoi(argv[2]);
  float** first = (float**)malloc(numDim * sizeof(float*));
  float** second = (float**)malloc(numDim * sizeof(float*));
  float** result = (float**)malloc(numDim * sizeof(float*));
  for(int i = 0; i < numDim; i++)
  {
    first[i] = (float*)malloc(numDim * sizeof(float));
    second[i] = (float*)malloc(numDim * sizeof(float));
    result[i] = (float*)malloc(numDim * sizeof(float));
  }
 // printf("Num trials: %d\n", numMult);
  for(int i = 0; i < numDim; i++) {
    for(int j = 0; j < numDim; j++) {
      first[i][j] = i * j * 0.5;
      second[i][j] = (i + j) * 0.5;
      result[i][j] = 0;
    }
  }
  //printf("Finsihed filling matrices\n");
  for(int i = 0; i < numMult; i++)
  {
    multiplyMatrices(first,second, result, numDim, numDim, numDim, numDim);
  }
  printf("Result[5][5]: %lf\n", result[5][5]);
  free(first);
  free(second);
  free(result);
}
  
