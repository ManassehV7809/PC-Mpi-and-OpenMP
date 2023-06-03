#include <iostream>
#include <algorithm>
#include <cmath>
#include <string.h>
#include <vector>
#include <mpi.h>
#include <chrono>

void alternate(std::vector<int>& bionic_sequence, std::vector<int>& group, bool isAscending, int startIndex) {
    int HALF = group.size() / 2;

    for (int i = 0; i < HALF; i++) {
        int num1 = group[i];
        int num2 = group[i + HALF];

        if (isAscending) {
            if (num1 > num2) {
                std::swap(group[i], group[i + HALF]);
            }
        } else {
            if (num1 < num2) {
                std::swap(group[i], group[i + HALF]);
            }
        }
    }

    for (int i = 0; i < group.size(); i++) {
        bionic_sequence[startIndex] = group[i];
        startIndex++;
    }
}

void step3(std::vector<int>& bionic_sequence, std::vector<int>& group, int startIndex) {
    int HALF = group.size() / 2;

    for (int i = 0; i < group.size(); i += 2) {
        int num1 = group[i];
        int num2 = group[i + 1];
        if (i < HALF) {
            if (num1 > num2) {
                std::swap(group[i], group[i + 1]);
            }
        } else {
            if (num1 < num2) {
                std::swap(group[i], group[i + 1]);
            }
        }
    }

    for (int i = 0; i < group.size(); i++) {
        bionic_sequence[startIndex] = group[i];
        startIndex++;
    }
}

void generateSequence(std::vector<int>& bionic_sequence) {
    int groupSize = 2;
    int HALF = bionic_sequence.size() / 2;

    while (groupSize <= HALF) {
        bool isAscending = true;
        if (groupSize == HALF) {
            std::vector<int> firstHalf(bionic_sequence.begin(), bionic_sequence.begin() + HALF);
            std::vector<int> secondHalf(bionic_sequence.begin() + HALF, bionic_sequence.end());

            step3(bionic_sequence, firstHalf, 0);
            step3(bionic_sequence, secondHalf, HALF);
        }

        for (int i = 0; i < bionic_sequence.size(); i += groupSize) {
            std::vector<int> group(bionic_sequence.begin() + i, bionic_sequence.begin() + i + groupSize);
            alternate(bionic_sequence, group, isAscending, i);
            isAscending = !isAscending;
        }

        groupSize *= 2;
    }

    std::vector<int> leftGroup1(bionic_sequence.begin(), bionic_sequence.begin() + HALF / 2);
    std::vector<int> leftGroup2(bionic_sequence.begin() + HALF / 2, bionic_sequence.begin() + HALF);

    std::vector<int> rightGroup1(bionic_sequence.begin() + HALF, bionic_sequence.begin() + HALF + (HALF / 2));
    std::vector<int> rightGroup2(bionic_sequence.begin() + HALF + (HALF / 2), bionic_sequence.end());

    alternate(bionic_sequence, leftGroup1, true, 0);
    alternate(bionic_sequence, leftGroup2, true, HALF / 2);
    alternate(bionic_sequence, rightGroup1, false, HALF);
    alternate(bionic_sequence, rightGroup2, false, HALF + (HALF / 2));

    for (int i = 0; i < HALF; i += 2) {
        std::vector<int> group{ bionic_sequence[i], bionic_sequence[i + 1] };
        alternate(bionic_sequence, group, true, i);
    }

    for (int i = HALF; i < bionic_sequence.size(); i += 2) {
        std::vector<int> group{ bionic_sequence[i], bionic_sequence[i + 1] };
        alternate(bionic_sequence, group, false, i);
    }
}

void bitonicSort(std::vector<int>& bionic_sequence, int low, int high) {
    int MAX_IT = low + ((high - low + 1) / 2);

    int count = 0;
    for (int i = low; i < MAX_IT; i++) {
        int index = MAX_IT + count;
        int element = bionic_sequence[index];
        int curr = bionic_sequence[i];

        if (curr > element) {
            std::swap(bionic_sequence[i], bionic_sequence[index]);
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

void print(const std::vector<int>& bionic_sequence) {
    for (int i = 0; i < bionic_sequence.size(); i++) {
        std::cout << bionic_sequence[i] << " ";
    }
    std::cout << "\n" << std::endl;
}

int main(int argc, char* argv[]) {
    std::vector<int> bionic_sequence{ 3, 7, 4, 8, 6, 2, 1, 5 };

    generateSequence(bionic_sequence);

    int id, num_processes;
    int half = bionic_sequence.size() / 2;
    int left[half];
    int right[half];
    std::vector<int> tempVector;

    int leftCount = 0;
    int rightCount = 0;
    int arrayCount = 0;

    auto start = std::chrono::high_resolution_clock::now();

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    switch (id) {
        case 0:
            for (int i = 0; i < half; i++) {
                left[i] = bionic_sequence[i];
                right[i] = bionic_sequence[half + i];
            }

            MPI_Send(&left, half, MPI_INT, 1, 1, MPI_COMM_WORLD);
            MPI_Send(&right, half, MPI_INT, 2, 2, MPI_COMM_WORLD);
            start = std::chrono::high_resolution_clock::now();
            break;
        case 1:
            MPI_Recv(&left, half, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int i = 0; i < half; i++) {
                tempVector.push_back(left[i]);
            }

            bitonicSort(tempVector, 0, tempVector.size() - 1);

            for (int i = 0; i < half; i++) {
                left[i] = tempVector[i];
            }

            MPI_Send(&left, half, MPI_INT, 3, 1, MPI_COMM_WORLD);
            break;
        case 2:
            MPI_Recv(&right, half, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int i = 0; i < half; i++) {
                tempVector.push_back(right[i]);
            }

            bitonicSort(tempVector, 0, tempVector.size() - 1);

            for (int i = 0; i < half; i++) {
                right[i] = tempVector[i];
            }

            MPI_Send(&right, half, MPI_INT, 3, 2, MPI_COMM_WORLD);
            break;
        case 3:
            MPI_Recv(&left, half, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&right, half, MPI_INT, 2, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            while (leftCount < half && rightCount < half) {
                if (left[leftCount] < right[rightCount]) {
                    bionic_sequence[arrayCount] = left[leftCount];
                    leftCount++;
                } else {
                    bionic_sequence[arrayCount] = right[rightCount];
                    rightCount++;
                }
                arrayCount++;
            }

            while (leftCount < half) {
                bionic_sequence[arrayCount] = left[leftCount];
                leftCount++;
                arrayCount++;
            }

            while (rightCount < half) {
                bionic_sequence[arrayCount] = right[rightCount];
                rightCount++;
                arrayCount++;
            }

            auto finish = std::chrono::high_resolution_clock::now();
            long long timeTaken = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
            std::cout << "time " << timeTaken << std::endl;

            print(bionic_sequence);
            break;
    }

    MPI_Finalize();

    return 0;
}
