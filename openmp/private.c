#include <assert.h>
#include <omp.h>
#include <stdio.h>

#define NUM_THREADS 4
#define SLEEP_THREAD 1
#define NUM_LOOPS 2

enum Types { ThreadPrivate,
             Private,
             FirstPrivate,
             LastPrivate,
             Shared,
             MAX_TYPES };

int nSave[NUM_THREADS][MAX_TYPES][NUM_LOOPS] = {{0}};

int nThreadPrivate;

#pragma omp threadprivate(nThreadPrivate)
#pragma warning(disable : 4700)

int main() {
    int nPrivate = NUM_THREADS;
    int nFirstPrivate = NUM_THREADS;
    int nLastPrivate = NUM_THREADS;
    int nShared = NUM_THREADS;
    int nRet = 0;
    int i, j, nLoop = 0;

    nThreadPrivate = NUM_THREADS;
    printf_s("These are the variables before the parallel region:\n");
    printf_s("nThreadPrivate = %d\n", nThreadPrivate);
    printf_s("nPrivate = %d\n", nPrivate);
    printf_s("nFirstPrivate = %d\n", nFirstPrivate);
    printf_s("nLastPrivate = %d\n", nLastPrivate);
    printf_s("nShared = %d\n", nShared);
    omp_set_num_threads(NUM_THREADS);

#pragma omp parallel for private(nPrivate) firstprivate(nFirstPrivate) lastprivate(nLastPrivate) shared(nShared) schedule(static, 1)
    {
#pragma omp for schedule(static) lastprivate(nLastPrivate)
        for (i = 0; i < NUM_THREADS; i++) {
            for (j = 0; j < NUM_LOOPS; j++) {
                int nThread = omp_get_thread_num();
                assert(nThread >= 0 && nThread < NUM_THREADS);

                if (nThread == SLEEP_THREAD)
                    Sleep(1000);
                nSave[nThread][ThreadPrivate][j] = nThreadPrivate;
                nSave[nThread][Private][j] = nPrivate;
                nSave[nThread][FirstPrivate][j] = nFirstPrivate;
                nSave[nThread][LastPrivate][j] = nLastPrivate;
                nSave[nThread][Shared][j] = nShared;
                nThreadPrivate = nThread;
                nPrivate = nThread;
                nLastPrivate = nThread;
                nShared = nThread;
                --nFirstPrivate;
            }
        }
    }

    for (i = 0; i < NUM_LOOPS; i++) {
        for (j = 0; j < NUM_THREADS; j++) {
            printf_s("Thread %d: ThreadPrivate = %d, Private = %d, FirstPrivate = %d, LastPrivate = %d, Shared = %d\n",
                     j, nSave[j][ThreadPrivate][i], nSave[j][Private][i], nSave[j][FirstPrivate][i], nSave[j][LastPrivate][i], nSave[j][Shared][i]);
        }
    }

    printf_s("These are the variables after the parallel region:\n");
    printf_s("nThreadPrivate = %d\n", nThreadPrivate);
    printf_s("nPrivate = %d\n", nPrivate);
    printf_s("nFirstPrivate = %d\n", nFirstPrivate);
    printf_s("nLastPrivate = %d\n", nLastPrivate);
    printf_s("nShared = %d\n", nShared);

    return 0;
}