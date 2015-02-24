CC_ARM              = arm-linux-gnueabi-gcc
CC_X64				= cc
CFLAGS_ARM			= -static -fomit-frame-pointer -march=armv7-a -mtune=cortex-a9 -mfpu=vfpv3-d16 -mfloat-abi=softfp -O3 -funroll-loops -W -Wall
CFLAGS_X64			= -static -fomit-frame-pointer -march=bdver2  -mtune=bdver2    -mfpmath=both                      -O3 -funroll-loops -W -Wall
CFLAGS_ARM_FPU		= -march=armv7-a -mtune=cortex-a9 -mfpu=vfpv3-d16 -mfloat-abi=softfp -fomit-frame-pointer -O3 -funroll-loops -W -Wall -S -c 
CFLAGS_X64_FPU		= -march=bdver2  -mtune=bdver2    -mfpmath=both                      -fomit-frame-pointer -O3 -funroll-loops -W -Wall -S -c

COMMON_OBJECTS		= PerfCounts.c utility.c randomaccess.c runBench.c
POWER_OBJECTS		= PerfCounts.c utility.c power.c 

all: arm x64

arm:
	${CC_ARM} ${CFLAGS_ARM} ${COMMON_OBJECTS} -o runBenchArm
x64:
	${CC_X64} ${CFLAGS_X64} ${COMMON_OBJECTS} -o runBenchX64
	
powerBenchX64:
	${CC_X64} ${CFLAGS_X64} inc.c ${POWER_OBJECTS} runPowerBench.c -o runPowerIncX64
	${CC_X64} ${CFLAGS_X64} swap.c ${POWER_OBJECTS} runPowerBench.c -o runPowerSwapX64
	${CC_X64} ${CFLAGS_X64} fpma.c ${POWER_OBJECTS} runPowerBench.c -o runPowerFPMA
	${CC_X64} ${CFLAGS_X64} fpadd.c ${POWER_OBJECTS} runPowerBench.c -o runPowerFADD
	${CC_X64} ${CFLAGS_X64} multFP.c ${POWER_OBJECTS} runPowerBench.c -o runPowerFMUL
	mv runPowerIncX64 runPowerSwapX64 runPowerFPMA runPowerFADD runPowerFMUL ./bin
	
powerBenchArm: assyOnly
	#${CC_ARM} ${CFLAGS_ARM} inc.c ${POWER_OBJECTS} runPowerBenchAll.c -o runPowerIncAllArm
	#${CC_ARM} ${CFLAGS_ARM} inc.c ${POWER_OBJECTS} runPowerBench.c -o runPowerIncArm
	#${CC_ARM} ${CFLAGS_ARM} swap.c ${POWER_OBJECTS} runPowerBench.c -o runPowerSwapArm
	#${CC_ARM} ${CFLAGS_ARM} mult.c ${POWER_OBJECTS} runPowerBench.c -o runPowerMultArm
	#${CC_ARM} ${CFLAGS_ARM} incFP.c ${POWER_OBJECTS} runPowerBench.c -o runPowerIncFPArm
	#${CC_ARM} ${CFLAGS_ARM} multFP.c ${POWER_OBJECTS} runPowerBench.c -o runPowerMultFPArm
	${CC_ARM} ${CFLAGS_ARM} fpma.o ${POWER_OBJECTS} runPowerBench.c -o runPowerFPMA
	${CC_ARM} ${CFLAGS_ARM} fpadd.c ${POWER_OBJECTS} runPowerBench.c -o runPowerFADD
	#${CC_ARM} ${CFLAGS_ARM} fpma.c ${POWER_OBJECTS} runPowerBench.c -o runPowerFPMA2
	
assyOnlyARM:
	${CC_ARM} ${CFLAGS_ARM_FPU} fpma.c
	arm-linux-gnueabi-as -o fpma.o fpma.s

assyOnlyX64:
	${CC_X64} ${CFLAGS_X64_FPU} fpma.c
	as -o fpma.o fpma.s
	
installArm: powerBenchArm
	adb push ./runPowerIncAllArm /data/arm
	adb push ./runPowerIncArm /data/arm
	adb push ./runPowerSwapArm /data/arm
	adb push ./runPowerMultArm /data/arm
	adb push ./runPowerIncFPArm /data/arm
	adb push ./runPowerMultFPArm /data/arm
	adb push ./runPowerFPMA /data/arm
	adb push ./runPowerFPAD /data/arm
	
clean:
	rm -f *.o *~ runBenchX64 runBenchArm runPower*
