// Defined for user-level only
#ifndef __KERNEL__
typedef unsigned long cputime_t;
#endif


/* Information shared between kernel and user levels */

/* Struct to copy to user */
struct user_process_info {
   long      upi_state;
   int       upi_on_cpu;
   pid_t     upi_pid;
   cputime_t upi_utime, upi_stime, upi_utimescaled, upi_stimescaled;
};

// ioctl requests that can be applied to the device driver
#define IOCTL_CHANGE_PROCESS  33   // argument is the pid of the target process
