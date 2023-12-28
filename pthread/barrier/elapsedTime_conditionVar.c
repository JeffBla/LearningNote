#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int counter_start = 0, counter_end = 0;
int thread_count;
pthread_mutex_t mutex_start, mutex_end;
pthread_cond_t cond_var_start, cond_var_end;

void *ThreadTime(void *rank) {
    long my_rank = (long)rank;
    clock_t my_start, my_fin;
    double *my_elapsed;

    // Barrier start
    pthread_mutex_lock(&mutex_start);
    counter_start++;
    if (counter_start == thread_count) {
        counter_start = 0;
        pthread_cond_broadcast(&cond_var_start);
    } else {
        while (pthread_cond_wait(&cond_var_start, &mutex_start))
            ;
    }
    pthread_mutex_unlock(&mutex_start);

    my_start = clock();

    for (long long i = 0; i < 10000000000; i++)
        ;

    // Barrier end
    pthread_mutex_lock(&mutex_end);
    counter_end++;
    if (counter_end == thread_count) {
        counter_end = 0;
        pthread_cond_broadcast(&cond_var_end);
    } else {
        while (pthread_cond_wait(&cond_var_end, &mutex_end))
            ;
    }
    pthread_mutex_unlock(&mutex_end);

    my_fin = clock();

    my_elapsed = malloc(sizeof(double));
    *my_elapsed = ((double)(my_fin - my_start)) / CLOCKS_PER_SEC;

    return (void *)my_elapsed;
}

int main(int argc, char *argv[]) {
    long thread;
    pthread_t *thread_handles;

    pthread_mutex_init(&mutex_start, NULL);
    pthread_mutex_init(&mutex_end, NULL);

    pthread_cond_init(&cond_var_start, NULL);
    pthread_cond_init(&cond_var_end, NULL);

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
