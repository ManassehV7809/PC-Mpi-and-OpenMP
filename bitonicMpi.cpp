bitonicMpi.cpp
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string.h>
#include <vector>
#include <mpi.h>
#include <chrono>

// mpirun --mca shmem posix --oversubscribe -np 10 mpi
// mpicc -o mpi bitonic_mpi.cpp -lstdc++

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

int main(int argc, char* argv[]){

    std::vector<int> bionic_sequence;
    bionic_sequence.push_back(3);
    bionic_sequence.push_back(7);
    bionic_sequence.push_back(4);
    bionic_sequence.push_back(8);
    bionic_sequence.push_back(6);
    bionic_sequence.push_back(2);
    bionic_sequence.push_back(1);
    bionic_sequence.push_back(5);
    
    
    //std::cout << "Random input" << std::endl;
    //print(bionic_sequence);

    generateSequence(bionic_sequence);

    //std::cout << "After generating bitonic sequence" << std::endl;
    //print(bionic_sequence);

    int id, num_processes;
    int half = bionic_sequence.size() / 2;
    int left[half];
    int right[half];
    std::vector<int> tempVector;

    int leftCount = 0;
    int rightCount = 0;
    int arrayCount = 0;

    auto start = std::chrono::high_resolution_clock::now();

    // init MPI
    MPI_Init(&argc, &argv);

    // get Process ID...
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    // get the number of processes...
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    switch(id){
        case 0:
            for(int i = 0; i < half; i++){
                left[i] = bionic_sequence[i];
                right[i] = bionic_sequence[half + i];
            }


            MPI_Send(&left, half, MPI_INT, 1, 1, MPI_COMM_WORLD);
            MPI_Send(&right, half, MPI_INT, 2, 2, MPI_COMM_WORLD);
            start = std::chrono::high_resolution_clock::now();
            break;
        case 1:
            MPI_Recv(&left, half, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for(int i = 0; i < half; i++){
                tempVector.push_back(left[i]);
            }
            
            bitonicSort(tempVector, 0, tempVector.size() - 1);

            for(int i = 0; i < half; i++){
                left[i] = tempVector[i];
            }

            MPI_Send(&left, half, MPI_INT, 3, 1, MPI_COMM_WORLD);
            break;
        case 2:
            MPI_Recv(&right, half, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for(int i = 0; i < half; i++){
                tempVector.push_back(right[i]);
            }
            
            bitonicSort(tempVector, 0, tempVector.size() - 1);

            for(int i = 0; i < half; i++){
                right[i] = tempVector[i];
            }

            MPI_Send(&right, half, MPI_INT, 3, 2, MPI_COMM_WORLD);
            break;
        case 3:
            MPI_Recv(&left, half, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&right, half, MPI_INT, 2, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

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

            auto finish = std::chrono::high_resolution_clock::now();
            long long timeTaken = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
            std::cout << "time " << timeTaken << std::endl;

            print(bionic_sequence);
            break;
    }
    
    
    MPI_Finalize();
    
    return 0;
}