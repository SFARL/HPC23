#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#include <iostream>
using namespace std;
// $ g++ -O3 -fopenmp omp-scan.cpp && ./a.out
// $ export OMP NUM THREADS=8

// Scan A array and write result into prefix_sum array;
// use long data type to avoid overflow
void scan_seq(long *prefix_sum, const long *A, long n)
{
  if (n == 0)
    return;
  prefix_sum[0] = 0;
  for (long i = 1; i < n; i++)
  {
    prefix_sum[i] = prefix_sum[i - 1] + A[i - 1];
  }
  // for (long i = 0; i < n; i++)
  // {
  //   cout << prefix_sum[i] << " ";
  // }
}

void scan_omp(long *prefix_sum, const long *A, long n)
{
  int p = 32;
  long *part_sum = (long *)malloc((p + 1) * sizeof(long));
  for (long i = 0; i <= p; i++)
    part_sum[i] = 0;
  int chunksize = n / p;
  // cout << "chunksize is " << chunksize << " and p is " << p << endl;
  // cout << p << " " << t << endl;
  // Fill out parallel scan: One way to do this is array into p chunks
  // Do a scan in parallel on each chunk, then share/compute the offset
  // through a shared vector and update each chunk by adding the offset
  // in parallel

#pragma omp parallel num_threads(p)
  {
    int t = omp_get_thread_num();
    // cout << "Now is thread " << t << endl;
    long chunck_sum = 0;
    int start = t * chunksize;
    int end = (t + 1) * chunksize;
    for (int i = start; i < end; ++i)
    {
      if (i == 0)
      {
        continue;
      }
      chunck_sum += A[i - 1];
      prefix_sum[i] = chunck_sum;
    }
    part_sum[t + 1] = chunck_sum;
  }
  long pre = 0;
  for (int i = p * chunksize; i < n; ++i)
  {
    pre += A[i - 1];
    prefix_sum[i] = pre;
  }

  for (int i = 1; i <= p; ++i)
  {
    part_sum[i] += part_sum[i - 1];
    // cout << part_sum[i - 1] << " ";
  }

#pragma omp parallel for num_threads(p)
  for (int i = 0; i < n; ++i)
  {
    int idx = i / chunksize;
    prefix_sum[i] += part_sum[idx];
  }

  // cout << endl;
  // for (long i = 0; i < n; i++)
  // {
  //   cout << prefix_sum[i] << " ";
  // }
  // cout << endl;
}

int main()
{
  long N = 100000000;
  long *A = (long *)malloc(N * sizeof(long));
  long *B0 = (long *)malloc(N * sizeof(long));
  long *B1 = (long *)malloc(N * sizeof(long));
  for (long i = 0; i < N; i++)
    A[i] = rand();
  // cout << endl;
  for (long i = 0; i < N; i++)
    B1[i] = 0;

  double tt = omp_get_wtime();
  scan_seq(B0, A, N);
  printf("sequential-scan = %fs\n", omp_get_wtime() - tt);

  tt = omp_get_wtime();
  scan_omp(B1, A, N);
  printf("parallel-scan   = %fs\n", omp_get_wtime() - tt);

  long err = 0;
  for (long i = 0; i < N; i++)
    err = std::max(err, std::abs(B0[i] - B1[i]));
  printf("error = %ld\n", err);

  free(A);
  free(B0);
  free(B1);
  return 0;
}
