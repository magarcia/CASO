all: sleep_2 curtime sbrk sbrk-incdec sched_yield getpid fork_waitpid write-to-disk read-from-disk

write-to-disk: write-to-disk.c
	gcc -O -o $@ $<

write-to-disk-debug: write-to-disk.c
	gcc -DDEBUG -O -o $@ $<

write-to-disk1: write-to-disk1.c
	gcc -O -o $@ $<

write-to-disk2: write-to-disk2.c
	gcc -O -o $@ $<

write-to-disk2-debug: write-to-disk2.c
	gcc -DDEBUG -O -o $@ $<

read-from-disk: read-from-disk.c
	gcc -O -o $@ $<

sbrk: sbrk.c
	gcc -O -o $@ $<

sbrk-incdec: sbrk-incdec.c
	gcc -O -o $@ $<

sched_yield: sched_yield.c
	gcc -O -o $@ $<

getpid: getpid.c
	gcc -O -o $@ $<

fork_waitpid: fork_waitpid.c
	gcc -O -o $@ $<

sleep_2: sleep_2.c
	gcc -O -o $@ $<

curtime: curtime.c
	gcc -O -o $@ $<


clean:
	rm -f write-to-disk write-to-disk.o read-from-disk read-from-disk.o \
              sbrk sbrk.o sched_yield sched_yield.o getpid getpid.o \
              sleep_2 sleep_2.o curtime curtime.o write-to-disk2 \
              write-to-disk1 write-to-disk-debug fork_waitpid
