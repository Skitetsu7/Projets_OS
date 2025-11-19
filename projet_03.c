#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_THREADS 2
typedef unsigned int index;
typedef unsigned int length;

void merge(int array[], index start, index middle, index final);
void merge_sort(int array[], index start, index final);

struct argument {
  int start, final;
  int *array;
};

void show(int array[], length count) {
  for (index i = 0; i < count; i++)
    printf("%d ", array[i]);
  printf("\n");
}

void *merge_sort_threads(void *args) {

  struct argument *arg = (void *)args;
  pthread_t t1, t2;
  int *ar = arg->array;
  int start = arg->start;
  int final = arg->final;
  if (start >= final) {
    free(arg);
    pthread_exit(NULL);
  }

  index middle = floor((start + final) / 2);







  int sizes[]= {};




  printf("start : %d , middle : %d , final : %d\n", start, middle, final);

  struct argument *first = malloc(sizeof(struct argument));
  first->start = start;
  first->final = middle;
  first->array = ar;

  struct argument *rest = malloc(sizeof(struct argument));
  rest->start = middle + 1;
  rest->final = final;
  rest->array = ar;

  pthread_create(&t1, NULL, merge_sort_threads, first);
  pthread_create(&t2, NULL, merge_sort_threads, rest);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  merge(ar, start, middle, final);
  free(arg);
  return NULL;
}

void merge_sort(int array[], index start, index final) {
  if (start < final) {
    index middle = floor((start + final) / 2);
    merge_sort(array, start, middle);
    merge_sort(array, middle + 1, final);
    merge(array, start, middle, final);
  }
}

void merge(int array[], index start, index middle, index final) {
  length countL = middle - start + 1;
  int *arrayL = malloc(countL * sizeof(int));
  index currentL, currentR;
  for (currentL = 0; currentL < countL; currentL++)
    arrayL[currentL] = array[start + currentL];

  length countR = final - middle;
  int *arrayR = malloc(countR * sizeof(int));
  for (currentR = 0; currentR < countR; currentR++)
    arrayR[currentR] = array[middle + 1 + currentR];

  currentL = 0;
  currentR = 0;
  index current;
  for (current = start;
       current <= final && currentL < countL && currentR < countR; current++) {
    if (arrayL[currentL] <= arrayR[currentR]) {
      array[current] = arrayL[currentL];
      currentL++;
    } else { // arrayL[currentL] > arrayR[currentR]
      array[current] = arrayR[currentR];
      currentR++;
    }
  }

  // If <arrayL> was completely consumed, copy the remainder of <arrayR> over
  // the remainder of <array>
  if (currentL >= countL)
    while (currentR < countR) {
      array[current] = arrayR[currentR];
      current++;
      currentR++;
    }

  // If <arrayR> was completely consumed, copy the remainder of <arrayL> over
  // the remainder of <array>
  if (currentR >= countR)
    while (currentL < countL) {
      array[current] = arrayL[currentL];
      current++;
      currentL++;
    }

  free(arrayL);
  free(arrayR);
}

long int nanos_between(struct timespec *final, struct timespec *start) {
  time_t seconds = final->tv_sec - start->tv_sec;
  long int nanoseconds = final->tv_nsec - start->tv_nsec;
  return seconds * 1e9 + nanoseconds;
}
int main(int argc, char *argv[]) {
  const int N = 3001;
  int *tableau = malloc(N * sizeof(int));
  if (!tableau) {
    perror("malloc");
    return EXIT_FAILURE;
  }

  for (int i = 0; i < N; ++i)
    tableau[i] = N - 1 - i; /* exemple décroissant */

  int longueur = N;
  struct timespec start, final;
  clock_gettime(CLOCK_REALTIME, &start);
  struct argument *try = malloc(sizeof(struct argument));
  try->array = tableau;
  try->start = 0;
  try->final = longueur - 1;
  show(tableau, longueur);
  merge_sort_threads(try);

  clock_gettime(CLOCK_REALTIME, &final);

  show(tableau, longueur);
  printf("Time elapsed: %ld ns\n\n", nanos_between(&final, &start));
  return EXIT_SUCCESS;
}
