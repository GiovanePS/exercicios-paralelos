#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 8

int numSorte;
pthread_cond_t condition;
pthread_mutex_t lock;

void *functionAuditora() {
  pthread_mutex_lock(&lock);
  numSorte = rand() % 100 + 1;
  pthread_cond_wait(&condition, &lock);
  pthread_mutex_unlock(&lock);
  return NULL;
}

void *functionApostadora() {
  int chute = rand() % 100 + 1;
  while (1) {
    if (chute == numSorte) {
      printf("Achei o número da sorte!\n");
      pthread_cond_signal(&condition);
      return NULL;
    }

    chute++;
    if (chute > 100) {
      chute = rand() % 100 + 1;
    }
  }
}

int main(void) {

  srand(time(NULL));
  pthread_t threadAuditora;
  pthread_t threadsApostadoras[NUM_THREADS];
  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&condition, NULL);

  pthread_create(&threadAuditora, NULL, functionAuditora, NULL);

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threadsApostadoras[i], NULL, functionApostadora, NULL);
  }

  pthread_join(threadAuditora, NULL);

  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&condition);

  return 0;
}
