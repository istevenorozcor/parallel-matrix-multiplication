/************************************************************************
 * Autor: N Rojas
 * Fecha: Noviembre 2023
 * Computación de Alto Rendimiento
 * Maestría en Inteligencia Artificial
 * Tema: Programa de Multiplicación de Matrices usando MPI
 * -Algorimo Clásico filasXcolumnas
 *************************************************************************/

#include "sample.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#define DATA_SZ (1024 * 1024 * 64 * 3)

static double MEM_CHUNK[DATA_SZ];

void Matrix_Init_col(int SZ, double *a, double *b, double *c) {
  int j, k;
  for (j = 0; j < SZ; j++) {
    for (k = 0; k < SZ; k++) {
      a[j + k * SZ] = 2.0 * (j + k);
      b[j + k * SZ] = 3.2 * (j - k);
      c[j + k * SZ] = 0.0;
    }
  }
}

int main(int argc, char **argv) {
  int N, rank, size;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (argc < 2) {
    if (rank == 0) {
      printf("MPI_MM1c MatrixSize [Sample arguments ...]\n");
    }
    MPI_Finalize();
    return -1;
  }

  N = (int)atof(argv[1]);
  argc--;
  argv++;

  if (N > 1024 * 10) {
    if (rank == 0) {
      printf("Invalid MatrixSize\n");
    }
    MPI_Finalize();
    return -1;
  }

  Sample_Init(argc, argv);

  int SZ = N / size;

  double *a = MEM_CHUNK;
  double *b = a + SZ * N;
  double *c = b + SZ * N;

  if (rank == 0) {
    Matrix_Init_col(N, a, b, c);
  }

  Sample_Start(rank);

  MPI_Bcast(a, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(b, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  for (int i = rank * SZ; i < (rank + 1) * SZ; ++i) {
    double *pA, *pB, S;
    pA = a + (i * N);
    for (int j = 0; j < N; ++j) {
      pB = b + (j * N);
      for (int k = 0; k < N; ++k, ++pB) {
        S = *pA * *pB;
        c[i * N + k] += S;
      }
      ++pA;
    }
  }

  Sample_Stop(rank);

  MPI_Gather(c + rank * SZ * N, SZ * N, MPI_DOUBLE, c, SZ * N, MPI_DOUBLE, 0,
             MPI_COMM_WORLD);

  if (rank == 0) {
    Sample_End(&N);
  }

  MPI_Finalize();

  return 0;
}
