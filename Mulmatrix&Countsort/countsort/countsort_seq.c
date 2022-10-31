#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>


void generate_random_array(int N, int * arr)
{
    int i = 0;
    srand (time(NULL));
    for (i = 0; i < N; i++)
    {
        arr[i] = rand();
    }
}

void count_sort(int N, int t, int * arr)
{
    int i, j, count = 0;
    int * tmp = malloc(N * sizeof(int));
    for (i = 0; i < N; i++)
    {
        count = 0;
        for (j = 0; j < N; j++)
        {
            if (arr[j] < arr[i])
            {
                count++;
            }
            else if (arr[j] == arr[i] && j < i)
            {
                count++;
            }
        }
        tmp[count] = arr[i];
    }

    for (i = 0; i < N; i++)
    {
        arr[i] = tmp[i];
    }
    free(tmp);
}



int main(int argc, char *argv[])
{
    int N, t;

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

    int * a = (int *) malloc(N * sizeof(int));

    // generate random array a
    generate_random_array(N, a);

    double start_time, end_time;
    start_time = omp_get_wtime();
    count_sort(N, t, a);
    end_time = omp_get_wtime();
    printf("N = %d, threads = %d: count sort time is %f s.\n", N, t, end_time - start_time);

    free(a);

    return 0;
}
