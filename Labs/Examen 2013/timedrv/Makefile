
ifneq ($(KERNELRELEASE),)

 # for kbuild

obj-m := mychardrv.o
mychardrv-y := interface.o implementation.o

else

 # normal Makefile

KDIR ?= /lib/modules/`uname -r`/build

default: mytimes yourtimes
	$(MAKE) -C $(KDIR) M=$$PWD

mytimes: mytimes.c
	gcc -O -o mytimes mytimes.c

yourtimes: yourtimes.c
	gcc -O -o yourtimes yourtimes.c

endif

clean:
	rm -f built-in.o implementation.o interface.o modules.order \
		Module.symvers mychardrv.ko mychardrv.mod.c mychardrv.mod.o \
		mychardrv.o mytimes yourtimes
