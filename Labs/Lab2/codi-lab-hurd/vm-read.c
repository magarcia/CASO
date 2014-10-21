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
    fprintf (stderr, "USAGE: vm-read PID ADDRESS\n");
    exit(1);

}

int main (int argc, char *argv[]) {

    // Define vars
    kern_return_t res;
    vm_address_t address, data;
    int data_cnt;
    unsigned int i;
    mach_port_t task_port;

    // Check parameters and show help if necessary
    if (argc < 3) {
        help();
    }

    // Parse args
    pid_t pid = atoi(argv[1]);
    address = atoi(argv[2]);

    // Get task
    task_t task = pid2task(pid);
    res = task_get_kernel_port(task, &task_port);
    if (res != KERN_SUCCESS) {
        printf("Error getting task port: 0x%x, %s\n", res,
                mach_error_string(res));
        exit(1);
    }

    // Read memory
    res = vm_read(task_port, address, PAGESIZE, &data, &data_cnt);
    if (res != KERN_SUCCESS) {
        printf("Error reading memory: 0x%x, %s\n", res,
                mach_error_string(res));
        exit(1);
    }
    printf("%d\n", &data);

}
