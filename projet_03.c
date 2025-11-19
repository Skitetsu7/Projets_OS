#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_THREADS 2
#define RUNS 100

typedef unsigned int index;
typedef unsigned int length;

struct argument {
    int start, final;
    int *array;
    int thread_remaining;
};

void show(int array[], length count) {
    for (index i = 0; i < count; i++)
        printf("%d ", array[i]);
    printf("\n");
}

void merge(int array[], index start, index middle, index final) {
    length countL = middle - start + 1;
    int *arrayL = malloc(countL * sizeof(int));
    for (index i = 0; i < countL; i++)
        arrayL[i] = array[start + i];

    length countR = final - middle;
    int *arrayR = malloc(countR * sizeof(int));
    for (index i = 0; i < countR; i++)
        arrayR[i] = array[middle + 1 + i];

    index i = 0, j = 0, k = start;
    while (i < countL && j < countR) {
        if (arrayL[i] <= arrayR[j])
            array[k++] = arrayL[i++];
        else
            array[k++] = arrayR[j++];
    }
    while (i < countL) array[k++] = arrayL[i++];
    while (j < countR) array[k++] = arrayR[j++];

    free(arrayL);
    free(arrayR);
}

void merge_sort(int array[], index start, index final) {
    if (start >= final) return;
    index middle = (start + final) / 2;
    merge_sort(array, start, middle);
    merge_sort(array, middle + 1, final);
    merge(array, start, middle, final);
}

void *merge_sort_threads(void *args) {
    struct argument *arg = (struct argument *)args;
    int start = arg->start;
    int final = arg->final;
    int *array = arg->array;

    if (start >= final) {
        free(arg);
        pthread_exit(NULL);
    }

    index middle = (start + final) / 2;

    if (arg->thread_remaining > 0) {
        pthread_t t1, t2;

        struct argument *left = malloc(sizeof(struct argument));
        left->start = start;
        left->final = middle;
        left->array = array;
        left->thread_remaining = (arg->thread_remaining - 1) / 2;

        struct argument *right = malloc(sizeof(struct argument));
        right->start = middle + 1;
        right->final = final;
        right->array = array;
        right->thread_remaining = (arg->thread_remaining - 1) / 2;

        pthread_create(&t1, NULL, merge_sort_threads, left);
        pthread_create(&t2, NULL, merge_sort_threads, right);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
    } else {
        merge_sort(array, start, middle);
        merge_sort(array, middle + 1, final);
    }

    merge(array, start, middle, final);
    free(arg);
    return NULL;
}

long int nanos_between(struct timespec *final, struct timespec *start) {
    time_t seconds = final->tv_sec - start->tv_sec;
    long int nanoseconds = final->tv_nsec - start->tv_nsec;
    return seconds * 1e9 + nanoseconds;
}

int main() {
    const int N = 100000;
    int *original = malloc(N * sizeof(int));
    for (int i = 0; i < N; ++i)
        original[i] = N - 1 - i; /* exemple décroissant */

    long total_time = 0;

    for (int run = 0; run < RUNS; run++) {
        int *tableau = malloc(N * sizeof(int));
        for (int i = 0; i < N; ++i)
            tableau[i] = original[i];

        struct timespec start, final;
        clock_gettime(CLOCK_REALTIME, &start);

        struct argument *arg = malloc(sizeof(struct argument));
        arg->start = 0;
        arg->final = N - 1;
        arg->array = tableau;
        arg->thread_remaining = MAX_THREADS;

        merge_sort_threads(arg);

        clock_gettime(CLOCK_REALTIME, &final);

        total_time += nanos_between(&final, &start);
        free(tableau);
    }

    printf("moyenne time on %d runs : %ld ns, for %d \n", RUNS, total_time / RUNS, MAX_THREADS);

    free(original);
    return 0;
}
