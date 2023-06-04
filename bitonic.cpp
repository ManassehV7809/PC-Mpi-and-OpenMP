#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

void compareAndSwap(vector<int>&bitonic_sequence, int low, int high, bool isAscending) {
   int half = (high + low) / 2;

    for (int i = low; i < low + half; i++) {
        if ((bitonic_sequence[i] > bitonic_sequence[i + half]) == isAscending) {
            swap(bitonic_sequence[i], bitonic_sequence[i + half]);
        }
    }
}

void bitonicMerge(vector<int>& bitonic_sequence, int low, int high, bool isAscending) {
    if (high - low > 0) {
       int half = (high + low) / 2;

        bitonicMerge(bitonic_sequence, low, low + half- 1, !isAscending);
        bitonicMerge(bitonic_sequence, low + half, high, !isAscending);

        compareAndSwap(bitonic_sequence, low, high, isAscending);
    }
}

void bitonicSort(vector<int>& bitonic_sequence, int low, int high, bool isAscending) {
    if (high > low) {
        int half = (high - low + 1) / 2;

        bitonicSort(bitonic_sequence, low, low + half - 1, true);
        bitonicSort(bitonic_sequence, low + half, high, false);

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


