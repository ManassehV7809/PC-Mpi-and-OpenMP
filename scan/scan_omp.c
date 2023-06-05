#include <omp.h>
#include <stdio.h>

void prefix_sum_omp(int *array, int n, int *output)
{
    int *sum = malloc(sizeof(int) * n);
    sum[0] = array[0];
    for (int i = 1; i < n; i++)
    {
        sum[i] = sum[i - 1] + array[i];
    }

#pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        output[i] = sum[i];
    }
    free(sum);
}

int main()
{
    int *array = malloc(sizeof(int) * 11);
    for (int i = 0; i < 11; i++)
    {
        array[i] = i;
    }

    int *output = malloc(sizeof(int) * 11);

    prefix_sum_omp(array, 11, output);

    for (int i = 0; i < 11; i++)
    {
        printf("%d ", output[i]);
    }
    printf("\n");
    free(array);
    free(output);

    return 0;
}
