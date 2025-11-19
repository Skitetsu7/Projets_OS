#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_THREADS 2
#define RUNS 10
#define N 1000000

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
    length countR = final - middle;
    int *arrayR = malloc(countR * sizeof(int));
    index i, j, k;

    for (i = 0; i < countL; i++)
        arrayL[i] = array[start + i];
    for (j = 0; j < countR; j++)
        arrayR[j] = array[middle + 1 + j];

    i = 0; j = 0; k = start;
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
        index middle = (start + final) / 2;
        merge_sort(array, start, middle);
        merge_sort(array, middle + 1, final);
        merge(array, start, middle, final);
    }
}

void *merge_sort_threads(void *arg_void) {
    struct argument *try = (struct argument *)arg_void;
    int start = try->start;
    int final = try->final;
    int *tableau = try->array;
    int *threads_remaining = try->threads_remaining;

    if (start >= final) {
        free(try);
        pthread_exit(NULL);
    }

    index middle = (start + final) / 2;

    pthread_t t1;
    int created_thread = 0;

    if (*threads_remaining > 0) {
        (*threads_remaining)--;
        struct argument *left = malloc(sizeof(struct argument));
        left->start = start;
        left->final = middle;
        left->array = tableau;
        left->threads_remaining = threads_remaining;
        pthread_create(&t1, NULL, merge_sort_threads, left);
        created_thread = 1;
    }

    if (*threads_remaining > 0) {
        (*threads_remaining)--;
        struct argument *right = malloc(sizeof(struct argument));
        right->start = middle + 1;
        right->final = final;
        right->array = tableau;
        right->threads_remaining = threads_remaining;
        merge_sort_threads(right);
    } else {
        merge_sort(tableau, middle + 1, final);
    }

    if (created_thread) {
        pthread_join(t1, NULL);
    }

    merge(tableau, start, middle, final);
    free(try);
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

        struct argument *try = malloc(sizeof(struct argument));
        try->start = 0;
        try->final = N - 1;
        try->array = tableau;
        try->threads_remaining = &threads_remaining;

        merge_sort_threads(try);

        clock_gettime(CLOCK_REALTIME, &final);

        total_time += nanos_between(&final, &start);
        free(tableau);
    }

    printf("moyenne time on %d runs : %ld ns, for %d threads\n", RUNS, total_time / RUNS, MAX_THREADS);

    free(list);
    return 0;
}
