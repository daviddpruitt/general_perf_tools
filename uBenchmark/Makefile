ARM_CC=arm-linux-gnueabi-gcc 
ARM_CFLAGS=-static -fomit-frame-pointer -march=armv7-a -mtune=cortex-a9 -mfpu=vfpv3-d16 -mfloat-abi=softfp -O3 -funroll-loops -W -Wall 
X64_CC=gcc
X64_CFLAGS=-fomit-frame-pointer -march=bdver2  -mtune=bdver2    -mfpmath=both                      -O3 -funroll-loops -W -Wall

CFLAGS=$(ARM_CFLAGS)
CC=$(ARM_CC)
LDFLAGS=$(CFLAGS)
COMMONOBJ=doMany.o utility.o PerfCounts.o

MULADD=1
ADD=2
MUL=3
MAXPOWER=4
BCOPYSIZE=1024
KB64=65000 #65536 is 64kb
MB=525000  #524288 is 1/2 meg
L1SIZE=32768
L2SIZE=1048576
MAXSIZE=10485760
CACHELINE=4	#size (in doubles) of a cacheline

fpmaMain-add: fpmaMain-add.o $(COMMONOBJ) 
	$(CC) $(LDFLAGS) -o $@ $^ 
fpmaMain-muladd: fpmaMain-muladd.o
	$(CC) $(LDFLAGS) -o $@ $^ $(COMMONOBJ)
fpmaMain-mul: fpmaMain-mul.o $(COMMONOBJ) 
	$(CC) $(LDFLAGS) -o $@ $^ 
copyMain-small: copyMain-small.o utility.o doManyCopy.o 
	$(CC) $(LDFLAGS) -o $@ $^ 
copyMain-med: copyMain-med.o utility.o doManyCopy.o 
	$(CC) $(LDFLAGS) -o $@ $^ 
copyMain-large: copyMain-large.o utility.o doManyCopy.o 
	$(CC) $(LDFLAGS) -o $@ $^ 
badLocality-small: badLocality-small.o utility.o
	$(CC) $(LDFLAGS) -o $@ $^ 
badLocality-med: badLocality-med.o utility.o
	$(CC) $(LDFLAGS) -o $@ $^ 
badLocality-large: badLocality-large.o utility.o
	$(CC) $(LDFLAGS) -o $@ $^ 
fpmaMain-maxPowerMain: fpmaMain-maxPowerMain.o $(COMMONOBJ) 
	$(CC) $(LDFLAGS) -o $@ $^ 
fpmaMain-maxPowerL2: fpmaMain-maxPowerL2.o $(COMMONOBJ) 
	$(CC) $(LDFLAGS) -o $@ $^ 

fpmaMain-add.o: fpmaMain.c
	$(CC) -c fpmaMain.c -DOP=$(ADD) $(CFLAGS) -o fpmaMain-add.o

fpmaMain-muladd.o: fpmaMain.c
	$(CC) -c fpmaMain.c -DOP=$(MULADD) $(CFLAGS) -o fpmaMain-muladd.o

fpmaMain-mul.o: fpmaMain.c
	$(CC) -c fpmaMain.c -DOP=$(MUL) $(CFLAGS) -o fpmaMain-mul.o
	
copyMain-small.o: copyMain.c
	$(CC) -c copyMain.c -DSIZE=$(BCOPYSIZE) -DSTRIDE=0 -DMAXSIZE=$(L1SIZE) $(CFLAGS) -o copyMain-small.o
	
copyMain-med.o: copyMain.c
	$(CC) -c copyMain.c -DSIZE=$(BCOPYSIZE) -DSTRIDE=$(KB64) -DMAXSIZE=$(L2SIZE) $(CFLAGS) -o copyMain-med.o
	
copyMain-large.o: copyMain.c
	$(CC) -c copyMain.c -DSIZE=$(BCOPYSIZE) -DSTRIDE=$(MB) -DMAXSIZE=$(MAXSIZE) $(CFLAGS) -o copyMain-large.o

badLocality-small.o: badLocality.c
	$(CC) -c badLocality.c -DSIZETOTEST=$(L1SIZE) $(CFLAGS) -o badLocality-small.o
	
badLocality-med.o: badLocality.c
	$(CC) -c badLocality.c -DSIZETOTEST=$(L2SIZE) $(CFLAGS) -o badLocality-med.o
	
badLocality-large.o: badLocality.c
	$(CC) -c badLocality.c -DSIZETOTEST=$(MAXSIZE) $(CFLAGS) -o badLocality-large.o
	
fpmaMain-maxPowerMain.o: fpmaMain.c
	$(CC) -c fpmaMain.c -DOP=$(MAXPOWER) -DARRAYSIZE=$(MAXSIZE) -DSTRIDE=$(MB) -DSTRIDE_ADJ=$(CACHELINE) $(CFLAGS) -o fpmaMain-maxPowerMain.o

fpmaMain-maxPowerL2.o: fpmaMain.c
	$(CC) -c fpmaMain.c -DOP=$(MAXPOWER) -DARRAYSIZE=$(L2SIZE) -DSTRIDE=$(KB64) -DSTRIDE_ADJ=$(CACHELINE) $(CFLAGS) -o fpmaMain-maxPowerL2.o
	

fpmaMain-add.s: fpmaMain.c
	$(CC) -S fpmaMain.c -DOP=$(ADD) $(CFLAGS) -o fpmaMain-add.s

fpmaMain-muladd.s: fpmaMain.c
	$(CC) -S fpmaMain.c -DOP=$(MULADD) $(CFLAGS) -o fpmaMain-muladd.s

fpmaMain-mul.s: fpmaMain.c
	$(CC) -S fpmaMain.c -DOP=$(MUL) $(CFLAGS) -o fpmaMain-mul.s
	
copyMain-small.s: copyMain.c
	$(CC) -S copyMain.c -DSIZE=$(BCOPYSIZE) -DSTRIDE=0 $(CFLAGS) -o copyMain-small.s

X64all: fpmaMain-add fpmaMain-mul fpmaMain-muladd copyMain-small copyMain-med copyMain-large fpmaMain-maxPowerMain fpmaMain-maxPowerL2

ARMall: fpmaMain-add fpmaMain-mul fpmaMain-muladd copyMain-small copyMain-med copyMain-large fpmaMain-maxPowerMain fpmaMain-maxPowerL2
#ARMall: fpmaMain-maxPower
	

X64: clean X64all
	mv fpmaMain-add fpmaMain-mul fpmaMain-muladd copyMain-small copyMain-med copyMain-large fpmaMain-maxPowerMain fpmaMain-maxPowerL2 ./x64bin

ARM: clean ARMall
	mv fpmaMain-add fpmaMain-mul fpmaMain-muladd copyMain-small copyMain-med copyMain-large fpmaMain-maxPowerMain fpmaMain-maxPowerL2 ./armbin
	
pushAll: ARM
	adb push ./armbin/fpmaMain-add /data/arm
	adb push ./armbin/fpmaMain-mul /data/arm
	adb push ./armbin/fpmaMain-muladd /data/arm
	adb push ./armbin/copyMain-small /data/arm
	adb push ./armbin/copyMain-med /data/arm
	adb push ./armbin/copyMain-large /data/arm
	adb push ./armbin/fpmaMain-maxPowerMain /data/arm
	adb push ./armbin/fpmaMain-maxPowerL2 /data/arm

clean:
	rm -f fpmaMain-* copyMain-* badLocality-* *.o

