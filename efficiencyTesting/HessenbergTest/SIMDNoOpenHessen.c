#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>
#define RADIX 2.0
#define N 1000
#define TRIALS 10
#define SWAP(a,b) do { float t = (a); (a) = (b); (b) = t; } while(0)
float hessen(float mat[N][N], int n)
{

  int i, j, m;
  float y, x;
  float a[N][N];

  for(i = 0; i < N; i++)
  {
    for(j = 0; j < N; j++)
    {
      a[i][j] = mat[i][j];
    }
  }
  
  for (m = 1; m < n; m++)
  {
  
    float32x4_t vmax = vdupq_n_f32(0.0f);
    int imax = m;
    
    for(j = m; j < n; j += 4)
    {
      float32x4_t col = vld1q_f32(&a[j][m - 1]);
      vmax = vmaxq_f32(vmax, vabsq_f32(col));
    }
    
    float32x2_t max2 = vpmax_f32(vget_low_f32(vmax), vget_high_f32(vmax));
    max2 = vpmax_f32(max2, max2);
    float max_value = vget_lane_f32(max2, 0);
    
    for(j = m; j < n; j++)
    {
      if(fabsf(a[j][m - 1]) == max_value)
      {
        imax = j;
        break;
      }
    }
    
    if(imax != m)
    {
      for(j = m - 1; j < n; j++)
        SWAP(a[imax][j], a[m][j]);
      for(j = 0; j < n; j++)
        SWAP(a[j][imax], a[j][m]);
    }
    
    float x = a[m][m - 1];
    if(x != 0.0)
    {
      for(i = m + 1; i < n; i++)
      {
        float y = a[i][m - 1];
        if(y != 0.0)
        {
          y /= x;
          a[i][m - 1] = y;
          
          for(j = m; j < n; j += 4)
          {
            float32x4_t a_mj = vld1q_f32(&a[m][j]);
            float32x4_t a_ij = vld1q_f32(&a[i][j]);
            float32x4_t sub_result = vmlaq_f32(a_ij, vdupq_n_f32(-y), a_mj);
            vst1q_f32(&a[i][j], sub_result);
          }
          
          for(j = 0; j < n; j += 4)
          {
            float32x4_t a_ji = vld1q_f32(&a[j][i]);
            float32x4_t a_jm = vld1q_f32(&a[j][m]);
            float32x4_t add_result = vmlaq_f32(a_ji, vdupq_n_f32(y), a_jm);
            vst1q_f32(&a[j][i], add_result);
          }
        }
      }
    }
  }
  return(a[N/2][N/2]);
} 




int main(int argc, char* argv)
{
  float mat[N][N];
  double total = 0;
  srand(10);
  for(int i = 0; i < N; i++)
  {
    for(int j = 0; j < N; j++)
    {
      mat[i][j] = ((rand() % 41) - 20) * 1.5;
    }
  }
  for(int i = 0; i < TRIALS; i++)
  {
    total += hessen(mat, N);
  }

  printf("Total: %lf\n", total);
  return(0);

}
