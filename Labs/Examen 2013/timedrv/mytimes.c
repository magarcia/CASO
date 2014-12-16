#include <sys/types.h>

#include <sys/file.h>
#include <sys/time.h>

#include <stdio.h>

#include "user_process_info.h"

struct user_process_info p_info;

int main()
{
   int i, fd, res;
   fd = open("/dev/pinfo", O_RDONLY, 0);

   if (fd < 0) { perror ("open pinfo"); return 1; }

   res = read (fd, &p_info, sizeof(p_info));

   if (res < 0) perror ("read pinfo");

   printf ("Process state %ld,  pid %d, on_cpu %d, initial times\n", 
                p_info.upi_state, p_info.upi_pid, p_info.upi_on_cpu);

   printf ("   User time %ld\n", p_info.upi_utime);
   printf ("   Sys  time %ld\n", p_info.upi_stime);   

   i = 0;
   while(i<100000) write (1, "", 0), ++i;

   res = read (fd, &p_info, sizeof(p_info));

   if (res < 0) perror ("read pinfo");

   printf ("Process state %ld,  pid %d, on_cpu %d, after actively counting times\n",
                p_info.upi_state, p_info.upi_pid, p_info.upi_on_cpu);

   printf ("   User time %ld\n", p_info.upi_utime);
   printf ("   Sys  time %ld\n", p_info.upi_stime);

   sleep (1);

   res = read (fd, &p_info, sizeof(p_info));

   if (res < 0) perror ("read pinfo");

   printf ("Process state %ld,  pid %d, on_cpu %d, after sleep times\n",
                p_info.upi_state, p_info.upi_pid, p_info.upi_on_cpu);

   printf ("   User time %ld\n", p_info.upi_utime);
   printf ("   Sys  time %ld\n", p_info.upi_stime);

   return 0;
}
