#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/major.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>

#include "user_process_info.h"


MODULE_LICENSE("Dual BSD/GPL");

#define MY_MAJOR 231

#define OK 0


static int get_process (int pid, struct task_struct ** p)
{
      struct pid * struct_pidp = find_get_pid(pid);
      struct task_struct * t = get_pid_task(struct_pidp, PIDTYPE_PID);
      if (t == NULL) {
         printk (KERN_DEBUG "error: unexistant process %d\n", pid);
         return -ESRCH;
      }
      *p = t;
      return OK;
}

static int current_pid;

static void set_inspect_process(int pid)
{
   current_pid = pid;
}


int num_opens;

static int mychardrv_open(struct inode * inode, struct file * file)
{
   struct task_struct * p;
   int minor = iminor(inode);
   int err = 0;

   printk(KERN_DEBUG "device open! (minor %d)\n", minor);

   if (minor!=3) err = -ENODEV;
   else {
      // implementation, check process is ok, and increment num_opens
      printk(KERN_DEBUG "setting current process %d\n", current->pid);
      err = get_process(current->pid, &p);
      if (err == 0) {
         set_inspect_process(current->pid);
         ++num_opens;
      }
   }

   return err;
}

static int mychardrv_close(struct inode * inode, struct file * file)
{
   int minor = iminor(inode);
   printk(KERN_DEBUG "device release! (minor %d)\n", minor);
   // implementation, decrease num_opens
   --num_opens;
   return 0;
}


static ssize_t mychardrv_read(struct file * f, char __user * address,
                            size_t size, loff_t * offset)
{
   struct user_process_info internal_copy;
   ssize_t res = 0;
   ssize_t total_copy_size;
   printk(KERN_DEBUG "device read to user address %p\n", address);
   printk(KERN_DEBUG "   requested size %ld bytes\n", size);

  // implementation goes here







  // end of your implementation

   return total_copy_size;
}

// uncomment and implement for phase 2
//static long mychardrv_ioctl(struct file * f, unsigned int comm, unsigned long pid)


static const struct file_operations mychardrv_fops = {
        .owner          = THIS_MODULE,
        .open           = mychardrv_open,
	.release        = mychardrv_close,
        .read           = mychardrv_read,
        //.llseek         = noop_llseek,
};

static int mychardrv_init(void) {
  int res;

  printk(KERN_DEBUG "Hello world!\n");


  res = register_chrdev(MY_MAJOR, "mychardrv", &mychardrv_fops);
  if (res < 0) {
    printk("unable to get major %d for mychardrv\n", MY_MAJOR);
    return res;
  }

  return 0;
}

static void mychardrv_exit(void) {

  unregister_chrdev(MY_MAJOR, "mychardrv");
  printk(KERN_DEBUG "Bye, cruel world\n");
}

module_init(mychardrv_init);
module_exit(mychardrv_exit);
