//gcc -fopenmp -Wall -o qsortomp qsortomp.c

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define TASK_SIZE 100
#define BILLION 1000000000L

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

unsigned int rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    do
    {
        r = rand();
    } 
    while (r >= limit);

    return min + (r / buckets);
}

void fillupRandomly (int *m, int size, unsigned int min, unsigned int max){
    int i = 0;
    while(i < size){
         m[i] = rand_interval(min, max);
    }
} 



int partition (int* arr, int low, int high)
{
    int pivot = arr[high]; // pivot
    int i = (low - 1); 
    int j = low;
    while(j < high)
    {

        if (arr[j] < pivot)
        {
            i++; 
            swap(&arr[i], &arr[j]);
        }
        j++;
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quicksort(int * a, int p, int r)
{
    int div;

    if(p < r){ 
        div = partition(a, p, r); 
        #pragma omp task shared(a) if(r - p > TASK_SIZE) 
        quicksort(a, p, div - 1); 
        #pragma omp task shared(a) if(r - p > TASK_SIZE)
        quicksort(a, div + 1, r); 
    }
}

int main(int argc, char *argv[])
{
        struct timespec start, end;
        srand(123456);
        int N  = (argc > 1) ? atoi(argv[1]) : 10;
        int numThreads = (argc > 2) ? atoi(argv[2]) : 2;
        int *X = malloc(N * sizeof(int));
        int *tmp = malloc(N * sizeof(int));

        omp_set_dynamic(0);              /** Explicitly disable dynamic teams **/
        omp_set_num_threads(numThreads); /** Use N threads for all parallel regions **/

         // Dealing with fail memory allocation
        if(!X || !tmp)
        { 
           if(X) free(X);
           if(tmp) free(tmp);
           return (EXIT_FAILURE);
        }
        fillupRandomly (X, N-1, 0, 999);
        clock_gettime(CLOCK_MONOTONIC, &start);
        #pragma omp parallel
        {
            #pragma omp single
             quicksort(X, 0, N - 1);
        }
        clock_gettime(CLOCK_MONOTONIC,&end);
        double diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        printf("the execution time is %e nanoseconds\n", diff);

        free(X);
        free(tmp);
        return (EXIT_SUCCESS);
    return 0;
}
