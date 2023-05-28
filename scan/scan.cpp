#include <omp.h>
#include <stdio.h>

void prefix_sum_omp(int *array, int n, int *output)
{
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int start = tid * (n / omp_get_num_threads());
        int end = (tid + 1) * (n / omp_get_num_threads()) - 1;

        if (start < n)
        {
            output[start] = array[start];
            for (int i = start + 1; i <= end; i++)
            {
                output[i] = output[i - 1] + array[i];
            }
        }
    }
}

int main()
{
    int *array = malloc(sizeof(int) * 1000000);
    for (int i = 0; i < 1000000; i++)
    {
        array[i] = i;
    }

    int *output = malloc(sizeof(int) * 1000000);

    prefix_sum_omp(array, 1000000, output);

    for (int i = 0; i < 1000000; i++)
    {
        if (output[i] != i)
        {
            return 1;
        }
    }

    printf("The parallel version of the prefix sum algorithm is correct!\n");

    free(array);
    free(output);

    return 0;
}
