/************************************************************************
 * Autor: N Rojas
 * Fecha: Noviembre 2023
 * Computación de Alto Rendimiento
 * Maestría en Inteligencia Artificial
 * Tema: Programa de Multiplicación de Matrices usando hilos OpenMP
 * -Algorimo filasXfilas
 *************************************************************************/

#include "sample.h"
#include <omp.h>
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
    a[j + k * SZ] = 2.0 * (j + k);
    b[j + k * SZ] = 3.2 * (j - k);
    c[j + k * SZ] = 0.0;
  }
}

int main(int argc, char **argv) {
  int N;

  if (argc < 2) {
    printf("MM1r MatrixSize [Sample arguments ...]\n");
    return -1;
  }

  N = (int)atof(argv[1]);
  argc--;
  argv++;

  if (N > 1024 * 10) {
    printf("Unvalid MatrixSize\n");
    return -1;
  }

  Sample_Init(argc, argv);

#pragma omp parallel
  {
    int NTHR, THR, SZ;
    int i, j, k;
    double *a, *b, *c;

    SZ = N;
    THR = Sample_PAR_install();
    NTHR = omp_get_num_threads();

    a = MEM_CHUNK;
    b = a + SZ * SZ;
    c = b + SZ * SZ;

#pragma omp master
    Matrix_Init_col(SZ, a, b, c);

    Sample_Start(THR);

#pragma omp for
    for (i = 0; i < SZ; ++i) {
      double *pA, *pB, S;
      pA = a + (i * SZ);
      for (j = 0; j < SZ; ++j) {
        pB = b + (j * SZ);
        for (k = 0; k < SZ; ++k, ++pB) {
          S = *pA * *pB;
          c[i * SZ + k] += S;
        }
        ++pA;
      }
    }

    Sample_Stop(THR);
  }

  Sample_End(&N);
}
