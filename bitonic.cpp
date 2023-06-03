#include <iostream>
#include <vector>
#include <chrono>

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

        for (int i = 0; i < bionic_sequence.size(); i += groupSize) {
            std::vector<int> group(bionic_sequence.begin() + i, bionic_sequence.begin() + i + groupSize);
            alternate(bionic_sequence, group, isAscending, i);
            isAscending = !isAscending;
        }

        groupSize *= 2;
    }

    // Split half into 4 pieces
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
    if (high - low > 0) {
        int MAX_IT = low + ((high - low + 1) / 2);
        int count = 0;

        for (int i = low; i < MAX_IT; i++) {
            int index = MAX_IT + count;
            int& curr = bionic_sequence[i];
            int& element = bionic_sequence[index];

            if (curr > element) {
                std::swap(curr, element);
            }

            count++;
        }

        int leftLow = low;
        int leftHigh = MAX_IT - 1;
        int rightLow = MAX_IT;
        int rightHigh = high;

        bitonicSort(bionic_sequence, leftLow, leftHigh);
        bitonicSort(bionic_sequence, rightLow, rightHigh);
    }
}

void print(const std::vector<int>& bionic_sequence) {
    for (int i = 0; i < bionic_sequence.size(); i++) {
        std::cout << bionic_sequence[i] << " ";
    }
    std::cout << "\n" << std::endl;
}

int main() {
    std::vector<int> bionic_sequence{ 3, 7, 4, 8, 6, 2, 1, 5 };

    std::cout << "Random input" << std::endl;
    print(bionic_sequence);

    generateSequence(bionic_sequence);

    std::cout << "After generating bitonic sequence" << std::endl;
    print(bionic_sequence);

    auto start = std::chrono::high_resolution_clock::now();
    bitonicSort(bionic_sequence, 0, bionic_sequence.size() - 1);
    auto finish = std::chrono::high_resolution_clock::now();
    long long timeTaken = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
    std::cout << "Time: " << timeTaken << std::endl;

    std::cout << "Sorted array" << std::endl;
    print(bionic_sequence);

    return 0;
}
