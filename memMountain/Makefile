stride 		= 1
random		= 0
num_elems	= 512
CC		= gcc
CFLAGS		= -O3 -g -mtune=native -march=native $(DEFINES)

all: memMountain random

memMountain: memMountain.c randomutils.o runTest.o
	$(CC) $(CFLAGS) $^ -DNUM_ELEMS=$(num_elems) -DSTRIDE=$(stride) -DRANDOM_ACCESS=0 -o $@

memMountainPtrChase: memMountain.c randomutils.o
	./createAccessPattern.py -s 1 -n $(num_elems) -b ptr
	$(CC) $(CFLAGS) -c runTest.c
	$(CC) $(CFLAGS) $^ runTest.o -DNUM_ELEMS=$(num_elems) -DSTRIDE=$(stride) -DRANDOM_ACCESS=$(random) -o $@

instrMountain: memMountain.c randomutils.o
	./createAccessPattern.py -s 1 -n $(num_elems) -b instr
	$(CC) $(CFLAGS) -c runTest.c
	$(CC) $(CFLAGS) $^ runTest.o -DNUM_ELEMS=$(num_elems) -DSTRIDE=$(stride) -DRANDOM_ACCESS=$(random) -o $@

access.h: createAccessPattern.py
	./createAccessPattern.py $(stride) $(num_elems)

runTest.o: runTest.c access.h
	$(CC) $(CFLAGS) $< -c

random: memMountain.c
	./createAccessPattern.py 1 $(num_elems) random
	$(CC) $(CFLAGS) $< -DNUM_ELEMS=$(num_elems) -DSTRIDE=1 -DRANDOM_ACCESS=$(random) -o $@

randomTest: randtest.c randomutils.o
	$(CC) $(CFLAGS) $^ -o $@

randomutils.o: randomutils.c randomutils.h
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -rf *.o memMountain instrMountain memMountainPtrChase random randomTest
