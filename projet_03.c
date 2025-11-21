#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

//initialisation for test
int NB_THREAD = 1;
#define THREAD_LIM 8
#define TEST_NBR 10
// size of array
#define LEN_TAB 30
typedef unsigned int index;
typedef unsigned int length;

// calculus of time
long int nanos_between(struct timespec *final, struct timespec *start) {
  time_t seconds = final->tv_sec - start->tv_sec;
  long int nanoseconds = final->tv_nsec - start->tv_nsec;
  return seconds * 1e9 + nanoseconds;
}
// argument for a thread because we changed merge_sort()
struct thread_arg {
  int start;
  int end;
  int *array;
};
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

// we changed show to print start and end of the array we want to show
void show(int array[], int start, int end) {
  while (start<end){

    printf("%d ", array[start]);
    start++;
  }
  printf("\n");
}
// modification to take void* arguments as parameters 
void *merge_sort(void *arguments) {
  struct thread_arg *args = (void *)arguments;
  int start = args->start;
  int final = args->end;
  int *array = args->array;
  if (start < final) {
    index middle = floor((start + final) / 2);
    // copy for left and right size of the array
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



int main() {
while(NB_THREAD < THREAD_LIM){
    long unsigned int result[TEST_NBR];

    for (int test = 0; test < TEST_NBR; test++) {
    
        // allocation of tableau in RAM
        int *tableau = malloc(LEN_TAB * sizeof(int));
        if (!tableau) {
            exit(EXIT_FAILURE);
        }
        // array filler
        for (int i = 0; i < LEN_TAB; i++) {
            tableau[i] = rand() % 10000;
        }

        int longueur = LEN_TAB;
        // be cause each thread do the same thing we print only on the first iteration
        if (test == 0){
            printf("liste pas trié");
            show(tableau, 0, longueur);
        }

        // innitialise thread array to don't have to write them one by one
        pthread_t T[NB_THREAD];
        int *découpage = malloc(sizeof(int) * NB_THREAD);

        //give each part of the table to each thread depending of the number of thread
        for (int i = 0; i < NB_THREAD; i++) {
            découpage[i] = (i + 1)*longueur/ NB_THREAD;
            découpage[i]--;
        }
        //allocate RAM for structure
        struct thread_arg *argument = malloc(sizeof(struct thread_arg) * NB_THREAD);
        //stock struct in argument array
        argument[0].array = tableau;
        argument[0].start = 0;
        argument[0].end = découpage[0];
        
        
        for (int i = 1; i < NB_THREAD; i++) {
            argument[i].array =tableau;
            argument[i].start= découpage[i-1] + 1;
            argument[i].end = découpage[i];
        }
        // timer initialiser
        struct timespec s, f;
        clock_gettime(CLOCK_REALTIME, &s);
        //lanching thread
        for (int i = 0;i <NB_THREAD;i++){
            if(test== 0){
            printf("thread_%d[%d-%d]\n",i+1,argument[i].start,argument[i].end);
            printf("array qu'il va trié\n");
            show(tableau, argument[i].start, argument[i].end);
            }
            pthread_create(&T[i], NULL, merge_sort, &argument[i]);
}
        // wait all thread to end
        for (int i= 0; i < NB_THREAD; i++)
            pthread_join(T[i], NULL);
        //join each little array in the big one
        int start =0;
        for (int i = 0; i < NB_THREAD - 1; i++) {
            merge(tableau, start, découpage[i], découpage[i + 1]);
        }
        

        clock_gettime(CLOCK_REALTIME, &f);

        result[test] = nanos_between(&f, &s);

        if (test == 0){
            printf("tableau trié: ");
            show(tableau,0, longueur);
        }
        free(argument);
        free(découpage);
        free(tableau);
        

    }
    
    //calcul of average score
    long unsigned int m = 0;
    for (int i = 0; i < TEST_NBR; i++)
        m += result[i];

    m = m/TEST_NBR;
    printf("\n\npour %d threads %d tests -> moyenne de %lu ns\n",NB_THREAD, TEST_NBR, m);

    NB_THREAD++;
  }
    return EXIT_SUCCESS;
}