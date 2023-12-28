#include <omp.h>
#include <stdio.h>

void OddEvenSort(int a[], int n) {
    int phase, i, temp;
#pragma omp parallel num_threads(thread_count) \ 
    default(none) shared(a, n) private(i, temp, phase)
    for (phase = 0; phase < n; phase++) {
        if (phase % 2 == 0)
#pragma omp for
            for (i = 1; i < n; i += 2) {
                if (a[i - 1] > a[i]) {
                    temp = a[i - 1];
                    a[i - 1] = a[i];
                    a[i] = temp;
                }
            }
        else
#pragma omp for
            for (i = 1; i < n - 1; i += 2) {
                if (a[i] > a[i + 1]) {
                    temp = a[i + 1];
                    a[i + 1] = a[i];
                    a[i] = temp;
                }
            }
    }
}

int main() {
    return 0;
}