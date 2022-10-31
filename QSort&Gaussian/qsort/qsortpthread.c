#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include <sys/time.h>
#define BILLION 1000000000L


//gcc qsortpthread.c -o qsortpthread -lpthread 


clock_t start_time, end_time; /* start and end times */

typedef struct qsort_starter
{
  int *arr;
  int low;
  int high;
  int depthOfThreadCreation;
} quickSort_parameters;

void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}


int partition (int *arr, int low, int high, int pivot)
{
	int pivotValue = arr[pivot];
  swap(&arr[pivot],&arr[high]); // pivot
	int s = low; // Index of smaller element
  int i  = low;
	while(i < high)
	{
		// If current element is smaller than or
		// equal to pivot
		if (arr[i] <= pivotValue)
		{
			swap(&arr[i], &arr[s]);
      s++; // increment index of smaller element
		}
    i++;
	}
	swap(&arr[s], &arr[high]);
	return s;
}



void concurrent_quickSort(int *arr, int low, int high, int depthOfThreadCreation);

void* worker_quickSort(void * initialValues){
  quickSort_parameters* parameters = initialValues;
  concurrent_quickSort(parameters->arr, parameters->low, parameters->high,parameters->depthOfThreadCreation);
  return NULL;
}



void concurrent_quickSort(int *arr, int low, int high, int depthOfThreadCreation){
    if (low < high){

          int pivotPos = low + (high - low)/2;
          pivotPos = partition(arr, low, high, pivotPos);
          pthread_t thread;


        if (depthOfThreadCreation > 0){
//          printf("Am i here??");
            quickSort_parameters thread_param = {arr, low, pivotPos-1, depthOfThreadCreation - 1};
            int result;
            //Creating the thread  by calling the pthread_create for the left part recursively
            result = pthread_create(&thread, NULL, worker_quickSort, &thread_param);

            //calling recursively for second time for the right part (for the same thread)
            concurrent_quickSort(arr, pivotPos+1, high, depthOfThreadCreation - 1);


            pthread_join(thread, NULL);

        } else{
            //printf("\n");
            //printf("I am here\n");
            concurrent_quickSort(arr, low, pivotPos-1, depthOfThreadCreation);
            concurrent_quickSort(arr, pivotPos+1, high, depthOfThreadCreation);
         }

    }

}

int main(int argc, char **argv){

  struct timespec start, end;

    int depthOfThreadCreation = 0;
    if (argc > 1){
        depthOfThreadCreation = strtol(argv[1], NULL, 10);
    }

    int size = 8;

    if (argc > 2){
        size = strtol(argv[2], NULL, 10);
    }

    int *arrayElements = malloc(size*  sizeof(long));
    int i = 0;
    while(i < size){
        arrayElements[i] = rand()%999;
        i++;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    concurrent_quickSort(arrayElements, 0, size-1, depthOfThreadCreation);
    clock_gettime(CLOCK_MONOTONIC,&end);

    double diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("the execution time is %e nanoseconds\n", diff);

    printf("\n");

     free(arrayElements);
     return 0;
}