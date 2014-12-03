#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define    N    10e6

void main () {

    // Define vars
    unsigned int i;
    int status;
    pid_t pid;
    struct timeval start, end;

    gettimeofday(&start, NULL); // Get start time

    for(i=0; i < N; i++) {
        pid = fork();
        if (pid ==  0) { // Child
            exit(0);
        } else if (pid == -1) {    // Parent error
            printf("Error on fork");
            exit(1);
        } else {    // Parent
            pid = waitpid(-1, &status, 0);
            if (pid == -1) {
                printf("Error on waitpid");
                exit(1);
            }
        }
    }

    gettimeofday(&end, NULL); // Get end time

    // Get elapset time seconds
    unsigned long time = (end.tv_sec*1e6 + end.tv_usec) - (start.tv_sec*1e6 + start.tv_usec);

    printf("%lu usec (%lf usec/fork_waitpid)\n", time, time/N);
}
