#include <omp.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int n, mMax;
vector<int> A;

void GetInput() {
    char filename[50];
    scanf("%s", filename);
    ifstream inFile(filename);

    int num;
    inFile >> n >> mMax;
    A.resize(n + 1);
    for (int i = 1; i < n + 1; i++) {
        inFile >> num;
        A[i] = num;
    }
}

void counting_sort(vector<int>& A, vector<int>& B, int k) {
    vector<int> C(k + 1, 0);
    vector<omp_lock_t> c_lock(k + 1);
#pragma omp parallel
    {
#pragma omp for
        for (int i = 0; i < k + 1; i++) {
            omp_init_lock(&c_lock[i]);
        }

#pragma omp for
        for (int j = 1; j < A.size(); j++) {
            omp_set_lock(&c_lock[A[j]]);
            C[A[j]] = C[A[j]] + 1;
            omp_unset_lock(&c_lock[A[j]]);
        }

#pragma omp barrier
#pragma omp single
        for (int i = 1; i <= k + 1; i++) {
            C[i] += C[i - 1];
        }

#pragma omp barrier
#pragma omp for
        for (int j = A.size() - 1; j > 0; j--) {
            omp_set_lock(&c_lock[A[j]]);
            B[C[A[j]]] = A[j];
            C[A[j]]--;
            omp_unset_lock(&c_lock[A[j]]);
        }

#pragma omp for
        for (int i = 0; i < k + 1; i++) {
            omp_destroy_lock(&c_lock[i]);
        }
    }
}

int main() {
    GetInput();

    vector<int> B(n + 1);
    counting_sort(A, B, mMax);

    for (int i = 1; i < n + 1; i++) {
        cout << B[i] << " ";
    }
    return 0;
}