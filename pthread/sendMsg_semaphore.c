#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define MSG_MAX 1000

int thread_count;
sem_t *pSem;
char *messages[10];

void *SendMsg(void *rank) {
    long my_rank = (long)rank;
    long dest = (my_rank + 1) % thread_count;
    char *my_msg = malloc(MSG_MAX * sizeof(char));

    sprintf(my_msg, "Hello to %ld from %ld", dest, my_rank);
    messages[dest] = my_msg;
    sem_post(&pSem[dest]);

    sem_wait(&pSem[my_rank]);
    printf("Thread %ld > %s\n", my_rank, messages[my_rank]);

    return NULL;
}

int main(int argc, char *argv[]) {
    long thread;
    pthread_t *thread_handles;

    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    pSem = malloc(thread_count * sizeof(sem_t));
    for (int i = 0; i < thread_count; i++) {
        sem_init(&pSem[i], 0, 0);
    }

    for (thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, SendMsg, (void *)thread);
    }

    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    for (int i = 0; i < thread_count; i++) {
        sem_destroy(&pSem[i]);
    }
    free(thread_handles);

    return 0;
}
