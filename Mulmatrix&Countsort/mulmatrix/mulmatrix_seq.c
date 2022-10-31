#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>


void generate_random_matrix(int N, float ** mat)
{
    int i = 0, j = 0;
    srand (time(NULL));
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            mat[i][j] = (float) (rand() % 10) * 0.1;
        }
    }
}

void multiply_matrix(int N, int t, float ** A, float ** B, float ** C)
{
    int i, j, k;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            C[i][j] = 0;
            for (k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}



int main(int argc, char *argv[])
{
    int N, t, i;

    // read the input from the command line
    if (argc != 3)
    {
        printf("usage: ./matrix_multiply N t\n");
        printf("N: a positive integer, N-by-N is the size of matrices\n");
        printf("t: number of threads and is a positive interger\n");
        exit(1);
    }

    N = atoi(argv[1]);
    t = atoi(argv[2]);

    float ** A = (float **) malloc(N * sizeof(float *));
    float ** B = (float **) malloc(N * sizeof(float *));
    float ** C = (float **)  malloc(N * sizeof(float *));
    for (i = 0; i < N; i++)
    {
        A[i] = (float *) malloc(N * sizeof(float));
        B[i] = (float *) malloc(N * sizeof(float));
        C[i] = (float *) malloc(N * sizeof(float));
    }

    // generate random matrices A and B
    generate_random_matrix(N, A);
    generate_random_matrix(N, B);

    double start_time, end_time;
    start_time = omp_get_wtime();
    multiply_matrix(N, t, A, B, C);
    end_time = omp_get_wtime();
    printf("N = %d, threads = %d: multiply matrix time is %f s.\n", N, t, end_time - start_time);

    for (i = 0; i < N; i++)
    {
        free(A[i]); free(B[i]); free(C[i]);
    }
    free(A); free(B); free(C);

    return 0;
}
