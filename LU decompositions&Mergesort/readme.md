This folder contains serial and parallel implement of LU Decompositions and Mergesort

1. Use randgen.c to generate test file
    gcc -g -Wall -o ./randgen ./randgen.c -lm
    ./randgen outputFlieName numberCount
2. Put all test data under the same folder with randgen.c source code
3. Modify the auto.sh, control the test flow by setting FILENAME and THREADSNUM
4. ./auto to start the test