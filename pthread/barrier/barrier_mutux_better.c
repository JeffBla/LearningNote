#include <pthread.h>
#include <stdio.h>

int NSmooth, counter[2], thread_count;

int main() {
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    // Barrier with mutex
    for (int count = 0; count < NSmooth; count++) {
        pthread_mutex_lock(&mutex);
        if (counter[count % 2] == thread_count - 1) {
            counter[(count + 1) % 2] = 0;
        }
        counter[(count + 1) % 2]++;
        pthread_mutex_unlock(&mutex);
        while (counter[count % 2] != thread_count)
            ;
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}