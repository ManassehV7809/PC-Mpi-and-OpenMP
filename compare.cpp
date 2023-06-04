#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;
// Alternate the elements in a group based on the given order

void alternate(vector<int>& bitonic_sequence, vector<int>& group, bool isAscending, int startIndex) {
    int halfSize = group.size() / 2;

    for (int i = 0; i < halfSize; i++) {
        int& num1 = group[i];
        int& num2 = group[i + halfSize];

        if ((isAscending && num1 > num2) || (!isAscending && num1 < num2)) {
            swap(num1, num2);
        }
    }

    for (const auto& num : group) {
        bitonic_sequence[startIndex++] = num;
    }
}

// Sort a group of elements using the alternate ordering
void step3(vector<int>& bitonic_sequence, vector<int> group, int startIndex) {
    int halfSize = group.size() / 2;

    for (int i = 0; i < group.size(); i += 2) {
        int& num1 = group[i];
        int& num2 = group[i + 1];
        bool isFirstHalf = i < halfSize;

        if ((isFirstHalf && num1 > num2) || (!isFirstHalf && num1 < num2)) {
            swap(num1, num2);
        }
    }

    for (const auto& num : group) {
        bitonic_sequence[startIndex++] = num;
    }
}

// Generate the bitonic sequence
void generateSequence(vector<int>& bitonic_sequence) {
    int groupSize = 2;
    int HALF = bitonic_sequence.size() / 2;

    while (groupSize <= HALF) {
        bool isAscending = true;

        if (groupSize == HALF) {
            vector<int> firstHalf(bitonic_sequence.begin(), bitonic_sequence.begin() + HALF);
            vector<int> secondHalf(bitonic_sequence.begin() + HALF, bitonic_sequence.end());

            step3(bitonic_sequence, firstHalf, 0);
            step3(bitonic_sequence, secondHalf, HALF);
        }

        for (int i = 0; i < bitonic_sequence.size(); i += groupSize) {
            vector<int> group(bitonic_sequence.begin() + i, bitonic_sequence.begin() + i + groupSize);
            alternate(bitonic_sequence, group, isAscending, i);
            isAscending = !isAscending;
        }

        groupSize *= 2;
    }

    vector<int> leftGroup1(bitonic_sequence.begin(), bitonic_sequence.begin() + HALF / 2);
    vector<int> leftGroup2(bitonic_sequence.begin() + HALF / 2, bitonic_sequence.begin() + HALF);
    vector<int> rightGroup1(bitonic_sequence.begin() + HALF, bitonic_sequence.begin() + HALF + HALF / 2);
    vector<int> rightGroup2(bitonic_sequence.begin() + HALF + HALF / 2, bitonic_sequence.end());

    alternate(bitonic_sequence, leftGroup1, true, 0);
    alternate(bitonic_sequence, leftGroup2, true, HALF / 2);
    alternate(bitonic_sequence, rightGroup1, false, HALF);
    alternate(bitonic_sequence, rightGroup2, false, HALF + HALF / 2);

    for (int i = 0; i < HALF; i += 2) {
        vector<int> group{ bitonic_sequence[i], bitonic_sequence[i + 1] };
        alternate(bitonic_sequence, group, true, i);
    }

    for (int i = HALF; i < bitonic_sequence.size(); i += 2) {
        vector<int> group{ bitonic_sequence[i], bitonic_sequence[i + 1] };
        alternate(bitonic_sequence, group, false, i);
    }
}

// Perform bitonic sort recursively
void bitonicSort(vector<int>& bitonic_sequence, int low, int high) {
    int mid = low + (high - low + 1) / 2;

    for (int i = low; i < mid; i++) {
        int compareIndex = mid + (i - low);
        int& current = bitonic_sequence[i];
        int& compare = bitonic_sequence[compareIndex];

        if (current > compare) {
            swap(current, compare);
        }
    }

    if (mid - low > 1) {
        bitonicSort(bitonic_sequence, low, mid - 1);
    }

    if (high - mid > 0) {
        bitonicSort(bitonic_sequence, mid, high);
    }
}

//omp stuff

// Alternate the elements in a group based on the given order
void ompalternate(vector<int>& bitonic_sequence, vector<int>& group, bool isAscending, int startIndex) {
    int halfSize = group.size() / 2;

    #pragma omp parallel for
    for (int i = 0; i < halfSize; i++) {
        int& num1 = group[i];
        int& num2 = group[i + halfSize];

        if ((isAscending && num1 > num2) || (!isAscending && num1 < num2)) {
            swap(num1, num2);
        }
    }

    #pragma omp parallel for
    for (const auto& num : group) {
        bitonic_sequence[startIndex++] = num;
    }
}

// Sort a group of elements using the alternate ordering
void ompstep3(vector<int>& bitonic_sequence, vector<int> group, int startIndex) {
    int halfSize = group.size() / 2;

    #pragma omp parallel for
    for (int i = 0; i < group.size(); i += 2) {
        int& num1 = group[i];
        int& num2 = group[i + 1];
        bool isFirstHalf = i < halfSize;

        if ((isFirstHalf && num1 > num2) || (!isFirstHalf && num1 < num2)) {
            swap(num1, num2);
        }
    }

    #pragma omp parallel for
    for (const auto& num : group) {
        bitonic_sequence[startIndex++] = num;
    }
}

