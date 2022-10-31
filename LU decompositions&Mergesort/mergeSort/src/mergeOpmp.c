#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <math.h>

void mergeSort(int*, int, int, int*);
void merge(int*, int , int , int , int*);
void mergeSubarrays(int*, int, int, int, int*);

int main(int argc, char * argv[]){
    if(argc != 3){
        fprintf(stderr, "wrong parameters\n");
        exit(1);
    }
    double tstart, tend, ttaken;
    int threadNum = atoi(argv[1]);
    FILE * fp;
    char* fileName =  argv[2];
    if((fp = fopen(fileName, "r")) == NULL){
        printf("Cannot open the file %s\n", fileName);
        exit(1);
    }
    int count;
    fscanf(fp,"%d ", &count);
    int* array = (int*)calloc(count, sizeof(int));
    if (!array){
        printf("Cannot allocate array.\n");
        exit(1);
    }
    int* temp = (int*)calloc(count, sizeof(int));
    if (!temp){
        printf("Cannot allocate array temp.\n");
        exit(1);
    }
    for (int i = 0; i < count; i++){
        fscanf(fp,"%d ", &array[i]);
    }
    fclose(fp);

    tstart = omp_get_wtime();
    int gap = count/threadNum;
    #pragma omp parallel num_threads(threadNum)
    {
        #pragma omp single
        {
            int start = 0;
            int end = start + gap;
            for (int i = 0; i < threadNum; i++){
                #pragma omp task firstprivate(start, end)
                mergeSort(array, start, end-1, temp);
                // printf("start %d, end %d\n", start, end);
                start += gap;
                end = (i == threadNum-2)?count:start+gap;
            }
        }
    }
    mergeSubarrays(array, threadNum, gap, count, temp);
    tend = omp_get_wtime();
    ttaken = tend - tstart;
    printf("With data size %d, threadnum %d\nTime consumption total: %lf\n", count, threadNum, ttaken);
    // printf("start checking...\n");
    // for (int i = 0; i < 10; i++){
    //     printf("%d ", array[i]);
    // }
    // printf("\n");
    // for (int i = 0; i < count-1; i++){
    //     if(array[i] > array[i+1])
    //         printf("error detected\n");
    // }
    // printf("checking end\n");
    // printf("\n");
    
    free(array);
    free(temp);
}

void mergeSort(int* arr, int left, int right, int* temp){
    if(left < right){
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, temp);
        mergeSort(arr, mid + 1, right, temp);
  
        merge(arr, left, mid, right, temp);
    }
}

void merge(int* arr, int left, int mid, int right, int* temp){
    int i = left;
    int j = mid+1;
    int index = left;
    
    while (i<=mid && j<=right){
        if (arr[i] < arr[j]){
            temp[index++] = arr[i++];
        }else{
            temp[index++] = arr[j++];
        }
    }
    while (i<=mid) {
        temp[index++] = arr[i++];
    }
    
    while (j<=right) { 
        temp[index++] = arr[j++];
    }
    memcpy(arr+left, temp+left, (right-left+1)*sizeof(int));
}

void mergeSubarrays(int* arr, int subarraysNum, int gapSize, int count, int* temp){
    // for (int i = 0; i < count; i++){
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");
    for (int i = 0; i < subarraysNum; i += 2){
        int left = i*gapSize;
        int right = (i == subarraysNum-1)? count-1: left+2*gapSize-1;
        int mid = left + gapSize-1;
        merge(arr, left, mid, right, temp);
    }
    if (subarraysNum / 2 >= 1) {
        mergeSubarrays(arr, subarraysNum / 2, gapSize*2, count, temp);
    }
}