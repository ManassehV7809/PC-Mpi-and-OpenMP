#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;

void bitonicMerge(vector<int>& bionic_sequence, int startIndex, int count, bool isAscending) {
    if (count > 1) {
        int k = count / 2;

#pragma omp parallel for shared(bionic_sequence, isAscending) default(none)
        for (int i = startIndex; i < startIndex + k; i++) {
            int num1 = bionic_sequence[i];
            int num2 = bionic_sequence[i + k];

            if ((num1 > num2 && isAscending) || (num1 < num2 && !isAscending)) {
                bionic_sequence[i] = num2;
                bionic_sequence[i + k] = num1;
            }
        }

        bitonicMerge(bionic_sequence, startIndex, k, isAscending);
        bitonicMerge(bionic_sequence, startIndex + k, k, isAscending);
    }
}

void generateSequence(vector<int>& bionic_sequence) {
    int groupSize = 2;
    int HALF = bionic_sequence.size() / 2;

    while (groupSize <= HALF) {
        bool isAscending = true;
        if (groupSize == HALF) {
            bitonicMerge(bionic_sequence, 0, HALF, true);
            bitonicMerge(bionic_sequence, HALF, HALF, true);
        }

#pragma omp parallel for shared(bionic_sequence, isAscending) default(none)
        for (int i = 0; i < bionic_sequence.size(); i = i + groupSize) {
            bitonicMerge(bionic_sequence, i, groupSize, isAscending);
            isAscending = !isAscending;
        }

        groupSize = groupSize * 2;
    }

    bitonicMerge(bionic_sequence, 0, HALF, true);
    bitonicMerge(bionic_sequence, HALF, HALF, false);

#pragma omp parallel for shared(bionic_sequence) default(none)
    for (int i = 0; i < HALF; i = i + 2) {
        bitonicMerge(bionic_sequence, i, 2, true);
    }

#pragma omp parallel for shared(bionic_sequence) default(none)
    for (int i = HALF; i < bionic_sequence.size(); i = i + 2) {
        bitonicMerge(bionic_sequence, i, 2, false);
    }
}

void bitonicSort(vector<int>& bionic_sequence, int low, int high) {
    int MAX_IT = low + ((high - low + 1) / 2);

    int count = 0;
#pragma omp parallel for shared(bionic_sequence) default(none)
    for (int i = low; i < MAX_IT; i++) {
        int index = MAX_IT + count;
        int element = bionic_sequence[index];
        int curr = bionic_sequence[i];

        if (curr > element) {
            bionic_sequence[i] = element;
            bionic_sequence[index] = curr;
        }
        count++;
    }

    int leftLow = low;
    int leftHigh = MAX_IT - 1;

    int rightLow = MAX_IT;
    int rightHigh = high;

    if (leftHigh - leftLow > 0) {
        bitonicSort(bionic_sequence, leftLow, leftHigh);
    }

    if (rightHigh - rightLow > 0) {
        bitonicSort(bionic_sequence, rightLow, rightHigh);
    }
}

void print(const vector<int>& bionic_sequence) {
    for (int i = 0; i < bionic_sequence.size(); i++) {
        cout << bionic_sequence[i] << " ";
    }
    cout << "\n" << endl;
}

int main() {
    vector<int> bionic_sequence = { 5, 214, 754, 72, 6, 357, 5, 74, 7, 96, 67, 80, 5, 4, 0, 3 };

    cout << "Random input" << endl;
    print(bionic_sequence);

    generateSequence(bionic_sequence);

    cout << "After generating bitonic sequence" << endl;
    print(bionic_sequence);

    auto start = chrono::high_resolution_clock::now();
    bitonicSort(bionic_sequence, 0, bionic_sequence.size() - 1);
    auto finish = chrono::high_resolution_clock::now();
    long long timeTaken = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
    cout << "Time taken: " << timeTaken << " nanoseconds" << endl;
    cout << "Sorted array" << endl;
    print(bionic_sequence);

    return 0;
}
