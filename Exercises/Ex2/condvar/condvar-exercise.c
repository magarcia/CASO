#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>

#define BLOCK_SIZE 1000
#define NWORKERS   4

#define OUTPUT

const char * search_basename(const char * fname)
{
   int i = strlen(fname) - 1;
   while (i>0 && fname[i] != '/') --i;
   if (fname[i] == '/') return &fname[i+1];
   return &fname[i];
}

int get_file(char * dirname, char * fname)
{
  char buffer[BLOCK_SIZE];
  char outfname[PATH_MAX];
   struct sockaddr_in addr_in;
   int off, total = 0, res, s = socket(AF_INET, SOCK_STREAM, 0);
#ifdef OUTPUT
   int fd;
   struct stat statbuf;
#endif
   if (s<0) { perror("socket"); return(1); }
   addr_in.sin_family = AF_INET;
   addr_in.sin_port = htons(80);
   addr_in.sin_addr.s_addr = htonl (0x93531e6a); //htonl(0x6a1e5393);
   bzero(addr_in.sin_zero, sizeof(addr_in.sin_zero));
   res = connect(s, (const struct sockaddr *) &addr_in, sizeof(addr_in));
   if (res<0) { perror("connect"); fprintf(stderr, "%d\n", errno); return(1); }


   write (s, "GET /xavim/", 11);
   write (s, fname, strlen(fname));
   write (s, " HTTP/1.1\n", 10);
   write (s, "Host: people.ac.upc.edu\n\n", 25);

#ifdef OUTPUT
   res = stat(dirname, &statbuf);
   if (res < 0) {
      perror ("stat");
      fprintf (stderr, "   checking directory %s\n", dirname);
      res = mkdir(dirname, 0750);
      if (res < 0 && errno != EEXIST) { perror ("mkdir (output dir)"); return(1); }
   }
   else if (! S_ISDIR(statbuf.st_mode) ) {
      fprintf (stderr, "Error: %s exists and it is not a directory. Exiting\n", dirname); return(2);
   }
   strcpy (outfname, dirname);
   strcat (outfname, "/");
   strcat (outfname, search_basename(fname));
   fd = open(outfname, O_WRONLY|O_CREAT|O_TRUNC, 0640);
   if (fd < 0) { perror ("open out"); return(1); }
#endif
    // at this point, fname can be freed
   free(fname);

   res = read (s, buffer, BLOCK_SIZE);
   if (res < 0) { perror ("read"); return(1); }
   off = 0;
   while ((off < (res-3)) && (0 != strncmp(&buffer[off], "\r\n\r\n", 4))) off++;
   off += 4;

   while (res > 0) {
#ifdef OUTPUT
      write(fd, &buffer[off], res-off);
#endif
      total += res;
      off = 0;
      res = read (s, buffer, BLOCK_SIZE);
   }

   if (res < 0) { perror ("read"); }

#ifdef OUTPUT
   res = close(fd);
   if (res < 0) { perror ("close out"); }
#endif

   res = close(s);
   if (res < 0) { perror ("close"); }
}


char * Work[4];
int insert_loc = 0, get_loc = 0;
pthread_mutex_t mutex;
pthread_cond_t workers_ready_condition;
pthread_cond_t work_array_full_condition;

void * wait_for_work(void * arg)
{
   int res;
   char * work;

  do {
    // get mutual exclusion to check for work
   res = pthread_mutex_lock(&mutex);
   if (res != 0) {
      fprintf (stderr, "pthread_mutex_lock: %s\n", strerror (res));
      exit (1);
   }

    // wait until there is work to do
   while (Work[get_loc] == NULL) {
     // needs to wait without holding the mutex


   }

   // critical region: there is work to do
   work = Work[get_loc];
   if (work != (void *) -1) {
       Work[get_loc] = NULL;
       get_loc = (get_loc + 1) % 4;
   }

   res = pthread_cond_signal(&work_array_full_condition);
   if (res != 0) {
      fprintf (stderr, "(wait_for_for) pthread_cond_signal: %s\n", strerror (res));
      exit(1);
   }
   res = pthread_mutex_unlock(&mutex);
   if (res != 0) {
      fprintf (stderr, "pthread_mutex_unlock: %s\n", strerror (res));
      exit (1);
   }

   if (work != (void *) -1)
      res = get_file ("parallel-out.tmp", work);

  } while (work != (void *) -1);

   // pthread_exit
   return NULL;
}

