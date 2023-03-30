#include <algorithm>
#include <stdio.h>
#include <math.h>
#if defined(_OPENMP)
#include <omp.h>
#endif
#include <chrono>
#include <cmath>
// g++ -O3 -std=c++11 jacobi2D-omp.cpp && ./a.out
// g++ -O3 -fopenmp -std=c++11 jacobi2D-omp.cpp && ./a.out

#include <iostream>
using namespace std;
const long MAXITER = 501;
void jacobi2d_seq(double *u, const double *f, long n)
{
  if (n == 0)
    return;
  double h2;
  h2 = 1 / (double)(n + 1) / (double)(n + 1);
  double *tempU = (double *)malloc(n * n * sizeof(double));
  double res;
  double res_single;

  for (int iter = 0; iter < MAXITER; ++iter)
  {

    // update tempU
    for (int i = 1; i < n - 1; ++i)
    {
      for (int j = 1; j < n - 1; ++j)
      {
        // double tmp = h2 * f[i * n + j] + u[(i - 1) * n + j] + u[i * n + j - 1] + u[(i + 1) * n + j] + u[i * n + j + 1];
        tempU[i * n + j] = 0.25 * (-h2 * f[i * n + j] + u[(i - 1) * n + j] + u[i * n + j - 1] + u[(i + 1) * n + j] + u[i * n + j + 1]);
        // if (i == 2 && j == 3)
        // {
        //   cout << i << " " << j << " " << tmp << " " << u[i * n + j] << " " << tempU[i * n + j] << endl;
        // }
        // cout << i << " " << j << " " << tmp << endl;
      }
    }

    // copy tempU to u
    for (int i = 1; i < n - 1; ++i)
    {
      for (int j = 1; j < n - 1; ++j)
      {
        u[i * n + j] = tempU[i * n + j];
      }
    }

    // compute residual
    res = 0;
    for (int i = 1; i < n - 1; ++i)
    {
      for (int j = 1; j < n - 1; ++j)
      {
        double tmp = (u[(i - 1) * n + j] + u[(i + 1) * n + j] + u[i * n + j - 1] + u[i * n + j + 1] - 4 * u[i * n + j]);
        res_single = (f[i * n + j] - tmp / h2);
        // if (i == 2 && j == 3)
        // {
        //   cout << i << " " << j << " " << res_single << " " << u[i * n + j] << " " << tempU[i * n + j] << endl;
        // }
        res += res_single * res_single;
      }
    }
    if (iter % 500 == 0)
    {
      cout << iter << " : " << sqrt(res) << endl;
    }
  }
  free(tempU);
}

void jacobi2d_omp(double *u, const double *f, long n)
{

  if (n == 0)
    return;
  double h2;
  h2 = 1 / (double)(n + 1) / (double)(n + 1);
  double *tempU = (double *)malloc(n * n * sizeof(double));
  double res;
  double res_single;

  for (int iter = 0; iter < MAXITER; ++iter)
  {

// update tempU
#pragma omp parallel for
    for (int i = 1; i < n - 1; ++i)
    {
      for (int j = 1; j < n - 1; ++j)
      {
        // double tmp = h2 * f[i * n + j] + u[(i - 1) * n + j] + u[i * n + j - 1] + u[(i + 1) * n + j] + u[i * n + j + 1];
        tempU[i * n + j] = 0.25 * (-h2 * f[i * n + j] + u[(i - 1) * n + j] + u[i * n + j - 1] + u[(i + 1) * n + j] + u[i * n + j + 1]);
        // if (i == 2 && j == 3)
        // {
        //   cout << i << " " << j << " " << tmp << " " << u[i * n + j] << " " << tempU[i * n + j] << endl;
        // }
        // cout << i << " " << j << " " << tmp << endl;
      }
    }

// copy tempU to u
#pragma omp parallel for
    for (int i = 1; i < n - 1; ++i)
    {
      for (int j = 1; j < n - 1; ++j)
      {
        u[i * n + j] = tempU[i * n + j];
      }
    }

    // compute residual
    res = 0;
#pragma omp parallel for reduction(+ \
                                   : res)
    for (int i = 1; i < n - 1; ++i)
    {
      for (int j = 1; j < n - 1; ++j)
      {
        double tmp = (u[(i - 1) * n + j] + u[(i + 1) * n + j] + u[i * n + j - 1] + u[i * n + j + 1] - 4 * u[i * n + j]);
        res_single = (f[i * n + j] - tmp / h2);
        // if (i == 2 && j == 3)
        // {
        //   cout << i << " " << j << " " << res_single << " " << u[i * n + j] << " " << tempU[i * n + j] << endl;
        // }
        res += res_single * res_single;
      }
    }
    if (iter % 500 == 0)
    {
      cout << iter << " : " << sqrt(res) << endl;
    }
  }
  free(tempU);
}

int main()
{
  long n = 10000;
  // long *A = (double *)malloc(n * n * sizeof(double));
  double *u1 = (double *)malloc(n * n * sizeof(double));
  double *u2 = (double *)malloc(n * n * sizeof(double));
  double *f = (double *)malloc(n * n * sizeof(double));
  // for (long i = 0; i < n * n; i++)
  //   A[i] = rand() / double(RAND_MAX);
  for (int i = 0; i < n * n; ++i)
    u1[i] = 0;
  for (int i = 0; i < n * n; ++i)
    u2[i] = u1[i];
  for (int i = 0; i < n * n; ++i)
    f[i] = 1;

#if defined(_OPENMP)
  omp_set_num_threads(8);
  double tt = omp_get_wtime();
  jacobi2d_omp(u2, f, n);
  printf("parallel-scan   = %fs\n", omp_get_wtime() - tt);
#else
  auto t_start = std::chrono::high_resolution_clock::now();
  jacobi2d_seq(u1, f, n);
  auto t_end = std::chrono::high_resolution_clock::now();
  cout << "time cost: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count() * 1e-9 << endl;
#endif

  // double err = 0;
  // for (long i = 0; i < n * n; i++)
  //   err = std::max(err, std::abs(u1[i] - u2[i]));
  // printf("error = %ld\n", err);

  free(u1);
  free(u2);
  free(f);
  return 0;
}
