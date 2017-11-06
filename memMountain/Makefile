stride 	= 1
random	= 0
size	= 128
CC	= gcc
CFLAGS	= -O3 -g -mtune=native -march=native $(DEFINES)

all: memMountain random

memMountain: memMountain.c randomutils.o runTest.o
	$(CC) $(CFLAGS) $^ -DARRAY_SIZE=$(size) -DSTRIDE=$(stride) -DRANDOM_ACCESS=0 -o $@

memMountainPtrChase: memMountain.c randomutils.o
	./createAccessPattern.py 1 $(size) strided ptr
	$(CC) $(CFLAGS) -c runTest.c
	$(CC) $(CFLAGS) $^ runTest.o -DARRAY_SIZE=$(size) -DSTRIDE=$(stride) -DRANDOM_ACCESS=$(random) -o $@

access.h: createAccessPattern.py
	./createAccessPattern.py $(stride) $(size)

runTest.o: runTest.c access.h
	$(CC) $(CFLAGS) $< -c

random: memMountain.c
	./createAccessPattern.py 1 $(size) random
	$(CC) $(CFLAGS) $< -DARRAY_SIZE=$(size) -DSTRIDE=1 -DRANDOM_ACCESS=$(random) -o $@

randomTest: randtest.c randomutils.o
	$(CC) $(CFLAGS) $^ -o $@

randomutils.o: randomutils.c randomutils.h
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -rf *.o memMountain memMountainPtrChase random randomTest access.h
