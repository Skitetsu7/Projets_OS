#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//number of thread wich increment after a tes
int NB_THREAD = 1;
//number of time your thread is tested
#define TEST_NBR 10
// lenght of the table you want to sort
#define LEN_TAB 1000000000

typedef unsigned int index;
typedef unsigned int length;

// calculus of passed time
long int nanos_between(struct timespec *final, struct timespec *start) {
  time_t seconds = final->tv_sec - start->tv_sec;
  long int nanoseconds = final->tv_nsec - start->tv_nsec;
  return seconds * 1e9 + nanoseconds;
}
// argument used to array passed to a thread
struct thread_arg {
  int start;
  int end;
  int *array;
};

// the fonction was a modified to take a structure as argument so it can be executed by a thread
void *merge_sort(void *arguments) {
  struct thread_arg *args = (void *)arguments;
  int start = args->start;
  int final = args->end;
  int *array = args->array;
  if (start < final) {
    index middle = floor((start + final) / 2);
    // copie of left and right to give it  in a new merge sort
    struct thread_arg left;
    left.start = start;
    left.end = middle;
    left.array = array;
    struct thread_arg right;
    right.start = middle + 1;
    right.end = final;
    right.array = array;
    merge_sort(&left);
    merge_sort(&right);
    merge(array, start, middle, final);
  }
  return NULL;
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

//show as been a bit modified
void show(int array[], length count) {
  for (index i = 0; i < count; i++)
    printf("%d ", array[i]);
  printf("\n");
}

int main() {
// start a test
while(NB_THREAD < 8){
//initialise array of result
    long unsigned int result[TEST_NBR];

    for (int test =0; test < TEST_NBR; test++) {
        // stock array in RAM
        int *tableau = malloc(LEN_TAB * sizeof(int));
        if (!tableau) {
            exit(EXIT_FAILURE);
        }
        //stock value in array
        for (int i = 0; i < LEN_TAB; i++) {
            tableau[i] = rand() % 10000;
        }

        int longueur = LEN_TAB;

        pthread_t T[NB_THREAD];
        int *découpage = malloc(sizeof(int) * NB_THREAD);
        for (int i = 0; i < NB_THREAD; i++) {
            découpage[i] = (i + 1)*longueur/ NB_THREAD - 1;
        }

        struct thread_arg *argument = malloc(sizeof(struct thread_arg) * NB_THREAD);
        argument[0].array = tableau;
        argument[0].start = 0;
        argument[0].end = découpage[0];

        for (int i = 1; i < NB_THREAD; i++) {
            argument[i].array =tableau;
            argument[i].start= découpage[i-1] + 1;
            argument[i].end = découpage[i];
        }

        struct timespec s, f;
        clock_gettime(CLOCK_REALTIME, &s);

        for (int i = 0;i <NB_THREAD;i++)
            pthread_create(&T[i], NULL, merge_sort, &argument[i]);

        for (int i= 0; i < NB_THREAD; i++)
            pthread_join(T[i], NULL);

        int start =0;
        for (int i = 0; i < NB_THREAD - 1; i++) {
            merge(tableau, start, découpage[i], découpage[i + 1]);
        }

        clock_gettime(CLOCK_REALTIME, &f);

        result[test] = nanos_between(&f, &s);

        free(argument);
        free(découpage);
        free(tableau);
    }

    long unsigned int m = 0;
    for (int i = 0; i < TEST_NBR; i++)
        m += result[i];

    m = m/TEST_NBR;
    printf("\n\npour %d threads %d tests -> moyenne de %lu ns\n",NB_THREAD, TEST_NBR, m);

    NB_THREAD++;
  }
    return EXIT_SUCCESS;
}
