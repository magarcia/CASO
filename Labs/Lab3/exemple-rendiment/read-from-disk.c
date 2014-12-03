
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/times.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[])
{
    // Init vars
    FILE *fd;
    int n,i, sec, usec, sizeMB, rounds, pad;
    unsigned long int size;
    float bandwidth;
    struct timeval start, end;
    char fmt_string[20];
    char *buffer;
    char *filename;

    // Check args and print help if necessary
    if (argc != 3) {
        fprintf(stderr, "USAGE: %s [FILE] [SIZE_MB]\n", argv[0]);
        exit(1);
    }

    // Parse args
    filename = argv[1];
    size = atoi(argv[2]) * 1024 * 1024;

    // Allocate size
    buffer = (char*) malloc(size);
    if (buffer == NULL) {
        error("Cannot allocate enough memory");
    }

    fd = fopen(filename, "r+"); // Open file

    gettimeofday(&start, NULL); // Get start time

    // Read from file
    fread(buffer, sizeof(char), size, fd);

    gettimeofday(&end, NULL); // Get end time

    free(buffer);

    fclose(fd); // Close the file

    // Get elapset time seconds
    sec = end.tv_sec - start.tv_sec;
    usec = end.tv_usec - start.tv_usec;
    long t = (end.tv_sec*1e6 + end.tv_usec) - (start.tv_sec*1e6 + start.tv_usec);

    sizeMB = (int)(size / 1000 / 1000); // Get size in MB

    // Calculate bandwidth
    sprintf(fmt_string, "%d.%d",sec, usec);
    bandwidth = sizeMB / atof(fmt_string);

    printf("%lu bytes (%d MB) readed, %f s, %.2f MB/s\n", size, sizeMB, (double)t/1e6, bandwidth);

    exit(EXIT_FAILURE);
}