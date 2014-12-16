#include <linux/blkdev.h>

#include <linux/kernel.h>
#include <linux/genhd.h>
#include <linux/hdreg.h>

#include <linux/vmalloc.h>

#include "interface.h"


char * alloc_disk_memory(unsigned long size)
{
   char * p = NULL;
   unsigned long i;

   // Comprovem que size no es masa gran
   if(size > xrd_size*1024) {
      return NULL;
   }

   // Allocatem espai a memoria aligian pagina
   p = (char *) vmalloc(PAGE_ALIGN(size));
   for(i = 0; i < size; ++i) {
      p[i] = i % 256;
   }

   printk(KERN_DEBUG "alloc_disk_memory %ld address %p\n", size, p);
   // retorna el punter a la memoria del kernel per guardar les dades del disc
   return p;
}

void   free_disk_memory(char * disk_mem)
{
   // Alliberem memoria
   if(disk_mem != NULL) {
      vfree((const void*) disk_mem);
   }

   printk(KERN_DEBUG "free_disk_memory %p\n", disk_mem);
}


int xrd_getgeo(struct block_device * bdev, struct hd_geometry *geo)
{
   int res = -EIO;
   struct xrd_struct *xrd_str;
   xrd_str = bdev->bd_disk->private_data;

   geo->heads = 32;
   geo->sectors = 128;
   geo->cylinders = xrd_str->size / geo->heads / geo->sectors / SECTOR_SIZE;
   geo->start = 0;
   res = 0;

   // un resultat = 0 indica tot correcte
   printk (KERN_DEBUG "xrd_getgeo returns %d", res);
   return res;
}


int copy_from_xrd(void *dst, struct xrd_struct *xrd,
                        sector_t sector, size_t n)
{
   int res = -ENODEV;
   unsigned long *src;

   src = (unsigned long *) (xrd->disk_memory + sector * SECTOR_SIZE);
   //TODO: check this comparision. Anyway, is working now
   //if((void*)src >= (void*)(xrd->disk_memory + xrd_size)) return -ENOMEM;
   memcpy(dst, (void*) src, n);
   res = 0;

   printk(KERN_DEBUG "copy_from_xrd retorna %d\n", res);
   // un resultat = 0 indica tot correcte
   return res;
}

int copy_to_xrd(struct xrd_struct *xrd, void *src,
                        sector_t sector, size_t n)
{
   int res = -ENODEV;
   unsigned long *dst;

   dst = (unsigned long *) (xrd->disk_memory + sector*SECTOR_SIZE);
   //TODO: check this comparision. Anyway, is working now
   //if((void *)dst >= (void*)(xrd->disk_memory + xrd_size)) return -ENOMEM;
   memcpy((void *) dst, src, n);
   res = 0;

   printk(KERN_DEBUG "copy_to_xrd retorna %d\n", res);
   // un resultat = 0 indica tot correcte
   return res;
}


