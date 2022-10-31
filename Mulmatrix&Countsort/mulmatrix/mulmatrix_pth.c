#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>

int N, t;
float ** A, ** B, ** C;

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

void * multiply_matrix(void * arg)
{
    int i, j, k, tid, portion_size, row_start, row_end;
    tid = *(int *) arg; // get the thread id
    portion_size = N / t;
    row_start = tid * portion_size;
    row_end = (tid == t -1) ? N : (tid + 1) * portion_size;

    for (i = row_start; i < row_end; i++)
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
    pthread_exit(NULL);
}



int main(int argc, char *argv[])
{
    int i;
    pthread_t * threads;
    int * tids;
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

    A = (float **) malloc(N * sizeof(float *));
    B = (float **) malloc(N * sizeof(float *));
    C = (float **) malloc(N * sizeof(float *));
    for (i = 0; i < N; i++)
    {
        A[i] = (float *) malloc(N * sizeof(float));
        B[i] = (float *) malloc(N * sizeof(float));
        C[i] = (float *) malloc(N * sizeof(float));
    }
    threads = (pthread_t *) malloc(t * sizeof(pthread_t));
    tids = (int *) malloc(t * sizeof(int));
    
    // generate random matrices A and B
    generate_random_matrix(N, A);
    generate_random_matrix(N, B);

    double start_time, end_time;
    start_time = omp_get_wtime();
    
    for (i = 0; i < t; i++)
    {
        tids[i] = i;
        pthread_create(&threads[i], NULL, multiply_matrix, (void *) &tids[i]);
    }
    for (i = 0; i < t; i++)
    {
        pthread_join(threads[i], NULL);
    }

    end_time = omp_get_wtime();
    printf("N = %d, threads = %d: multiply matrix time is %f s.\n", N, t, end_time - start_time);

    for (i = 0; i < N; i++)
    {
        free(A[i]); free(B[i]); free(C[i]);
    }
    free(A); free(B); free(C);
    free(threads); free(tids);

    return 0;
}
