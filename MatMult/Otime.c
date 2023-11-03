#include <errno.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MAX_THREADS 20

struct timeval start[MAX_THREADS];
struct timeval stop[MAX_THREADS];

static int N_THREADS;

void Sample_Start(int THR) {
#pragma omp barrier
  gettimeofday(start + THR, (void *)0);
}

void Sample_Stop(int THR) { gettimeofday(&(stop[THR]), (void *)0); }

void Sample_Init(int argc, char *argv[]) {

  if (argc < 3) {
    printf("Sample parameters: NumberThreads \n");
    exit(1);
  }

  N_THREADS = (int)atof(argv[1]);

  if (!N_THREADS || N_THREADS > MAX_THREADS) {
    printf("Number of Threads is not valid\n");
    exit(1);
  }

  omp_set_num_threads(N_THREADS);
}

int Sample_PAR_install() {
  int THR;

  THR = omp_get_thread_num();

  return THR;
}

void Sample_End(const int *SZ) {
  int THR, i;

  for (THR = 0; THR < N_THREADS; THR++) {
    printf("%1.0f,", (double)*SZ);
    printf("%1.0f,", (double)N_THREADS);
    printf("%1.0f,", (double)THR);
    stop[THR].tv_usec -= start[THR].tv_usec;
    if (stop[THR].tv_usec < 0) {
      stop[THR].tv_usec += 1000000;
      stop[THR].tv_sec--;
    }
    stop[THR].tv_sec -= start[THR].tv_sec;

    printf("%1.0f\n", (double)(stop[THR].tv_sec * 1000000 + stop[THR].tv_usec));
  }
}
