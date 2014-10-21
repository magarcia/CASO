#include <mach.h>
#include <mach_error.h>
#include <mach/mig_errors.h>
#include <mach/thread_status.h>
#include <mach/mach_traps.h>
#include <stdio.h>
#include <stdlib.h>
#include <hurd.h>
#include <unistd.h>

#define    PAGESIZE    4096
#define    ANYWHERE    1

float data[PAGESIZE/sizeof(float)] __attribute__((aligned(PAGESIZE)));

void help() {
    fprintf (stderr, "USAGE: mtask [-r|-s] PID1 [PID2...]\n");
    fprintf (stderr, "\t-r: Resume tasks identified by PID's\n");
    fprintf (stderr, "\t-s: Suspend tasks identified by PID's\n");
    exit(1);

}

int main (int argc, char *argv[]) {

    // Define vars
    kern_return_t res;
    vm_address_t address, data;
    int data_cnt;
    unsigned int i;

    // Check parameters and show help if necessary
    if (argc < 3) {
        help();
    }

    // Parse args
    pid_t pid = atoi(argv[1]);
    address = atoi(argv[2]);

    // Get task
    task_t task = pid2task(pid);

    // Read memory
    res = vm_read(task, address, PAGESIZE, &data, &data_cnt);
    if (res != KERN_SUCCESS) {
        printf("Error allocating memory: 0x%x, %s\n", res,
                mach_error_string(res));
        exit(1);
    }
    printf(data);

}
