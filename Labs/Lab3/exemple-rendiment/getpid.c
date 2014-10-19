#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define    N    10e6

void main () {

    // Define vars
    unsigned int i;
    pid_t pid;
    struct timeval start, end;

    gettimeofday(&start, NULL); // Get start time

    for(i=0; i < N; i++)
        pid = getpid();

    gettimeofday(&end, NULL); // Get end time

    // Get elapset time seconds
    unsigned long time = (end.tv_sec*1e6 + end.tv_usec) - (start.tv_sec*1e6 + start.tv_usec);

    printf("%lu usec (%lf usec/getpid)\n", time, time/N);
}
