#!/bin/bash
module load gcc-9.2
g++ -o ./qsort/qsort ./qsort/qsort.cpp 
gcc -fopenmp -Wall -o ./qsort/qsortomp ./qsort/qsortomp.c
gcc ./qsort/qsortpthread.c -o ./qsort/qsortpthread -lpthread 

INPUTSIZE=(10000 100000 1000000 2500000 5000000 7500000 10000000)
# INPUTSIZE=(rand100k rand1m rand10m rand25m)
THREADSNUM=(2 4 8 16 32 64 128)

echo "Exp Start"
for SIZE in ${INPUTSIZE[@]} 
do
    ./qsort/qsort 1 $SIZE >> ./qsort/seq.log
done

for SIZE in ${INPUTSIZE[@]} 
do
    for THREAD in ${THREADSNUM[@]} 
    do
        ./qsort/qsortomp $THREAD $SIZE >> ./qsort/omp.log
    done
done

for SIZE in ${INPUTSIZE[@]} 
do
    for THREAD in ${THREADSNUM[@]} 
    do
        ./qsort/qsortpthread $THREAD $SIZE >> ./qsort/pthread.log
    done
done