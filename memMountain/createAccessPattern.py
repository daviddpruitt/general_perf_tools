#!/usr/bin/env python

import sys
import random
import argparse

def getVectISA():
    ''' Get what Vector instructions the CPU supports '''
    try:
        cpuFile = open("/proc/cpuinfo", 'r')
    except Exception as err:
        print "Unable to open CPU info file: %s\nAssuming AVX support" % err
        return "avx"

    for line in cpuFile:
        if "avx2" in line:
            return "avx2"
        if "avx" in line:
            return "avx"
        if "vfpv4" in line:
            return "vfpv4"

    
argParser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
argParser.add_argument("--stride",     "-s", 
                       help="Stride length when walking array          ", 
                       type=int, default=1)

argParser.add_argument("--random",     "-r", 
                       help="Use a random memory access pattern        ", 
                       type=bool, default=False)

argParser.add_argument("--num_elems",  "-n", 
                       help="Total number of elements in the array     ", 
                       type=int, default=8192)

argParser.add_argument("--bench",      "-b", 
                       help="The type of benchmark to run              ", 
                       type=str, default="ptr", choices=["ptr","asm","instr","instr-load"])

argParser.add_argument("--loads_iter", "-l", 
                       help="Number of elements to load per iteration  ", 
                       type=int, default=2048)

args = argParser.parse_args()

# largest number of loads we want to do
# too many slows down compiles
loadsIter = args.loads_iter
benchType = args.bench
randomPattern = args.random
numIndexesToUse = 8

cpuType = getVectISA()
print "CPU type %s" % cpuType

outfile = open("access.h", 'w')

stride = args.stride 
numElems = args.num_elems
numLoads = numElems / stride
if numLoads % stride is not 0:
    print "Warning stride does not evenly divide the number of loads"

numChunks = numLoads / loadsIter
if numChunks < 1:
    numChunks = 1
    loadsIter = numLoads
elif numLoads % loadsIter is not 0:
    print "Warning, %s loads will be split up unevenly among %s loops" % (numLoads, numChunks)

#print "numChunks %s numLoads %s maxLoads %s" % (numChunks, numLoads, maxLoads)

if benchType == "asm":
    regIndex = 0
    #strided pattern
    if randomPattern is False:
        for index in range(0, size, stride):
            outfile.write('		asm("movq %%0, %%%%r%s"::"g" (testArray[%s]));\n' % 
                          (regNumber, index))
            #outfile.write("		dest = testArray[%s];\n" % index)
            if regNumber is 11:
                regNumber = 8
            else:
                regNumber = regNumber + 1
    # Random pattern
    else:
        for index in range(0, size):
            outfile.write('		asm("movq %%0, %%%%r%s"::"g" (testArray[%s]));\n' %
                          (regNumber, random.randint(0, size)))
            if regNumber is 11:
                regNumber = 8
            else:
                regNumber = regNumber + 1 
elif benchType == "ptr":
    indexNum = 0
    outfile.write('		for (chunkNum = 0; chunkNum < %s; chunkNum++) {\n' % numChunks);
    for index in range(0, loadsIter):
        outfile.write('			index_%s = testArray[index_%s];\n' % (indexNum, indexNum));
        #outfile.write('			printf("%%d \\n", index_%s);\n' % (indexNum));
        #outfile.write('			index_%s = testArray[index_%s];\n' % (indexNum, indexNum));
        #outfile.write('		asm("movq %%0, %%1":"=g" (index_%s):"g" (testArray[index_%s]));\n' % (indexNum, indexNum))
        indexNum = indexNum + 1
        if indexNum is numIndexesToUse:
            indexNum = 0
    outfile.write('		}\n')
elif benchType == "instr":
    regIndex = 0
    outfile.write('		for (chunkNum = 0; chunkNum < %s; chunkNum++) {\n' % numChunks)
    #outfile.write('	        IACA_START\n')

    if cpuType == "avx":
        loadsIter = loadsIter / 4
        instr = "vaddpd"
    if cpuType == "avx2":
        loadsIter = loadsIter / 8
        instr = "vfmadd231pd"
    for index in range(0, loadsIter):
        outfile.write('			asm("%s %%ymm%s, %%ymm%s, %%ymm%s");\n' % 
                      (instr, 
                       (regIndex + 1) % numIndexesToUse, 
                       (regIndex + 2) % numIndexesToUse, 
                       regIndex))
        regIndex = regIndex + 1

        if regIndex == numIndexesToUse:
           regIndex = 0
    outfile.write('		}\n')
    #outfile.write('		IACA_END\n')
elif benchType == "instr-load":
    regIndex = 0
    outfile.write('		for (chunkNum = 0; chunkNum < %s; chunkNum++) {\n' % numChunks)
    for index in range(0, loadsIter):
        outfile.write('			asm("mulsd %%xmm%s, %%xmm%s");\n' % 
                      ((regIndex + 1) % numIndexesToUse, 
                       regIndex))
        regIndex = regIndex + 1

        if regIndex == numIndexesToUse:
           regIndex = 0
    outfile.write('		}\n')
                
