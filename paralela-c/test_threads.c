#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define TAM 90000000
#define numThreads 4
#define a 4

void calcUntilX(unsigned int *x, unsigned int *y);
void *calcUntilXThreaded(void *args);

typedef struct {
  unsigned int *x;
  unsigned int *y;
  int threadId;
} threadArgs;

int main(void) {
  unsigned int *x = (unsigned int *)malloc(sizeof(unsigned int) * TAM);
  unsigned int *y = (unsigned int *)malloc(sizeof(unsigned int) * TAM);
  pthread_t threads[numThreads];
  threadArgs *args[numThreads]; // Separate args for each thread

  for (int i = 0; i < TAM; i++) {
    x[i] = i;
    y[i] = i;
  }

  // Non-threaded calculation
  struct timeval start, end;
  gettimeofday(&start, NULL);
  calcUntilX(x, y);
  gettimeofday(&end, NULL);
  double seconds =
      (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000000.0);
  printf("Non-threaded: %f seconds\n", seconds);

  // Threaded calculation
  gettimeofday(&start, NULL);
  for (int i = 0; i < numThreads; i++) {
    args[i] = (threadArgs *)malloc(
        sizeof(threadArgs)); // Allocate memory for each thread's args
    args[i]->x = x;
    args[i]->y = y;
    args[i]->threadId = i; // Assign unique thread ID

    pthread_create(&threads[i], NULL, calcUntilXThreaded, (void *)args[i]);
  }

  for (int i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
    free(args[i]); // Free memory after thread is done
  }

  gettimeofday(&end, NULL);
  seconds =
      (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000000.0);
  printf("Threaded: %lf seconds\n", seconds);

  return 0;
}

void calcUntilX(unsigned int *x, unsigned int *y) {
  for (int i = 0; i < TAM; i++) {
    y[i] = a * x[i] + y[i];
  }
}

void *calcUntilXThreaded(void *args) {
  threadArgs *localArgs = (threadArgs *)args;
  int threadId = localArgs->threadId;
  unsigned int *y = localArgs->y;
  unsigned int *x = localArgs->x;

  int startIdx = threadId * (TAM / numThreads);
  int endIdx = startIdx + (TAM / numThreads);

  for (int i = startIdx; i < endIdx; i++) {
    y[i] = a * x[i] + y[i];
  }

  return NULL;
}
