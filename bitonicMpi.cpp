#include <iostream>
#include <vector>
#include <chrono>
#include <mpi.h>

using namespace std;

void compareAndSwap(vector<int>& bitonic_sequence, int low, int high, bool isAscending) {
    int half = (high - low + 1) / 2;

    for (int i = low; i < low + half; i++) {
        if ((bitonic_sequence[i] > bitonic_sequence[i + half]) == isAscending) {
            swap(bitonic_sequence[i], bitonic_sequence[i + half]);
        }
    }
}

void bitonicMerge(vector<int>& bitonic_sequence, int low, int high, bool isAscending) {
    if (high - low > 0) {
        int mid = (high - low + 1) / 2;

        bitonicMerge(bitonic_sequence, low, low + mid - 1, !isAscending);
        bitonicMerge(bitonic_sequence, low + mid, high, !isAscending);

        compareAndSwap(bitonic_sequence, low, high, isAscending);
    }
}

void bitonicSort(vector<int>& bitonic_sequence, int low, int high, bool isAscending) {
    if (high > low) {
        int mid = (high - low + 1) / 2;

        bitonicSort(bitonic_sequence, low, low + mid - 1, true);
        bitonicSort(bitonic_sequence, low + mid, high, false);

        bitonicMerge(bitonic_sequence, low, high, isAscending);
    }
}

void print(const vector<int>& bitonic_sequence) {
    for (int i = 0; i < bitonic_sequence.size(); i++) {
        cout << bitonic_sequence[i] << " ";
    }
    cout << endl;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int processRank, numProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    vector<int> bitonic_sequence{ 3, 7, 4, 8, 6, 2, 1, 5 };
    int sequenceSize = bitonic_sequence.size();

    // Scatter the input sequence to all processes
    int localSize = sequenceSize / numProcesses;
    vector<int> localSequence(localSize);
    MPI_Scatter(bitonic_sequence.data(), localSize, MPI_INT, localSequence.data(), localSize, MPI_INT, 0, MPI_COMM_WORLD);

    // Perform local bitonic sort
    bitonicSort(localSequence, 0, localSize - 1, true);

    // Gather the sorted local sequences back to the root process
    vector<int> sortedSequence(sequenceSize);
    MPI_Gather(localSequence.data(), localSize, MPI_INT, sortedSequence.data(), localSize, MPI_INT, 0, MPI_COMM_WORLD);

    // Root process merges the sorted sequences
    if (processRank == 0) {
        bitonicMerge(sortedSequence, 0, sequenceSize - 1, true);

        cout << "Sorted array: ";
        print(sortedSequence);
    }

    MPI_Finalize();
    return 0;
}
