#include <mach.h>
#include <mach_error.h>
#include <mach/mig_errors.h>
#include <mach/thread_status.h>
#include <mach/machine/thread_status.h>
#include <mach/mach_traps.h>
#include <stdio.h>
#include <stdlib.h>
#include <hurd.h>
#include <unistd.h>

void thread_core() {
    while(1);
    /*printf("Hurd rules\n");*/
}

int stack[8192];

int main () {

    // Define vars
    kern_return_t res;
    mach_port_t parent, child;
    thread_state_t state_child;
    struct i386_thread_state state_child_i386;
    mach_msg_type_number_t state_cnt = THREAD_STATE_MAX;
    parent = mach_task_self();

    // Create thread
    res = thread_create(parent, &child);
    if (res != KERN_SUCCESS) {
        printf("Error creating the thread: 0x%x, %s\n", res,
               mach_error_string(res));
        exit(1);
    }

    // Get thread state
    res = thread_get_state(child, i386_THREAD_STATE, (thread_state_t)&state_child_i386, &state_cnt);
    if (res != KERN_SUCCESS) {
        printf("Error getting the state of the child: 0x%x, %s\n", res,
               mach_error_string(res));
        exit (1);
    }

    state_child_i386.uesp = (unsigned int) stack - 80;
    state_child_i386.eip = (unsigned int) thread_core;

    // Set thread state
    res = thread_set_state(child, i386_THREAD_STATE, (thread_state_t)&state_child_i386, state_cnt);
    if (res != KERN_SUCCESS) {
        printf("Error setting the state of the child: 0x%x, %s\n", res,
               mach_error_string(res));
        exit(1);
    }

    // Resume thread
    res = thread_resume(child);
    if (res != KERN_SUCCESS) {
        printf("Error resuming the child thread: 0x%x, %s\n", res,
               mach_error_string(res));
        exit(1);
    }

    sleep(20);

    // Terminate thread
    res = thread_terminate(child);
    if (res != KERN_SUCCESS) {
        printf("Error terminating the child thread: 0x%x, %s\n", res,
               mach_error_string(res));
        exit (1);
    }
}
