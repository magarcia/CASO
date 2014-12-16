
ifneq ($(KERNELRELEASE),)

 # for kbuild

obj-m := mychardrv.o
mychardrv-y := interface.o implementation.o

else

 # normal Makefile

KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD

endif

play-simple: play-simple.c
	cc -O -o play-simple play-simple.c

play: play.c
	cc -O -o play play.c


demo: default ./play-simple play
	mknod /dev/mychardrv c 230 3
	insmod mychardrv.ko
	./play

clean:
	rm -rf \
.built-in.o.cmd        \
.implementation.o.cmd  \
.interface.o.cmd       \
.mychardrv.ko.cmd      \
.mychardrv.mod.o.cmd   \
.mychardrv.o.cmd       \
.tmp_versions          \
Module.symvers         \
built-in.o             \
implementation.o       \
interface.o            \
modules.order          \
mychardrv.ko           \
mychardrv.mod.c        \
mychardrv.mod.o        \
mychardrv.o            \
play                   \
play-simple
