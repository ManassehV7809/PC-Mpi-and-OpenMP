#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <omp.h>

void alternate(std::vector<int> &bionic_sequence, std::vector<int> group, bool isAccending, int startIndex){
    int HALF = group.size() / 2;

    for(int i = 0; i < HALF; i++){
        int num1 = group[i];
        int num2 = group[i + HALF];

        if(isAccending){
            if(num1 > num2){
                group[i] = num2;
                group[i + HALF] = num1;
            }
        }else{
            if(num1 < num2){
                group[i] = num2;
                group[i + HALF] = num1;
            }
        }
    }

    for(int i = 0; i < group.size(); i++){
        bionic_sequence[startIndex] = group[i];
        startIndex++;
    }
}

void step3(std::vector<int> &bionic_sequence, std::vector<int> group, int startIndex){
    int HALF = group.size() / 2;

    for(int i = 0; i < group.size(); i = i + 2){
        int num1 = group[i];
        int num2 = group[i + 1];
        if(i < HALF){
            if(num1 > num2){
                group[i] = num2;
                group[i + 1] = num1;
            }
        }else{
            if(num1 < num2){
                group[i] = num2;
                group[i + 1] = num1;
            }
        }
    }

    for(int i = 0; i < group.size(); i++){
        bionic_sequence[startIndex] = group[i];
        startIndex++;
    }
}

void generateSequence(std::vector<int> &bionic_sequence){
    int groupSize = 2;
    int HALF = bionic_sequence.size() / 2;

    while(groupSize <= HALF){
        bool isAccending = true;
        if(groupSize == HALF){
            std::vector<int> firtsHalf;
            std::vector<int> secondHalf;
            for(int i = 0; i < HALF; i++){
                firtsHalf.push_back(bionic_sequence[i]);
            }for(int i = HALF; i < bionic_sequence.size(); i++){
                secondHalf.push_back(bionic_sequence[i]);
            }

            step3(bionic_sequence, firtsHalf, 0);
            step3(bionic_sequence, secondHalf, HALF);
        }

        for(int i = 0; i < bionic_sequence.size(); i = i + groupSize){
            std::vector<int> group;
            for(int j = i; j < i + groupSize; j++){
                group.push_back(bionic_sequence[j]);
            }
            alternate(bionic_sequence, group, isAccending, i);
            isAccending = !isAccending;
        }
    
        groupSize = groupSize * 2;
    }

    // split half into 4 pieces

    std::vector<int> leftGroup1;
    std::vector<int> leftGroup2;

    for(int i = 0; i < HALF / 2; i++){
        leftGroup1.push_back(bionic_sequence[i]);
    }for(int i = HALF / 2; i < HALF; i++){
        leftGroup2.push_back(bionic_sequence[i]);
    }

    std::vector<int> rightGroup1;
    std::vector<int> rightGroup2;

    for(int i = HALF; i < HALF + (HALF / 2); i++){
        rightGroup1.push_back(bionic_sequence[i]);
    }for(int i = HALF + (HALF / 2); i < bionic_sequence.size(); i++){
        rightGroup2.push_back(bionic_sequence[i]);
    }

    alternate(bionic_sequence, leftGroup1, true, 0);
    alternate(bionic_sequence, leftGroup2, true, HALF / 2);
    alternate(bionic_sequence, rightGroup1, false, HALF);
    alternate(bionic_sequence, rightGroup2, false, HALF + (HALF / 2));

    for(int i = 0; i < HALF; i = i + 2){
        std::vector<int> group;
        group.push_back(bionic_sequence[i]);
        group.push_back(bionic_sequence[i + 1]);
        alternate(bionic_sequence, group, true, i);
    }

    for(int i = HALF; i < bionic_sequence.size(); i = i + 2){
        std::vector<int> group;
        group.push_back(bionic_sequence[i]);
        group.push_back(bionic_sequence[i + 1]);
        alternate(bionic_sequence, group, false, i);
    }

}
    
void bitonicSort(std::vector<int> &bionic_sequence, int low, int high){
    // split into half...
    // number of iterations to do...
    int MAX_IT = low + ((high - low + 1) / 2);
    
    int count = 0;
    for(int i = low; i < MAX_IT; i++){
        // element to campare to on other half of the list...
        int index = MAX_IT + count;
        int element = bionic_sequence[index];
        int curr = bionic_sequence[i];

        if(curr > element){
            bionic_sequence[i] = element;
            bionic_sequence[index] = curr;
        }
        count++;
    }

    // indeces for the next iteration...
    int leftLow = low;
    int leftHigh = MAX_IT - 1;

    int rightLow = MAX_IT;
    int rightHigh = high;

    if(leftHigh - leftLow > 0){
        bitonicSort(bionic_sequence, leftLow, leftHigh);
    }

    if(rightHigh - rightLow > 0){
        bitonicSort(bionic_sequence, rightLow, rightHigh);
    }
}

void print(std::vector<int> bionic_sequence){
    for(int i = 0; i < bionic_sequence.size(); i++){
            std::cout << bionic_sequence[i] << " ";
        }
        std::cout << "\n" << std::endl;
}


int main(){

    std::vector<int> bionic_sequence;
    bionic_sequence.push_back(19);
    bionic_sequence.push_back(0);
    bionic_sequence.push_back(12);
    bionic_sequence.push_back(5);
    bionic_sequence.push_back(8);
    bionic_sequence.push_back(3);
    bionic_sequence.push_back(7);
    bionic_sequence.push_back(9);
    bionic_sequence.push_back(11);
    bionic_sequence.push_back(22);
    bionic_sequence.push_back(25);
    bionic_sequence.push_back(18);
    bionic_sequence.push_back(4);
    bionic_sequence.push_back(10);
    bionic_sequence.push_back(17);
    bionic_sequence.push_back(15);
    
    // std::cout << "Random input" << std::endl;
    // print(bionic_sequence);

    generateSequence(bionic_sequence);

    // std::cout << "After generating bitonic sequence" << std::endl;
    // print(bionic_sequence);

    // bitonicSort(bionic_sequence, 0, bionic_sequence.size() - 1);
    // std::cout << "Sorted array" << std::endl;
    // print(bionic_sequence);

    int half = bionic_sequence.size() / 2;
    std::vector<int> left;
    std::vector<int> right;

    omp_set_num_threads(2);

    #pragma omp parallel
    {

        int id = omp_get_thread_num();
        if(id == 0){
            for(int i = 0; i < half; i++){
                left.push_back(bionic_sequence[i]);
            }
            bitonicSort(left, 0, left.size() - 1);
        }else{
            for(int i = half; i < bionic_sequence.size(); i++){
                right.push_back(bionic_sequence[i]);
            }
            bitonicSort(right, 0, right.size() - 1);
        }
    }

    // MERGE
    int leftCount = 0;
    int rightCount = 0;
    int arrayCount = 0;


    while(leftCount < half && rightCount < half){
        if(left[leftCount] < right[rightCount]){
            bionic_sequence[arrayCount] = left[leftCount];
            leftCount++;
        }else{
            bionic_sequence[arrayCount] = right[rightCount];
            rightCount++;
        }
        arrayCount++;
    }

    while(leftCount < half){
        bionic_sequence[arrayCount] = left[leftCount];
        leftCount++;
        arrayCount++;
    }

    while(rightCount < half){
        bionic_sequence[arrayCount] = right[rightCount];
        rightCount++;
        arrayCount++;
    }

    print(bionic_sequence);
    
    return 0;
}