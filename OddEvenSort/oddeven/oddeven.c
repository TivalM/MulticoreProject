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

void odd_even_sort(int* a, int n){
    int phase, i, temp;
    for(phase = 0; phase < n; phase++){
        if(phase % 2==0) //even phase{
            for(i = 1; i < n; i += 2){
                if(a[i-1] > a[i]){
                    temp = a[i];
                    a[i] = a[i-1];
                    a[i-1] = temp;
                }
            }
        }
        else //odd phase{
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
    char *fileName = argv[1];
    if (argc != 2){
        printf("Error: invalid arguments\n\n");
        printf("usage: %s filename\n", executable);
        printf("positional arguments:\n");
        printf("  filename   the name of the text file that contains the integers\n");
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

    odd_even_sort(nums, numCount);

    tend = omp_get_wtime();
    ttaken = tend - tstart;
    printf("Time take for the main part: %f\n", ttaken);
    /** Test Time End **/

    self_test(initial, nums, numCount);
    if(TEST){
        dump_array(nums, numCount);
    }
}