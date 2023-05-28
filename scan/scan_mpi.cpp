#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void parallelScan(const int *input, int *output, int size, MPI_Comm comm)
{
    int rank, numprocs;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &numprocs);

    int *prefixSums = (int *)malloc(sizeof(int) * size);
    prefixSums[0] = input[0];

    for (int i = 1; i < size; ++i)
    {
        prefixSums[i] = prefixSums[i - 1] + input[i];
    }

    int *prefixSumsAll = (int *)malloc(sizeof(int) * numprocs);
    MPI_Allgather(&prefixSums[size - 1], 1, MPI_INT, prefixSumsAll, 1, MPI_INT, comm);

    int prefixSumOffset = 0;
    for (int i = 0; i < rank; ++i)
    {
        prefixSumOffset += prefixSumsAll[i];
    }

    for (int i = 0; i < size; ++i)
    {
        output[i] = prefixSums[i] + prefixSumOffset;
    }

    free(prefixSums);
    free(prefixSumsAll);
}

int main(int argc, char *argv[])
{
    const int size = 10;
    int input[size] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int output[size];

    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    parallelScan(input, output, size, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Prefix Sum: ");
        for (int i = 0; i < size; ++i)
        {
            printf("%d ", output[i]);
        }
        printf("\n");
    }

    MPI_Finalize();

    return 0;
}
