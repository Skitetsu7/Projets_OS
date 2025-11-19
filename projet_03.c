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
    int *threads_remaining;
};

void show(int array[], length count) {
    for (index i = 0; i < count; i++)
        printf("%d ", array[i]);
    printf("\n");
}

void *merge_sort_threads(void *args) {
    struct argument *arg = (struct argument *)args;
    int *ar = arg->array;
    int start = arg->start;
    int final = arg->final;
    int *threads_remaining = arg->threads_remaining;

    if (start >= final) {
        free(arg);
        pthread_exit(NULL);
    }

    index middle = floor((start + final) / 2);

    pthread_t t1;
    int created_thread = 0;

    if (*threads_remaining > 0) {
        (*threads_remaining)--;
        struct argument *first = malloc(sizeof(struct argument));
        first->start = start;
        first->final = middle;
        first->array = ar;
        first->threads_remaining = threads_remaining;
        pthread_create(&t1, NULL, merge_sort_threads, first);
        created_thread = 1;
    }

    if (*threads_remaining > 0) {
        (*threads_remaining)--;
        struct argument *rest = malloc(sizeof(struct argument));
        rest->start = middle + 1;
        rest->final = final;
        rest->array = ar;
        rest->threads_remaining = threads_remaining;
        merge_sort_threads(rest);
    } else {
        merge_sort(ar, middle + 1, final);
    }

    if (created_thread) {
        pthread_join(t1, NULL);
    }

    merge(ar, start, middle, final);
    free(arg);
    return NULL;
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
        tableau[i] = N - 1 - i;

    int longueur = N;
    struct timespec start, final;
    clock_gettime(CLOCK_REALTIME, &start);

    int threads_remaining = MAX_THREADS;

    struct argument *try = malloc(sizeof(struct argument));
    try->array = tableau;
    try->start = 0;
    try->final = longueur - 1;
    try->threads_remaining = &threads_remaining;

    show(tableau, longueur);
    merge_sort_threads(try);

    clock_gettime(CLOCK_REALTIME, &final);

    show(tableau, longueur);
    printf("Time elapsed: %ld ns\n\n", nanos_between(&final, &start));
    return EXIT_SUCCESS;
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
    for (current = start; current <= final && currentL < countL && currentR < countR; current++) {
        if (arrayL[currentL] <= arrayR[currentR]) {
            array[current] = arrayL[currentL];
            currentL++;
        } else {
            array[current] = arrayR[currentR];
            currentR++;
        }
    }

    if (currentL >= countL)
        while (currentR < countR) {
            array[current] = arrayR[currentR];
            current++;
            currentR++;
        }
    if (currentR >= countR)
        while (currentL < countL) {
            array[current] = arrayL[currentL];
            current++;
            currentL++;
        }

    free(arrayL);
    free(arrayR);
}
