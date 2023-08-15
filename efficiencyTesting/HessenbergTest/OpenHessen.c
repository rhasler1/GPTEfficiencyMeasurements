#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
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
  
  #pragma omp parallel for private(x, y, i, j) shared(a)
  for(m = 1; m < n - 1; m++)
  {
    x = 0.0;
    i = m;
    for(j = m; j < n; j++)
    {
      if(fabs(a[j][m - 1]) > fabs(x))
      {
        x = a[j][m - 1];
        i = j;
      }
    }
    if(i != m) 
    {
    #pragma omp critical
    {
      for(j = m - 1; j < n; j++)
        SWAP(a[i][j], a[m][j]);
      for(j = 0; j < n; j++)
        SWAP(a[j][i], a[j][m]);
    }
    }
    if(x != 0.0)
    {
      #pragma omp parallel for private(y, j) shared(a)
      for(i = m + 1; i < n; i++)
      {
        if((y = a[i][m - 1]) != 0.0)
        {
          y /= x;
          a[i][m - 1] = y;
          for (j = m; j < n; j++)
            a[i][j] -= y * a[m][j];
          for(j = 0; j < n; j++)
            a[j][m] += y * a[j][i];
        }
      }
    }
  }
  return(a[n/2][n/2]);
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