void more_work(char *fname)
{
   int res;

     // get mutual exclusion to enter the critical region
   res = pthread_mutex_lock(&mutex);
   if (res != 0) {
      fprintf (stderr, "pthread_mutex_lock: %s\n", strerror (res));
      exit (1);
   }

     // wait until there is space to add the new work
   while ( ((insert_loc+1)%4) == get_loc ) {
      res = pthread_cond_wait(&work_array_full_condition, &mutex);
      if (res != 0) {
         fprintf (stderr, "(more_work) pthread_cond_wait: %s\n", strerror (res));
         exit(1);
      }
   }

   // critical region and there is space
   Work[insert_loc] = fname;
   insert_loc = (insert_loc + 1) % 4;

   if (fname == (void *) -1) {
      // Wakeup all workers to end the processing
      res = pthread_cond_broadcast(&work_array_full_condition);
      if (res != 0) {
         fprintf (stderr, "(more_work) pthread_cond_broadcast: %s\n", strerror (res));
         exit(1);
      }
   }
   else {
      // Wakeup a worker if any is sleepping
      res = pthread_cond_signal(&work_array_full_condition);
      if (res != 0) {
         fprintf (stderr, "(wait_for_for) pthread_cond_signal: %s\n", strerror (res));
         exit(1);
      }
   }

   // leaving the critical section
   res = pthread_mutex_unlock(&mutex);
   if (res != 0) {
      fprintf (stderr, "pthread_mutex_unlock: %s\n", strerror (res));
      exit (1);
   }
}

int main()
{
   int res;
   time_t start_time = time(NULL), stop_time;
   FILE * f = fopen("index", "r");
   char * linetoprocess;
   if (f==NULL) { perror ("fopen"); exit(1); }

   printf ("Serial execution...   ");

   res = fscanf(f, "%80ms", &linetoprocess);
   while (res > 0) {
      printf ("Processing %s (%d, 0x%lX)\n", linetoprocess, res, malloc(0));

       // get_file frees the memory area pointed to by linetoprocess
      res = get_file("serial-out.tmp", linetoprocess);
      if (res != 0) exit(res);
      res = fscanf(f, "%80ms", &linetoprocess);
   }
   fclose(f);

   stop_time = time(NULL);
   printf ("  Serial execution time %ld secs.\n", stop_time - start_time);

   start_time = time(NULL);
   pthread_t thrs[NWORKERS];
   int i;

   printf ("Parallel execution... ");

   res = pthread_mutex_init(&mutex, NULL);
   if (res != 0) {
      fprintf (stderr, "pthread_mutex_init: %s\n", strerror (res));
      exit (1);
   }
   res = pthread_cond_init(&workers_ready_condition, NULL);
   if (res != 0) {
      fprintf (stderr, "pthread_cond_init: %s\n", strerror (res));
      exit (1);
   }
   res = pthread_cond_init(&work_array_full_condition, NULL);
   if (res != 0) {
      fprintf (stderr, "pthread_cond_init: %s\n", strerror (res));
      exit (1);
   }

   for (i=0; i < NWORKERS; i++) {
      res = pthread_create(&thrs[i], NULL, wait_for_work, NULL);
      if (res != 0) fprintf (stderr, "pthread_create: %s\n", strerror(res));
   }

   f = fopen("index", "r");
   if (f==NULL) { perror ("fopen"); exit(1); }

   res = fscanf(f, "%80ms", &linetoprocess);
   while (res > 0) {
      printf ("Processing %s (%d, 0x%lX)\n", linetoprocess, res, malloc(0));

      more_work(linetoprocess);
      res = fscanf(f, "%80ms", &linetoprocess);
   }

   fclose(f);

   more_work((char *) -1);

   for (i=0; i < NWORKERS; i++) {
      res = pthread_join(thrs[i], NULL);
      if (res != 0) fprintf (stderr, "pthread_join: %s\n", strerror(res));
   }
   stop_time = time(NULL);
   printf ("Parallel execution time %ld secs.\n", stop_time - start_time);

   return 0;
}
