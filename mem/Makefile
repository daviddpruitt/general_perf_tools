CFLAGS	=	-O3 -g
CC	= 	cc
all: memtest

memtest: memtest.o memfuncs.o
	${CC} ${CFLAGS} -o $@ $^

memfuncs.o: memfuncs.c memfuncs.h data.h
	${CC} ${CFLAGS} -c $<

memtest.o: memtest.c memfuncs.h data.h
	${CC} ${CFLAGS} -c $<

clean:
	rm -f *.o memtest
