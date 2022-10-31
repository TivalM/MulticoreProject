#!/bin/bash
set -x
rm -f ./mulmatrix_seq3.log
rm -f ./mulmatrix_omp3.log
rm -f ./mulmatrix_pth3.log
rm -f ./mulmatrix_seq
rm -f ./mulmatrix_omp
rm -f ./mulmatrix_pth

module load gcc-9.2
gcc -g -Wall -fopenmp -lpthread -o ./mulmatrix_seq ./mulmatrix_seq.c
gcc -g -Wall -fopenmp -lpthread -o ./mulmatrix_omp ./mulmatrix_omp.c
gcc -g -Wall -fopenmp -lpthread -o ./mulmatrix_pth ./mulmatrix_pth.c

RUNS=(1 2 3)
DIMS=(100 200 400 600 800 1000 1200 1400 1600)
THREADSNUM=(2 4 6 8 12 16 20 24 32 48 64)

echo "Exp Start"
for IND in ${RUNS[@]}
do
    echo "run " $IND >> ./mulmatrix_seq3.log
    for N in ${DIMS[@]} 
    do
        ./mulmatrix_seq $N 1 >> ./mulmatrix_seq3.log
    done
    echo "" >> ./mulmatrix_seq3.log
done

for IND in ${RUNS[@]}
do
    echo "run " $IND >> ./mulmatrix_omp3.log
    for N in ${DIMS[@]} 
    do
        for THREAD in ${THREADSNUM[@]} 
        do
            ./mulmatrix_omp $N $THREAD >> ./mulmatrix_omp3.log
        done
    done
    echo "" >> ./mulmatrix_omp3.log
done

for IND in ${RUNS[@]}
do
    echo "run " $IND >> ./mulmatrix_pth3.log
    for N in ${DIMS[@]} 
    do
        for THREAD in ${THREADSNUM[@]} 
        do
            ./mulmatrix_pth $N $THREAD >> ./mulmatrix_pth3.log
        done
    done
    echo "" >> ./mulmatrix_pth3.log
done
