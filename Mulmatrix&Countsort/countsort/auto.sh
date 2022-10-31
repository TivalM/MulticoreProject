#!/bin/bash
set -x
rm -f ./countsort_seq3.log
rm -f ./countsort_omp3.log
rm -f ./countsort_pth3.log
rm -f ./countsort_seq
rm -f ./countsort_omp
rm -f ./countsort_pth

module load gcc-9.2
gcc -g -Wall -fopenmp -lpthread -o ./countsort_seq ./countsort_seq.c
gcc -g -Wall -fopenmp -lpthread -o ./countsort_omp ./countsort_omp.c
gcc -g -Wall -fopenmp -lpthread -o ./countsort_pth ./countsort_pth.c

RUNS=(1 2 3)
DIMS=(1000 2000 4000 8000 10000 20000 40000 80000 100000)
THREADSNUM=(2 4 6 8 12 16 20 24 32 48 64)

echo "Exp Start"
for IND in ${RUNS[@]}
do
    echo "run " $IND >> ./countsort_seq3.log
    for N in ${DIMS[@]} 
    do
        ./countsort_seq $N 1 >> ./countsort_seq3.log
    done
    echo "" >> ./countsort_seq3.log
done

for IND in ${RUNS[@]}
do
    echo "run " $IND >> ./countsort_omp3.log
    for N in ${DIMS[@]} 
    do
        for THREAD in ${THREADSNUM[@]} 
        do
            ./countsort_omp $N $THREAD >> ./countsort_omp3.log
        done
    done
    echo "" >> ./countsort_omp3.log
done

for IND in ${RUNS[@]}
do
    echo "run " $IND >> ./countsort_pth3.log
    for N in ${DIMS[@]} 
    do
        for THREAD in ${THREADSNUM[@]} 
        do
            ./countsort_pth $N $THREAD >> ./countsort_pth3.log
        done
    done
    echo "" >> ./countsort_pth3.log
done
