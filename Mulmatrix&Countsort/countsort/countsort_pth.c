#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>

int N, t;
int * a;

void generate_random_array(int N, int * arr)
{
    int i = 0;
    srand (time(NULL));
    for (i = 0; i < N; i++)
    {
        arr[i] = rand();
    }
}

void * count_sort(void * arg)
{
    int * tmp = malloc(N * sizeof(int));
    
    int i, j, count = 0, tid, portion_size, row_start, row_end;
    tid = *(int *) arg; // get the thread id
    portion_size = N / t;
    row_start = tid * portion_size;
    row_end = (tid == t -1) ? N : (tid + 1) * portion_size;
    
    for (i = row_start; i < row_end; i++)
    {
        count = 0;
        for (j = 0; j < N; j++)
        {
            if (a[j] < a[i])
            {
                count++;
            }
            else if (a[j] == a[i] && j < i)
            {
                count++;
            }
        }
        tmp[count] = a[i];
    }
    for (i = 0; i < N; i++)
    {
        a[i] = tmp[i];
    }
    free(tmp);
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
        printf("usage: ./countsort N t\n");
        printf("N: a positive integer, N is the size of array\n");
        printf("t: number of threads and is a positive interger\n");
        exit(1);
    }

    N = atoi(argv[1]);
    t = atoi(argv[2]);

    a = (int *) malloc(N * sizeof(int));
    threads = (pthread_t *) malloc(t * sizeof(pthread_t));
    tids = (int *) malloc(t * sizeof(int));

    // generate random array a
    generate_random_array(N, a);

    double start_time, end_time;
    start_time = omp_get_wtime();
    
    for (i = 0; i < t; i++)
    {
        tids[i] = i;
        pthread_create(&threads[i], NULL, count_sort, (void *) &tids[i]);
    }
    for (i = 0; i < t; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    end_time = omp_get_wtime();
    printf("N = %d, threads = %d: count sort time is %f s.\n", N, t, end_time - start_time);

    free(a); free(threads); free(tids);

    return 0;
}