// Generate the bitonic sequence
void ompgenerateSequence(vector<int>& bitonic_sequence) {
    int groupSize = 2;
    int HALF = bitonic_sequence.size() / 2;

    while (groupSize <= HALF) {
        bool isAscending = true;

        if (groupSize == HALF) {
            vector<int> firstHalf(bitonic_sequence.begin(), bitonic_sequence.begin() + HALF);
            vector<int> secondHalf(bitonic_sequence.begin() + HALF, bitonic_sequence.end());

            ompstep3(bitonic_sequence, firstHalf, 0);
            ompstep3(bitonic_sequence, secondHalf, HALF);
        }

        #pragma omp parallel for
        for (int i = 0; i < bitonic_sequence.size(); i += groupSize) {
            vector<int> group(bitonic_sequence.begin() + i, bitonic_sequence.begin() + i + groupSize);
            ompalternate(bitonic_sequence, group, isAscending, i);
            isAscending = !isAscending;
        }

        groupSize *= 2;
    }

    vector<int> leftGroup1(bitonic_sequence.begin(), bitonic_sequence.begin() + HALF / 2);
    vector<int> leftGroup2(bitonic_sequence.begin() + HALF / 2, bitonic_sequence.begin() + HALF);
    vector<int> rightGroup1(bitonic_sequence.begin() + HALF, bitonic_sequence.begin() + HALF + HALF / 2);
    vector<int> rightGroup2(bitonic_sequence.begin() + HALF + HALF / 2, bitonic_sequence.end());

    ompalternate(bitonic_sequence, leftGroup1, true, 0);
    ompalternate(bitonic_sequence, leftGroup2, true, HALF / 2);
    ompalternate(bitonic_sequence, rightGroup1, false, HALF);
    ompalternate(bitonic_sequence, rightGroup2, false, HALF + HALF / 2);

    #pragma omp parallel for
    for (int i = 0; i < HALF; i += 2) {
        vector<int> group{ bitonic_sequence[i], bitonic_sequence[i + 1] };
        ompalternate(bitonic_sequence, group, true, i);
    }

    #pragma omp parallel for
    for (int i = HALF; i < bitonic_sequence.size(); i += 2) {
        vector<int> group{ bitonic_sequence[i], bitonic_sequence[i + 1] };
        ompalternate(bitonic_sequence, group, false, i);
    }
}

// Perform bitonic sort recursively
void ompbitonicSort(vector<int>& bitonic_sequence, int low, int high) {
    int mid = low + (high - low + 1) / 2;

    #pragma omp parallel for
    for (int i = low; i < mid; i++) {
        int compareIndex = mid + (i - low);
        int& current = bitonic_sequence[i];
        int& compare = bitonic_sequence[compareIndex];

        if (current > compare) {
            swap(current, compare);
        }
    }

    if (mid - low > 1) {
        ompbitonicSort(bitonic_sequence, low, mid - 1);
    }

    if (high - mid > 0) {
        ompbitonicSort(bitonic_sequence, mid, high);
    }
}

int main() {
    vector<int> bitonic_sequence ={83, 17, 92, 29, 71, 5, 49, 60, 44, 14, 78, 26, 65, 53, 36, 69, 23, 99, 33, 88, 57, 41, 75, 62, 11, 94, 38, 80, 55, 2, 47, 20};

    // Serial bitonic sort
    vector<int> bitonic_sequence_serial = bitonic_sequence; // Create a copy for serial sort
    generateSequence(bitonic_sequence_serial);

    auto startSerial = high_resolution_clock::now();
    bitonicSort(bitonic_sequence_serial, 0, bitonic_sequence_serial.size() - 1);
    auto finishSerial = high_resolution_clock::now();
    long long timeTakenSerial = duration_cast<milliseconds>(finishSerial - startSerial).count();

    // Parallel bitonic sort
    vector<int> bitonic_sequence_parallel = bitonic_sequence; // Create a copy for parallel sort
    ompgenerateSequence(bitonic_sequence_parallel);

    auto startParallel = high_resolution_clock::now();
    ompbitonicSort(bitonic_sequence_parallel, 0, bitonic_sequence_parallel.size() - 1);
    auto finishParallel = high_resolution_clock::now();
    long long timeTakenParallel = duration_cast<milliseconds>(finishParallel - startParallel).count();

    // Print the sorted arrays and their runtimes
    cout << "Serial bitonic sort:" << endl;
    cout << "Time taken: " << timeTakenSerial << " ms" << endl;
    cout << "Sorted array:" << endl;
    for (int i = 0; i < bitonic_sequence_serial.size(); i++) {
        cout << bitonic_sequence_serial[i] << " ";
    }
    cout << endl << endl;

    cout << "Parallel bitonic sort:" << endl;
    cout << "Time taken: " << timeTakenParallel << " ms" << endl;
    cout << "Sorted array:" << endl;
    for (int i = 0; i < bitonic_sequence_parallel.size(); i++) {
        cout << bitonic_sequence_parallel[i] << " ";
    }
    cout << endl << endl;

    // Calculate speedup
    double speedup = static_cast<double>(timeTakenSerial) / timeTakenParallel;
    cout << "Speedup: " << speedup << endl;

    return 0;
}