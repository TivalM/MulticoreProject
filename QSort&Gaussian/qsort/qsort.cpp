#include <iostream>
#include <stdio.h>
#include<stdlib.h>



using namespace std;
#define BILLION 1000000000L




 //g++ -o qsort qsort.cpp 
void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
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
 
void quickSort(int* arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
 
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
 
int main(int argc, char **argv)
{
    struct timespec start, end;
    int size;
    if (argc > 1){
        size = strtol(argv[1], NULL, 10);
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    int *arr = (int*)malloc(size*  sizeof(int));
    int i = 0;
    while(i < size){
        arr[i] = rand()%999;
        i++;
    }
    quickSort(arr, 0, size - 1);
    clock_gettime(CLOCK_MONOTONIC,&end);
    double diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("the execution time is %e nanoseconds\n", diff);
    return 0;
}