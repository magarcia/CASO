#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define    N    1000000

void main () {

    // Define vars
    unsigned int i;
    int res;
    struct timeval start, end;

    gettimeofday(&start, NULL); // Get start time

    for(i=0; i < N; i++) {
        res = sched_yield();

        if (res == -1) {
            printf("Error on execute sched_yield");
            exit(1);
        }
    }

    gettimeofday(&end, NULL); // Get end time

    // Get elapset time seconds
    long t = (end.tv_sec*1e6 + end.tv_usec) - (start.tv_sec*1e6 + start.tv_usec);
    double time = (double)t/1e6;

    printf("%lf s (%lf s/sched_yield)\n", time, time/N);
}
