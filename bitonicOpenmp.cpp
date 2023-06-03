#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

void alternate(std::vector<int>& bionic_sequence, std::vector<int>& group, bool isAscending, int startIndex) {
    int HALF = group.size() / 2;

    for (int i = 0; i < HALF; i++) {
        int num1 = group[i];
        int num2 = group[i + HALF];

        if ((num1 > num2) == isAscending) {
            std::swap(group[i], group[i + HALF]);
        }
    }

    for (int i = 0; i < group.size(); i++) {
        bionic_sequence[startIndex++] = group[i];
    }
}

void generateSequence(std::vector<int>& bionic_sequence) {
    int groupSize = 2;
    int HALF = bionic_sequence.size() / 2;

    while (groupSize <= HALF) {
        bool isAscending = true;

        #pragma omp parallel for
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

void bitonicMerge(std::vector<int>& bionic_sequence, int low, int high) {
    int half = (high - low + 1) / 2;
    std::vector<int> left(bionic_sequence.begin() + low, bionic_sequence.begin() + low + half);
    std::vector<int> right(bionic_sequence.begin() + low + half, bionic_sequence.begin() + high + 1);

    int leftCount = 0;
    int rightCount = 0;
    int arrayCount = low;

    while (leftCount < half && rightCount < half) {
        if (left[leftCount] < right[rightCount]) {
            bionic_sequence[arrayCount++] = left[leftCount++];
        } else {
            bionic_sequence[arrayCount++] = right[rightCount++];
        }
    }

    while (leftCount < half) {
        bionic_sequence[arrayCount++] = left[leftCount++];
    }

    while (rightCount < half) {
        bionic_sequence[arrayCount++] = right[rightCount++];
    }
}

void bitonicSort(std::vector<int>& bionic_sequence, int low, int high) {
    if (high - low >= 1) {
        int mid = low + (high - low) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                bitonicSort(bionic_sequence, low, mid);
            }
            #pragma omp section
            {
                bitonicSort(bionic_sequence, mid + 1, high);
            }
        }

        bitonicMerge(bionic_sequence, low, high);
    }
}

void print(const std::vector<int>& bionic_sequence) {
    for (int i = 0; i < bionic_sequence.size(); i++) {
        std::cout << bionic_sequence[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::vector<int> bionic_sequence{ 19, 0, 12, 5, 8, 3, 7, 9, 11, 22, 25, 18, 4, 10, 17, 15 };
    int half = bionic_sequence.size() / 2;
    std::vector<int> left;
    std::vector<int> right;

    omp_set_num_threads(2);

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        if (id == 0) {
            left = std::vector<int>(bionic_sequence.begin(), bionic_sequence.begin() + half);
            bitonicSort(left, 0, left.size() - 1);
        } else {
            right = std::vector<int>(bionic_sequence.begin() + half, bionic_sequence.end());
            bitonicSort(right, 0, right.size() - 1);
        }
    }

    bionic_sequence.clear();
    bionic_sequence.reserve(left.size() + right.size());
    bionic_sequence.insert(bionic_sequence.end(), left.begin(), left.end());
    bionic_sequence.insert(bionic_sequence.end(), right.begin(), right.end());

    bitonicMerge(bionic_sequence, 0, bionic_sequence.size() - 1);

    print(bionic_sequence);

    return 0;
}
