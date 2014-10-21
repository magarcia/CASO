#include <mach.h>
#include <mach_error.h>
#include <mach/mig_errors.h>
#include <mach/thread_status.h>
#include <stdio.h>
#include <stdlib.h>
#include <hurd.h>

void help() {
    fprintf (stderr, "USAGE: mtask [-r|-s] PID1 [PID2...]\n");
    fprintf (stderr, "\t-r: Resume tasks identified by PID's\n");
    fprintf (stderr, "\t-s: Suspend tasks identified by PID's\n");
    exit(1);

}

int main (int argc, char *argv[])
{
    // Define vars
    int res, i, j, num_pids;
    mach_port_t host_privileged_port;
    device_t device_privileged_port;
    mach_port_t default_set, processor_set;
    task_array_t task_list;
    mach_msg_type_number_t task_count;
    pid_t *pids;
    unsigned char res_susp;

    // Check parameters and show help if necessary
    if (argc < 3) {
        help();
    }

    // Check if first option si -r of -s
    if (strcmp (argv[1], "-r") == 0)
        res_susp = 1;
    else if (strcmp (argv[1], "-s") == 0)
        res_susp = 0;
    else {
        help();
    }

    // Get PIDs from params
    pids = (pid_t *) malloc(sizeof(pid_t) * (argc-2));
    for (i = 2; i < argc; ++i) {
        pids[i-2] = atoi(argv[i]);
    }
    num_pids = argc - 2;

    // For each PID resume or suspend task
    for (i = 0; i < num_pids; ++i) {
        task_t task = pid2task(pids[i]);

        // Check if task exist
        if(task == MACH_PORT_NULL) {
            printf("There is no task related to PID %d\n", pids[i]);
        } else {
            if (res_susp) {
                // Resume task
                res = task_resume(task);
                if (res != KERN_SUCCESS) {
                    printf("Error resuming task with PID %d, 0x%x, %s\n", pids[i], res,
                            mach_error_string(res));
                }
            } else {
                // Suspend task
                res = task_suspend(task);
                if (res != KERN_SUCCESS) {
                    printf("Error suspending task with PID %d, 0x%x, %s\n", pids[i], res,
                            mach_error_string(res));
                }
            }
        }
    }

}
