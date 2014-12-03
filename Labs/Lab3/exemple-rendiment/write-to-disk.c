
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/times.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    // Init vars
    FILE *fd;
    int i, sec, usec, sizeMB, rounds, pad;
    unsigned long int size;
    float bandwidth;
    struct timeval start, end, res;
    char *buffer;
    char *filename;

    // Check args and print help if necessary
    if (argc != 3) {
        fprintf(stderr, "USAGE: %s [FILE] [SIZE_MB]\n", argv[0]);
        exit(1);
    }

    // Parse args
    filename = argv[1];
    size = atoi(argv[2]);

    // Prevent allocate too much memory
    rounds = 1;
    pad = 0;
    if (size > 1024) {
        rounds = size / 1024; // Rounds of 1024 * 1024 * 1024, 1Gb
        pad = size % 1024;  // Pad of 1024 * 1024 * 1024, 1Gb
        size = 1024;
    }

    size = (unsigned long int)(size * 1024 * 1024);

    // Allocate size
    buffer = (char*) malloc(size);
    if (buffer == NULL) {
        error("Cannot allocate enough memory");
    }

    fd = fopen(filename, "w+"); // Open file
    
    gettimeofday(&start, NULL); // Get start time

    // Write to file
    for(i = 0; i < rounds; i++){
        fwrite(buffer, sizeof(char), size, fd);
    }
    if (pad > 0) {
        fwrite(buffer, sizeof(char), pad*1024*1024, fd);
    }


    gettimeofday(&end, NULL); // Get end time

    size = (size * rounds) + (pad * 1024);

    free(buffer);

    fclose(fd); // Close the file

    // Get elapset time seconds
    timersub(&end, &start, &res);
    long t = res.tv_sec*1e6 + res.tv_usec;


    // Calculate bandwidth
    bandwidth = (float)(size/1024/1024)/((long double)t/1e6);
    #ifdef DEBUG
    printf("%d\t%.2f\n", (int)(size/1024/1024), bandwidth);
    #else
    printf("%lu bytes (%d MB) writed, %Lf s, %.2f MB/s\n", size, (int)(size/1024/1024), (long double)t/1e6, bandwidth);
    #endif

    exit(EXIT_FAILURE);
}