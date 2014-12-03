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

int main (int argc, char *argv[]) {

    // Define vars
    kern_return_t res;
    mach_port_t parent_task, child_task;
    vm_address_t address;
    unsigned int i;

    parent_task = mach_task_self();

    // Create task
    res = task_create(parent_task, 0, &child_task);
    if (res != KERN_SUCCESS) {
        printf("Error creating the child task: 0x%x, %s\n", res,
               mach_error_string(res));
        exit(1);
    }

    // Allocate memory to thread
    res = vm_allocate(child_task, &address, PAGESIZE, ANYWHERE);
    if (res != KERN_SUCCESS) {
        printf("Error allocating memory: 0x%x, %s\n", res,
                mach_error_string(res));
        exit(1);
    }

    // Write in memory thread
    res = vm_write(child_task, address, (vm_offset_t)data, PAGESIZE);
    if (res != KERN_SUCCESS) {
        printf("Error writing: 0x%x, %s\n", res,
               mach_error_string(res));
        exit(1);
    }
}
