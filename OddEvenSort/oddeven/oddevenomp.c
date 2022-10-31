#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include <omp.h>
#define TEST 0

void dump_array(int* a, int size){
    int i;
    for(i = 0; i < size; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}

void odd_even_sort(int* a, int n, int threadNumber){
    int phase, i, temp;
#pragma omp parallel num_threads(threadNumber) default(none) shared(a,n) private(i,temp,phase)
    {
        for(phase = 0; phase < n; phase++)
        {
            if(phase % 2==0) //even phase
            {
#pragma omp for
                for(i = 1; i < n; i += 2){
                    if(a[i-1] > a[i]){
                        temp = a[i];
                        a[i] = a[i-1];
                        a[i-1] = temp;
                    }
                }
            }
            else //odd phase{
#pragma omp for
                for(i = 1; i < n-1; i += 2){
                    if(a[i] > a[i+1]){
                        temp = a[i];
                        a[i] = a[i+1];
                        a[i+1] = temp;
                    }
                }
            }
        }
    }
}

int compare_integers(const void* pInt1, const void* pInt2){
    int a = *((int*)pInt1);
    int b = *((int*)pInt2);
    return (a < b) ? -1 : (a == b) ? 0 : 1;
}

void self_test(int* initial, int* sorted, int length){
    int i;
    qsort(initial, length, sizeof(int), compare_integers);
    for(i = 0; i < length; i++){
        if(initial[i] != sorted[i]){
            printf("[Error] Array is not sorted\n");
            return;
        }
    }
    printf("[OK] Array is sorted\n");
}


int main(int argc, char** argv){
    double tstart = 0.0, tend = 0.0, ttaken;
    char *executable = argv[0];
    char *tStr = argv[1];
    char *fileName = argv[2];
    if (argc != 3){
        printf("Error: invalid arguments\n\n");
        printf("usage: %s t filename\n", executable);
        printf("positional arguments:\n");
        printf("  t          the number of threads\n");
        printf("  filename   the name of the text file that contains the integers\n");
        return 1;
    }

    int t = atoi(tStr);
    if(t < 0){
        printf("Error: invalid N %s\n", tStr);
        return 1;
    }

    // Open input file
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL){
        printf("Error: cannot create file %s\n", fileName);
        return 1;
    }
    // Read num count in input file
    int numCount = 0;
    fscanf(fp, "%d", &numCount);

    // Read in numbers
    int *nums = (int *)malloc(numCount * sizeof(int));
    for (int i = 0; i < numCount; i++){
        fscanf(fp, "%d", &nums[i]);
    }
    fclose(fp);

    int *initial = (int *)malloc(numCount * sizeof(int));
    memcpy(initial, nums, numCount * sizeof(int));

    /** Test Time **/
    tstart = omp_get_wtime();

    odd_even_sort(nums, numCount, t);

    tend = omp_get_wtime();
    ttaken = tend - tstart;
    printf("Time take for the main part: %f\n", ttaken);
    /** Test Time End **/

    self_test(initial, nums, numCount);
    if(TEST){
        dump_array(nums, numCount);
    }
}