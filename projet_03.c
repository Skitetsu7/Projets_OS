#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_THREADS 2
#define RUNS 100
#define N 1000000 // taille du tableau

typedef unsigned int index;
typedef unsigned int length;

struct argument {
    int start, final;
    int *array;
    int *threads_remaining;
};

void show(int array[], length count) {
    for (index i = 0; i < count && i < 100; i++)
        printf("%d ", array[i]);
    if (count > 100) printf("...");
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
    if (start < final) {
        index middle = floor((start + final) / 2);
        merge_sort(array, start, middle);
        merge_sort(array, middle + 1, final);
        merge(array, start, middle, final);
    }
}

void *merge_sort_threads(void *args) {
    struct argument *arg = (struct argument *)args;
    int start = arg->start;
    int final = arg->final;
    int *array = arg->array;
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
        struct argument *left = malloc(sizeof(struct argument));
        left->start = start;
        left->final = middle;
        left->array = array;
        left->threads_remaining = threads_remaining;
        pthread_create(&t1, NULL, merge_sort_threads, left);
        created_thread = 1;
    }

    if (*threads_remaining > 0) {
        (*threads_remaining)--;
        struct argument *right = malloc(sizeof(struct argument));
        right->start = middle + 1;
        right->final = final;
        right->array = array;
        right->threads_remaining = threads_remaining;
        merge_sort_threads(right);
    } else {
        merge_sort(array, middle + 1, final);
    }

    if (created_thread) {
        pthread_join(t1, NULL);
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
    int *list = malloc(N * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < N; ++i)
        list[i] = rand();

    long total_time = 0;

    for (int run = 0; run < RUNS; run++) {
        int *tableau = malloc(N * sizeof(int));
        for (int i = 0; i < N; ++i)
            tableau[i] = list[i];

        int threads_remaining = MAX_THREADS;
        struct timespec start, final;
        clock_gettime(CLOCK_REALTIME, &start);

        struct argument *arg = malloc(sizeof(struct argument));
        arg->start = 0;
        arg->final = N - 1;
        arg->array = tableau;
        arg->threads_remaining = &threads_remaining;

        merge_sort_threads(arg);

        clock_gettime(CLOCK_REALTIME, &final);

        total_time += nanos_between(&final, &start);
        free(tableau);
    }

    printf("moyenne time on %d runs : %ld ns, for %d threads\n", RUNS, total_time / RUNS, MAX_THREADS);
    free(list);
    return 0;
}
