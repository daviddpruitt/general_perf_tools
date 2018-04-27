stride 		= 1
random		= True
num_elems	= 512
num_regs	= 14
loads_iter	= 512
mem_type	= -b reg_reg
iaca		=
CC		= gcc
CFLAGS		= -O3 -mtune=native -march=native -Wall $(DEFINES)

all: memMountain random

memMountain: memMountain.c randomutils.o 
	./createAccessPattern.py -s 1 -n $(num_elems) -g $(num_regs) -l $(loads_iter) $(mem_type) $(iaca)
	$(CC) $(CFLAGS) -c runTest.c	
	$(CC) $(CFLAGS) -S runTest.c
	$(CC) $(CFLAGS) $^ runTest.o -DNUM_ELEMS=$(num_elems) -DSTRIDE=$(stride) -DRANDOM_ACCESS=0 -o $@

memMountainPtrChase: memMountain.c randomutils.o
	./createAccessPattern.py -s 1 -n $(num_elems) -b ptr
	$(CC) $(CFLAGS) -c runTest.c
	$(CC) $(CFLAGS) $^ runTest.o -DNUM_ELEMS=$(num_elems) -DSTRIDE=$(stride) -DRANDOM_ACCESS=$(random) -o $@

instrMountain: memMountain.c randomutils.o
	./createAccessPattern.py -s 1 -n $(num_elems) -b instr
	$(CC) $(CFLAGS) -c runTest.c
	$(CC) $(CFLAGS) $^ runTest.o -DNUM_ELEMS=$(num_elems) -DSTRIDE=1 -DRANDOM_ACCESS=0 -o $@

instrLoadMountain: memMountain.c randomutils.o
	./createAccessPattern.py -s 1 -n $(num_elems) -b instr-load
	$(CC) $(CFLAGS) -c runTest.c
	$(CC) $(CFLAGS) $^ runTest.o -DNUM_ELEMS=$(num_elems) -DSTRIDE=1 -DRANDOM_ACCESS=0 -o $@

access.h: createAccessPattern.py
	./createAccessPattern.py -s $(stride) -n $(num_elems) -b asm

runTest.o: runTest.c access.h
	$(CC) $(CFLAGS) $< -c

random: memMountain.c randomutils.o
	./createAccessPattern.py -s 1 -n $(num_elems) -g $(num_regs) -l $(loads_iter) -b mem_reg  -r random
	$(CC) $(CFLAGS) -c runTest.c
	$(CC) $(CFLAGS) -S runTest.c -o runTestRandom.s
	$(CC) $(CFLAGS) $^ runTest.o -DNUM_ELEMS=$(num_elems) -DSTRIDE=$(stride) -DRANDOM_ACCESS=$(random) -o $@

randomTest: randtest.c randomutils.o
	$(CC) $(CFLAGS) $^ -o $@

randomutils.o: randomutils.c randomutils.h
	$(CC) $(CFLAGS) $< -c -o $@

# make print 'CFLAGS=-g -mvols' 'mem_type=-b reg_reg' 'iaca=-i False'
print:
	echo $(CFLAGS)
	echo $(mem_type)
	echo $(iaca)
	echo $(CFLAGS) $(mem_type) $(iaca)
clean:
	rm -rf *.o memMountain instrMountain memMountainPtrChase random randomTest
