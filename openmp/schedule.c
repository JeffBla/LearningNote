#include <omp.h>
#include <stdio.h>

// original f
double f(double i) {
    int j, start = i * (i + 1) / 2, finish = start + i;
    double return_val = 0.0;

    for (j = start; j <= finish; j++)
        return_val += sin(j);
    return return_val;
}

// default schedule
double f(double i) {
    int j, start = i * (i + 1) / 2, finish = start + i;
    double return_val = 0.0;

#pragma omp parallel for
    for (j = start; j <= finish; j++)
        return_val += sin(j);
    return return_val;
}

// cyclic schedule
double f(double i) {
    int j, start = i * (i + 1) / 2, finish = start + i;
    double return_val = 0.0;

#pragma omp parallel for schedule(static, 1)
    for (j = start; j <= finish; j++)
        return_val += sin(j);
    return return_val;
}

int main() {
    return 0;
}