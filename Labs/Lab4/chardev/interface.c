#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/major.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include "file_string_user.h"


MODULE_LICENSE("Dual BSD/GPL");

#define MY_MAJOR 230

int num_opens;

struct file_string {
   struct file_string * next;
   int id;
   char * string;
};


struct strings_representation {
   struct file_string * next;
   int current_id;
};

static int mychardrv_open(struct inode * inode, struct file * file)
{
   int minor = iminor(inode);
   int err = 0;

   printk(KERN_DEBUG "device open! (minor %d)\n", minor);

   if (minor!=3) err = -ENODEV;
   else {
      // implementation, increment num_opens
      ++num_opens;
      // initialize the string table associated to the file
      struct strings_representation * sr =
                     kzalloc(sizeof(struct strings_representation), GFP_USER);
      sr->next = NULL;
      sr->current_id = -1;

      file->private_data = sr;
   }

   return err;
}

static int mychardrv_close(struct inode * inode, struct file * file)
{
   int minor = iminor(inode);
   printk(KERN_DEBUG "device release! (minor %d)\n", minor);
   // implementation, decrease num_opens
   --num_opens;
   // release memory at file->private_data !!!

   // TODO
   struct strings_representation * sr = file->private_data;
   kfree(sr);


   return 0;
}

static struct file_string * find_file_string (struct file * f)
{
   struct strings_representation * sr = f->private_data;
   struct file_string * fstr = NULL;
   if (sr != NULL) {
      fstr = sr->next;
   }
   if (sr && sr->current_id >= 0) {
      while ((fstr !=NULL) && (fstr->id != sr->current_id)) {
         fstr = fstr -> next;
      }
      return fstr;
   }
   else return NULL;
}

static ssize_t mychardrv_write (struct file * f, const char __user * address,
                                 size_t size, loff_t * offset)
{
   struct strings_representation * sr = f->private_data;
   ssize_t res = 0;
   long result;
   struct file_string * fstr = find_file_string(f);
   if (fstr ==NULL) {
      // add a new string
      fstr = kzalloc(sizeof(struct file_string), GFP_USER);
      if (fstr == NULL) {
         res = -ENOMEM;
         goto error;
      }
      fstr->string = kzalloc(size, GFP_USER);
      if (fstr->string==NULL) {
         kfree (fstr);
         res = -ENOMEM;
         goto error;
      }
      fstr->id = sr->current_id;
      fstr->next = sr->next;
      sr->next = fstr;
   }
   else {
      char * tmp = kzalloc(size, GFP_USER);
      if (tmp==NULL) {
         res = -ENOMEM;
         goto error;
      }
      kfree (fstr->string);
      fstr->string = tmp;
   }
   result = probe_kernel_read(fstr->string, address, size);
   printk (KERN_DEBUG "probe_kernel_write returns %ld\n", result);
   res = result;
   if (result < 0) {
      printk (KERN_DEBUG "string is nullified\n");
      fstr->string = NULL;
      goto error;
   }

   res = copy_from_user(fstr->string, address, size);
   if (res == 0) {
      printk(KERN_DEBUG "copied %ld bytes from the user at offset %ld\n", size, offset);
      res = size;
      *offset += size;
   }
   else {
      printk(KERN_DEBUG "error: %ld bytes could not be copied from user at offset %ld\n", res, offset);
      res = (ssize_t) -EFAULT;
   }

error:
   return res;
}

static ssize_t mychardrv_read(struct file * f, char __user * address,
                            size_t size, loff_t * offset)
{
   ssize_t res = 0;
   ssize_t copy_size;
   struct file_string * fstr = find_file_string(f);
   printk(KERN_DEBUG "device read to user address %p\n", address);
   printk(KERN_DEBUG "   requested size %ld bytes\n", size);
   if (offset==NULL)
      printk(KERN_DEBUG "   current file offset not provided (NULL)\n");
   else
      printk(KERN_DEBUG "   current file offset %lld\n", *offset);

   if (fstr!=NULL) {
    if (fstr->string!=NULL) {
      copy_size = min(size, strlen(fstr->string)+1);
      res = copy_to_user (address, fstr->string, copy_size);
      if (res == 0)
         printk(KERN_DEBUG "copied %ld bytes to the user\n", copy_size);
      else {
         printk(KERN_DEBUG "error: %ld bytes could not be copied to user\n", res);
         copy_size = (ssize_t) -EFAULT;
      }
    }
    else {
       copy_size = 0;
    }
   }
   else if (size < sizeof (num_opens)) copy_size = (ssize_t) -ENOBUFS;
   else {
      // implementation, copy the num_opens number into the user buffer
      copy_size = sizeof(num_opens);
      res = copy_to_user (address, &num_opens, copy_size);
      if (res == 0)
         printk(KERN_DEBUG "copied %ld bytes to the user\n", copy_size);
      else {
         printk(KERN_DEBUG "error: %ld bytes could not be copied to user\n", res);
         copy_size = (ssize_t) -EFAULT;
      }
   }

   return copy_size;
}

long mychardrv_ioctl (struct file * f, unsigned int command, unsigned long arg)
{
   long ret = -EINVAL;
   struct strings_representation * sr = f->private_data;
   if (command == SET_STRING_ID) {
      sr->current_id = arg;
      ret = 0;
   }
   else if (command == GET_STRING_ID) {
      ret = sr->current_id;
   }
   return ret;
}


static const struct file_operations mychardrv_fops = {
        .owner          = THIS_MODULE,
        .open           = mychardrv_open,
	      .release        = mychardrv_close,
        .read           = mychardrv_read,
	      .unlocked_ioctl   = mychardrv_ioctl,
	      .write          = mychardrv_write,
        //.llseek         = noop_llseek,
};

static int mychardrv_init(void) {
  int res;

  printk(KERN_DEBUG "Hello world!\n");


  //proc_create("mychardrv", 0, NULL, &mychardrv_proc_fops);
  res = register_chrdev(MY_MAJOR, "mychardrv", &mychardrv_fops);
  if (res < 0) {
    printk("unable to get major %d for mychardrv\n", MY_MAJOR);
    return res;
  }

  return 0;
}

static void mychardrv_exit(void) {

  //int res;

  unregister_chrdev(MY_MAJOR, "mychardrv");
  printk(KERN_DEBUG "Bye, cruel world\n");
}

module_init(mychardrv_init);
module_exit(mychardrv_exit);
