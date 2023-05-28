#include <stdio.h>
#include <omp.h>

int main()
{

#pragma omp parallel
    {
        printf("hello world");
    }

    return 0;
}