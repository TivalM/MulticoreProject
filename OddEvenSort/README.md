Please use gcc-11.2.

random number generatior random-num.c: 
gcc -o random random-num.c
.\random filename numberCount

Sequential version oddeven.c: 
gcc -g -Wall -fopenmp -o oddeven oddeven.c
.\oddeven threadnum filename

OpenMP version oddevenomp.c: 
gcc -g -Wall -fopenmp -o oddevenomp oddevenomp.c
.\oddevenomp threadnum filename

Pthread version oddevenpthread.c: 
gcc -g -Wall -fopenmp -o oddevenpthread -lpthread oddevenpthread.c
.\oddevenpthread threadnum filename