#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;

void compareAndSwap(vector<int>& bitonic_sequence, int low, int high, bool isAscending) {
    int half = (high - low + 1) / 2;

    #pragma omp parallel for
    for (int i = low; i < low + half; i++) {
        if ((bitonic_sequence[i] > bitonic_sequence[i + half]) == isAscending) {
            swap(bitonic_sequence[i], bitonic_sequence[i + half]);
        }
    }
}

void bitonicMerge(vector<int>& bitonic_sequence, int low, int high, bool isAscending) {
    if (high - low > 0) {
        int mid = (high - low + 1) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            bitonicMerge(bitonic_sequence, low, low + mid - 1, !isAscending);

            #pragma omp section
            bitonicMerge(bitonic_sequence, low + mid, high, !isAscending);
        }

        compareAndSwap(bitonic_sequence, low, high, isAscending);
    }
}

void bitonicSort(vector<int>& bitonic_sequence, int low, int high, bool isAscending) {
    if (high > low) {
        int mid = (high - low + 1) / 2;

        #pragma omp parallel
        {
            #pragma omp single nowait
            {
                #pragma omp task
                bitonicSort(bitonic_sequence, low, low + mid - 1, true);
            }

            #pragma omp single nowait
            {
                #pragma omp task
                bitonicSort(bitonic_sequence, low + mid, high, false);
            }
        }

        #pragma omp taskwait

        bitonicMerge(bitonic_sequence, low, high, isAscending);
    }
}

void print(const vector<int>& bitonic_sequence) {
    for (int i = 0; i < bitonic_sequence.size(); i++) {
        cout << bitonic_sequence[i] << " ";
    }
    cout << endl;
}

int main() {
    vector<int> bitonic_sequence{ 3, 7, 4, 8, 6, 2, 1, 5 };

    cout << "Random input: ";
    print(bitonic_sequence);

    auto start = chrono::high_resolution_clock::now();
    bitonicSort(bitonic_sequence, 0, bitonic_sequence.size() - 1, true);
    auto finish = chrono::high_resolution_clock::now();
    long long timeTaken = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
    cout << "Time: " << timeTaken << endl;

    cout << "Sorted array: ";
    print(bitonic_sequence);

    return 0;
}
