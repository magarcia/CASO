#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define    N    1000000

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
    long t = (end.tv_sec*1e6 + end.tv_usec) - (start.tv_sec*1e6 + start.tv_usec);
    double time = (double)t/1e6;

    printf("%0.5f s (%0.5f s/sbrk)\n", time, time/N);
}
