#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <math.h>
void LUDec(double**, double**, double**, int, int);
void resultTest(double**, double**, double**, int);

// int main(int argc, char * argv[]){
int main(int argc, char * argv[]){
    if(argc != 3){
        fprintf(stderr, "wrong parameters\n");
        exit(1);
    }
    double tstart, tend, ttaken;
    int threadNum = atoi(argv[1]);
    // int threadNum = 1;
    FILE * fp;
    char* fileName =  argv[2];
    // char* fileName =  "./rand100";
    if((fp = fopen(fileName, "r")) == NULL){
        printf("Cannot open the file %s\n", fileName);
        exit(1);
    }
    int count;
    fscanf(fp,"%d ", &count);
    int N = sqrt(count);
    if (N*N != count){
        printf("Need to be a square matrix\n");
        exit(1);
    }
    double** inputMat =(double**)calloc(N,sizeof(double*));
    double** upperMat =(double**)calloc(N,sizeof(double*));
    double** lowerMat =(double**)calloc(N,sizeof(double*));
    if (!inputMat || !upperMat || !lowerMat){
        printf("Cannot allocate array.\n");
        exit(1);
    }
    for (int i = 0; i < N; i++){
        inputMat[i]=(double*)calloc(N, sizeof(double));
        upperMat[i]=(double*)calloc(N, sizeof(double));
        lowerMat[i]=(double*)calloc(N, sizeof(double));
        if (!inputMat[i] || !upperMat[i] || !lowerMat[i]){
        printf("Cannot allocate array.\n");
        exit(1);
        }
    }
    
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            fscanf(fp,"%lf ", &inputMat[i][j]);
            if (i == j){
                lowerMat[i][j] = 1;
            }
        }
    }
    fclose(fp);
    // for (int i = 0; i < N; i++){
    //     for (int j = 0; j < N; j++){
    //         printf("%lf ", inputMat[i][j]);
    //     }
    //     printf("\n");
    // }
    printf("\n");
    tstart = omp_get_wtime();
    LUDec(inputMat, lowerMat, upperMat, N, threadNum);
    tend = omp_get_wtime();
    ttaken = tend - tstart;

    printf("With data size %d, threadnum %d\nTime consumption total: %lf\n", count, threadNum, ttaken);
    // for (int i = 0; i < N; i++){
    //     for (int j = 0; j < N; j++){
    //         printf("%lf ", lowerMat[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    // for (int i = 0; i < N; i++){
    //     for (int j = 0; j < N; j++){
    //         printf("%lf ", upperMat[i][j]);
    //     }
    //     printf("\n");
    // }
    // for (int i = 0; i < count-1; i++){
    //     if(array[i] > array[i+1]){
    //         printf("!!!error detected!!!\n");
    //         exit(-1);
    //     }
    // }
    // resultTest(inputMat, lowerMat, upperMat, N);
    // printf("\n");
    
    for (int i = 0; i < N; i++){
        free(inputMat[i]);
        free(upperMat[i]);
        free(lowerMat[i]);
    }
    free(inputMat);
    free(upperMat);
    free(lowerMat);
}

void LUDec(double** input, double** lower, double** upper, int n, int threadNum){
    //Doolittle's Algorithm
    #pragma omp parallel num_threads(threadNum)
    for (int k = 0; k < n; k++){
        #pragma omp for
        for (int m = k; m < n; m++){
            long double sum = 0;
            for (int j = 0; j < k; j++){
                sum += lower[k][j]*upper[j][m];
            }
            upper[k][m] = input[k][m] - sum;
        }
        #pragma omp for
        for (int i = k+1; i < n; i++){
            long double sum = 0;
            for (int j = 0; j < k; j++){
                sum += lower[i][j]*upper[j][k];
            }
            lower[i][k] = (input[i][k] - sum)/upper[k][k];
        }
    }
}

void resultTest(double** input, double** lower, double** upper, int n){
    printf("start checking...\n");
    // for (int i = 0; i < n; i++){
    //     for (int j = 0; j < n; j++){
    //         printf("%lf ", lower[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    // for (int i = 0; i < n; i++){
    //     for (int j = 0; j < n; j++){
    //         printf("%lf ", upper[i][j]);
    //     }
    //     printf("\n");
    // }
    double epsilon = 0.00001f;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double result = 0;
 
            for (int k = 0; k < n; k++) {
                result += lower[i][k] * upper[k][j];
            }
            if(fabs(result - input[i][j]) > epsilon){
                printf("!!!error detected!!!\n");
                printf("result != input on i: %d, j: %d: %lf, %lf", i,j,result,input[i][j]);
                exit(-1);
            }
        }
    }
    printf("checking end\n");
}