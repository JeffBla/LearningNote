#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int counter_start = 0, counter_end = 0;
int thread_count;
sem_t counter_sem_start, counter_sem_end;
sem_t barrier_sem_start, barrier_sem_end;

void *ThreadTime(void *rank) {
    long my_rank = (long)rank;
    clock_t my_start, my_fin;
    double *my_elapsed;

    // Barrier start
    sem_wait(&counter_sem_start);
    if (counter_start == thread_count - 1) {
        counter_start = 0;
        sem_post(&counter_sem_start);
        for (int j = 0; j < thread_count - 1; j++)
            sem_post(&barrier_sem_start);
    } else {
        counter_start++;
        sem_post(&counter_sem_start);
        sem_wait(&barrier_sem_start); /* 如果重覆使用這個barrier_sem 可能因為有的thread跑太快再重新進入這裡, 導致錯誤  */
    }

    my_start = clock();

    for (long long i = 0; i < 10000000000; i++)
        ;

    // Barrier end
    sem_wait(&counter_sem_end);
    if (counter_end == thread_count - 1) {
        counter_end = 0;
        sem_post(&counter_sem_end);
        for (int j = 0; j < thread_count - 1; j++)
            sem_post(&barrier_sem_end);
    } else {
        counter_end++;
        sem_post(&counter_sem_end);
        sem_wait(&barrier_sem_end);
    }

    my_fin = clock();

    my_elapsed = malloc(sizeof(double));
    *my_elapsed = ((double)(my_fin - my_start)) / CLOCKS_PER_SEC;

    return (void *)my_elapsed;
}

int main(int argc, char *argv[]) {
    long thread;
    pthread_t *thread_handles;

    sem_init(&counter_sem_start, 0, 1);
    sem_init(&counter_sem_end, 0, 1);

    sem_init(&barrier_sem_start, 0, 0);
    sem_init(&barrier_sem_end, 0, 0);

    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    for (thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, ThreadTime, (void *)thread);
    }

    double *return_arr[thread_count];
    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], (void **)&(return_arr[thread]));
    }

    for (int i = 0; i < thread_count; i++) {
        printf("%lf ", *return_arr[i]);
        fflush(stdout);
        free(return_arr[i]);
    }

    free(thread_handles);

    return 0;
}
