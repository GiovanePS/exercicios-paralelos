#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <threads.h>
#include <time.h>

// #define IMPRIME

#define MAX_NUMBERS 500000000
// #define MAX_NUMBERS 10
#define MAX_VALUE 1000
#define NUM_THREADS 4

float numbers[MAX_NUMBERS];
unsigned int i;

typedef struct {
  int threadId;
} threadArgs;

int init_numbers() {
  for (i = 0; i < MAX_NUMBERS; i++)
    numbers[i] = ((float)rand() / (float)RAND_MAX) * MAX_VALUE;
  return 0;
}

int show_numbers() {
  for (i = 0; i < MAX_NUMBERS; i++)
    printf("number[%u] = %f\n", i, numbers[i]);
  return 0;
}

void *calcNumbers(void *args) {
  threadArgs *tArgs = (threadArgs *)args;
  int a = tArgs->threadId * MAX_NUMBERS / NUM_THREADS;
  int end = (tArgs->threadId + 1) * MAX_NUMBERS / NUM_THREADS;
  for (i = a; i < end; i++) {
    numbers[i] = numbers[i] * 0.2 + numbers[i] / 0.3;
  }

  return NULL;
}

int main(int argc, char **argv) {
  struct timeval t1, t2;
  pthread_t threads[NUM_THREADS];

  srand(time(NULL));

  gettimeofday(&t1, NULL);

  init_numbers();

#ifdef IMPRIME
  show_numbers();
#endif
  //
  // for (i = 0; i < MAX_NUMBERS; i++) {
  //   numbers[i] = numbers[i] * 0.2 + numbers[i] / 0.3;
  // }

#ifdef IMPRIME
  printf("Após a operacao matemática\n");
  show_numbers();
#endif

  // Threaded way
  for (int j = 0; j < NUM_THREADS; j++) {
    threadArgs *args = (threadArgs *)malloc(sizeof(threadArgs));
    pthread_create(&threads[j], NULL, calcNumbers, (void *)args);
  }

  for (int j = 0; j < NUM_THREADS; j++) {
    pthread_join(threads[j], NULL);
  }

  gettimeofday(&t2, NULL);
  double t_total =
      (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);
  printf("Total time: %f\n", t_total);
  return 0;
}
