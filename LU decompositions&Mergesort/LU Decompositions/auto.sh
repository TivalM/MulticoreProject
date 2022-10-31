#!/bin/bash
set -x
rm -f ./src/seq_exe_time.log
rm -f ./src/opmp_exe_time.log
rm -f ./src/pthread_exe_time.log

rm -f ./src/seq_scala.log
rm -f ./src/opmp_scala.log
rm -f ./src/pthread_scala.log

rm -f ./src/LuDecSeq
rm -f ./src/LuDecOmp
rm -f ./src/LuDecPthread

module load gcc-9.2
gcc -Wall -O2 -fopenmp -lpthread -o ./src/LuDecSeq ./src/LuDecSeq.c -lm
gcc -Wall -O2 -fopenmp -lpthread -o ./src/LuDecOmp ./src/LuDecOmp.c -lm
gcc -Wall -O2 -fopenmp -lpthread -o ./src/LuDecPthread ./src/LuDecPthread.c -lm

FILENAME=(rand10k rand40k rand640k rand1m rand2_25m rand4m rand6_25m rand9m)
# FILENAME=(rand10k rand40k rand640k rand1m)
THREADSNUM=(2 4 6 8 12 16 20 24 32 48 64)
# THREADSNUM=(2 4 8 16 32 64 128 256)

echo "execute time"
for NAME in ${FILENAME[@]}
do
    ./src/LuDecSeq 1 $NAME >> ./src/seq_exe_time.log
done

for NAME in ${FILENAME[@]}
do
    ./src/LuDecOmp 8 $NAME >> ./src/opmp_exe_time.log
done

for NAME in ${FILENAME[@]}
do
    ./src/LuDecPthread 8 $NAME >> ./src/pthread_exe_time.log
done


echo "scalability"
./src/LuDecSeq 1 rand4m >> ./src/seq_scala.log

for THREAD in ${THREADSNUM[@]}
do
    ./src/LuDecOmp $THREAD rand4m >> ./src/opmp_scala.log
done

for THREAD in ${THREADSNUM[@]}
do
    ./src/LuDecPthread $THREAD rand4m >> ./src/pthread_scala.log
done