#include <sys/types.h>

#include <sys/file.h>
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>

#include "user_process_info.h"

struct user_process_info p_info;

int main(int argc, char * argv [])
{
   int arg, i, fd, res, pid;

   if (argc < 2) {
      printf ("Us: %s pid [...pid...]\n", argv[0]);
      exit(1);
   }

   arg = 1;
   while (arg < argc) {

      fd = open("/dev/pinfo", O_RDONLY, 0);

      if (fd < 0) { perror ("open pinfo"); return 1; }

      if (strcmp("me", argv[arg]) == 0)
         pid = getpid();
      else
         pid = atoi(argv[arg]);

      res = ioctl(fd, IOCTL_CHANGE_PROCESS, pid);
      if (res < 0) { perror ("ioctl pinfo"); return 1; }

      res = read (fd, &p_info, sizeof(p_info));

      if (res < 0) perror ("read pinfo");
      else {
         printf ("Process state %ld,  pid %d, on_cpu %d\n", 
                   p_info.upi_state, p_info.upi_pid, p_info.upi_on_cpu);

         printf ("   User time %ld\n", p_info.upi_utime);
         printf ("   Sys  time %ld\n", p_info.upi_stime);   
      }

      close(fd);

    ++arg;
   }

   return 0;
}
