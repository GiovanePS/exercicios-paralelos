#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 8
#define TAM_NUM_SORTE 10000000

int numSorte;
pthread_cond_t condition;
pthread_mutex_t lock;

void *functionAuditora() {
  pthread_mutex_lock(&lock);
  numSorte = rand() % TAM_NUM_SORTE + 1;
  while (numSorte != 0) {
    pthread_cond_wait(&condition, &lock);
  }

  pthread_mutex_unlock(&lock);
  return NULL;
}

void *functionApostadora() {
  int contadorDeChutes = 0;
  int chute = rand() % 100 + 1;
  while (1) {
    contadorDeChutes++;
    if (chute == numSorte) {
      pthread_mutex_lock(&lock);
      numSorte = 0;
      printf("Achei o número da sorte!\n");
      printf("Eu sou a thread nº %lu\n", pthread_self());
      printf("Achei na tentativa nº %d\n", contadorDeChutes);
      printf( "O número da sorte é %d\n", chute);
      pthread_cond_signal(&condition);
      pthread_mutex_unlock(&lock);
      return NULL;
    }

    chute = rand() % TAM_NUM_SORTE + 1;

    if (numSorte == 0) {
      return NULL;
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
