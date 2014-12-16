#include <stdio.h>

#include "file_string_user.h"


int buf [4096];
int cbuf[1024];

int main()
{
   FILE * f = fopen ("/dev/mychardrv", "w+");

   int res;


   res = ioctl (fileno(f), GET_STRING_ID, 0);
   printf ("current string id %d\n", res);



   res = fread(buf, 1, 4096, f);
   if (res < 0) perror ("fread");
   if (res < sizeof(int)) printf ("Expected %d bytes, got %d\n", (int)sizeof(int), res);
   printf ("Got %d opens\n", buf[0]);

   res = ioctl (fileno(f), SET_STRING_ID, 9);
   if (res < 0) perror ("ioctl SET_STRING_ID");

   res = fwrite("Hi, this is a first test\n", 1, 26, f);
   if (res < 0) perror ("fwrite");
   printf ("Written %d bytes to the file\n", res);

   res = fread(cbuf, 1, 10, f);
   if (res < 0) perror ("read");
   else {
      cbuf[res] = '\0';
      printf ("Read '%s'\n", cbuf);
   }

   fclose(f);


}
