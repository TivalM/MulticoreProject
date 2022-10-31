#!/bin/bash
set -x
rm -f ./src/seq_exe_time.log
rm -f ./src/opmp_exe_time.log
rm -f ./src/pthread_exe_time.log

rm -f ./src/seq_scala.log
rm -f ./src/opmp_scala.log
rm -f ./src/pthread_scala.log

rm -f ./src/mergesortSeq
rm -f ./src/mergeOpmp
rm -f ./src/mergePthread

module load gcc-9.2
gcc -Wall -O2 -fopenmp -lpthread -o ./src/mergesortSeq ./src/mergesortSeq.c -lm
gcc -Wall -O2 -fopenmp -lpthread -o ./src/mergeOpmp ./src/mergeOpmp.c -lm
gcc -Wall -O2 -fopenmp -lpthread -o ./src/mergePthread ./src/mergePthread.c -lm

FILENAME=(rand10k rand100k rand1m rand10m rand25m rand50m rand75m rand100m)
# FILENAME=(rand10k rand100k rand1m rand10m)
THREADSNUM=(2 4 6 8 12 16 20 24 32 48 64)
# THREADSNUM=(2 4 8 16 32 64 128 256)

echo "execute time"
for NAME in ${FILENAME[@]}
do
    ./src/mergesortSeq 1 $NAME >> ./src/seq_exe_time.log
done

for NAME in ${FILENAME[@]}
do
    ./src/mergeOpmp 8 $NAME >> ./src/opmp_exe_time.log
done

for NAME in ${FILENAME[@]}
do
    ./src/mergePthread 8 $NAME >> ./src/pthread_exe_time.log
done


echo "scalability"
./src/mergesortSeq 1 rand50m >> ./src/seq_scala.log

for THREAD in ${THREADSNUM[@]}
do
    ./src/mergeOpmp $THREAD rand50m >> ./src/opmp_scala.log
done

for THREAD in ${THREADSNUM[@]}
do
    ./src/mergePthread $THREAD rand50m >> ./src/pthread_scala.log
done