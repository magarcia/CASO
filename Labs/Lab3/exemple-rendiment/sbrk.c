#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define    N    10e6

void main () {

    // Define vars
    unsigned int i;
    void *p;
    struct timeval start, end;

    gettimeofday(&start, NULL); // Get start time

    for(i=0; i < N; i++) {
        p = sbrk((intptr_t) 0);

        if (p == (void *) -1) {
            printf("Error on execute sbrk");
            exit(1);
        }
    }

    gettimeofday(&end, NULL); // Get end time

    // Get elapset time seconds
    unsigned long t = (end.tv_sec*1e6 + end.tv_usec) - (start.tv_sec*1e6 + start.tv_usec);

    printf("%lu usec (%lf usec/sbrk)\n", time, time/N);
}
