#include <iostream>
#include <vector>

void parallelScan(const std::vector<int> &input, std::vector<int> &output)
{
    int size = input.size();

    std::vector<int> prefixSums(size);
    prefixSums[0] = input[0];

    for (int i = 1; i < size; ++i)
    {
        prefixSums[i] = prefixSums[i - 1] + input[i];
    }

    int prefixSumOffset = prefixSums[0];

    for (int i = 0; i < size; ++i)
    {
        output[i] = prefixSums[i] + prefixSumOffset;
    }
}

int main()
{

    std::vector<int> input(11);
    for (int i = 0; i < 11; i++)
    {
        input[i] = i;
    }

    std::vector<int> output(11);

    parallelScan(input, output);

    printf("Prefix Sum: ");
    for (int i = 0; i < 11; ++i)
    {
        printf("%d ", output[i]);
    }

    return 0;
}
