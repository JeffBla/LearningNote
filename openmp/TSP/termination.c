#include <mpi.h>
#include <omp.h>
#include <stdio.h>

MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status);

omp_test_lock(&lock);