#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3

typedef struct {
  int contador;
  pthread_mutex_t mutex;
  pthread_cond_t condition;
} barreira;

void inicializa_barreira(barreira *b, int cont) {
  b->contador = cont;
  pthread_mutex_init(&b->mutex, NULL);
  pthread_cond_init(&b->condition, NULL);
}

void wait(barreira *b) {
  pthread_mutex_lock(&b->mutex);
  b->contador--;
  if (b->contador > 0) {
    while (b->contador > 0) {
      pthread_cond_wait(&b->condition, &b->mutex);
    }
    pthread_mutex_unlock(&b->mutex);
  } else {
    pthread_cond_broadcast(&b->condition);
    pthread_mutex_unlock(&b->mutex);
  }
}

void *functionThread(void *args) {
  struct {
    int seconds;
    barreira *b;
  } *tArgs = args;
  sleep(tArgs->seconds);
  printf("Thread executando antes da barreira!\n");
  wait(tArgs->b);
  printf("Thread executando depois da barreira!\n");
  return NULL;
}

int main(void) {
  pthread_t threads[NUM_THREADS];
  barreira barrier;
  inicializa_barreira(&barrier, NUM_THREADS);


  for (int i = 0; i < NUM_THREADS; i++) {
    struct {
      int seconds;
      barreira *b;
    } *tArgs = malloc(sizeof(*tArgs));

    tArgs->seconds = i;
    tArgs->b = &barrier;

    pthread_create(&threads[i], NULL, functionThread, tArgs);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&barrier.mutex);
  pthread_cond_destroy(&barrier.condition);
  return 0;
}
