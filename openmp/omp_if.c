#include <omp.h>
#include <stdio.h>

void test(int val) {
    // #pragma omp parallel if(val)
#pragma omp parallel if (val > 0)
    {
        if (omp_in_parallel())
            printf("Hello World from thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads());
        else
            printf("Hello World!\n");
    }
}

int main() {
    test(1);
    test(0);
    return 0;
}