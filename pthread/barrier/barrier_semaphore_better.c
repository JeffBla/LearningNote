#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

int NSmooth, thread_count, counter;

int main() {
    sem_t count_sem, barrier_sem[2];
    sem_init(&count_sem, 0, 1);
    sem_init(&barrier_sem[0], 0, 0);
    sem_init(&barrier_sem[1], 0, 0);

    // Barrier with semaphore
    for (int count = 0; count < NSmooth; count++) {
        sem_wait(&count_sem);
        if (counter == thread_count - 1) {
            counter = 0;
            sem_post(&count_sem);
            for (int j = 0; j < thread_count - 1; j++)
                sem_post(&barrier_sem[count % 2]);
        } else {
            counter++;
            sem_post(&count_sem);
            sem_wait(&barrier_sem[count % 2]);
        }
    }

    sem_destroy(&count_sem);
    sem_destroy(&barrier_sem[0]);
    sem_destroy(&barrier_sem[1]);
    return 0;
}