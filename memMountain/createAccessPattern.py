#!/usr/bin/env python3

import sys
import random
import argparse

def getVectISA():
    ''' Get what Vector instructions the CPU supports '''
    try:
        cpuFile = open("/proc/cpuinfo", 'r')
    except Exception as err:
        print ("Unable to open CPU info file: %s\nAssuming AVX support".format(err))
        return "avx"

    for line in cpuFile:
        if "avx2" in line:
            return "avx2"
        if "avx" in line:
            return "avx"
        if "vfpv4" in line:
            return "vfpv4"

def writeRegAssignHeaders(cpuType, numRegistersToUse, benchType):
    # create empty files if bench type isn't asm
    if benchType != "asm":
        outfile = open("assignRegs.h","w")
        outfile.close()
        outfile = open("assignDest.h","w")
        outfile.close()
        return

    # first write stuff into registers
    registerPrefix = "xmm"
    if cpuType is "avx" or "avx2":
        registerPrefix = "xmm"
    elif cpuType is "vfpv4":
        registerPrefix = "d"

    # write var to reg header
    outfile = open("assignRegs.h","w")
    outfile.write('		asm(')
    instructions = ""
    operands     = ""
    for regNumber in range(0, numRegistersToUse):
        instructions = instructions + str('"  vmovsd %%%s, %%%%%s\\n"\n		    ' % 
                                          (str(regNumber), 
                                           registerPrefix + str(regNumber)))
        operands = operands + str('		           "m" (%s),\n' % 
                                  ("dest_" + str(regNumber)))
        #outfile.write('		asm("vmovsd %%0, %%%%%s"::"m" (%s):);\n' % 
        #              (registerPrefix + str(regNumber), 
        #               'dest_' + str(regNumber) ))
    instructions = instructions[:-7]
    operands = operands[:-2]
    outfile.write(instructions)
    outfile.write('::\n')
    outfile.write(operands)
    outfile.write(':);\n')
    outfile.close()

    # write reg to var header
    outfile = open("assignDest.h","w")
    outfile.write('		asm(')
    instructions = ""
    operands     = ""
    for regNumber in range(0, numRegistersToUse):
        #outfile.write('		asm("vmovsd %%%%%s, %%0":"=m" (%s)::);\n' % 
        #              (registerPrefix + str(regNumber), 
        #               'dest_' + str(regNumber) ))
        instructions = instructions + str('"  vmovsd %%%%%s, %%%s\\n"\n		    ' %
                                          (registerPrefix + str(regNumber), 
                                           regNumber))
        operands = operands + str('		           "=m" (%s),\n' % 
                                  ("dest_" + str(regNumber)))
    instructions = instructions[:-7]
    operands = operands[:-2]
    outfile.write(instructions)
    outfile.write(':\n')
    outfile.write(operands)
    outfile.write('::);\n')
    outfile.close()

        
 
argParser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
argParser.add_argument("--stride",     "-s", 
                       help="Stride length when walking array          ", 
                       type=int, default=1)

argParser.add_argument("--random",     "-r", 
                       help="Use a random memory access pattern        ", 
                       type=bool, default=False)

argParser.add_argument("--num_elems",  "-n", 
                       help="Total number of elements in the array     ", 
                       type=int, default=4096)

argParser.add_argument("--bench",      "-b", 
                       help="The type of benchmark to run              ", 
                       type=str, default="ptr", choices=["ptr","asm","instr","instr-load"])

argParser.add_argument("--loads_iter", "-l", 
                       help="Number of elements to load per iteration  ", 
                       type=int, default=2048)

argParser.add_argument("--iaca",      "-i",
                       help="Enable Intel IACA marks                   ",
                       type=bool, default=False)

args = argParser.parse_args()

# largest number of loads we want to do
# too many slows down compiles
loadsIter = args.loads_iter
benchType = args.bench
enableIaca = args.iaca
randomPattern = args.random
numRegsToUse = 8

cpuType = getVectISA()
print ("CPU type {0}".format(cpuType))

writeRegAssignHeaders(cpuType, numRegsToUse, benchType)

#exit()

outfile = open("access.h", 'w')

stride = args.stride 
numElems = args.num_elems
numLoads = numElems / stride
if numLoads % stride is not 0:
    print ("Warning stride does not evenly divide the number of loads")

numChunks = numLoads / loadsIter
if numChunks < 1:
    numChunks = 1
    loadsIter = numLoads
elif numLoads % loadsIter is not 0:
    print ("Warning, %s loads will be split up unevenly among %s loops".format(numLoads, numChunks))

print ("Generating files with the following parameters")
print ("	Stride:               {0:6}".format(stride    ))
print ("	Number of Elements:   {0:6}".format(numElems ))
print ("	Loads per iteration:  {0:6}".format(loadsIter))
print ("	Chunks:               {0:6}".format(numChunks))



#print "numChunks %s numLoads %s maxLoads %s" % (numChunks, numLoads, maxLoads)

if benchType == "asm":
    regNumber = 0
    #strided pattern
    if randomPattern is False:
        if enableIaca:
            outfile.write('	        IACA_START\n')
        
        for index in range(0, numElems, stride):
            outfile.write('		dest_%s = dest_%s * testArray[%s];\n' % 
                          (regNumber,
                           (regNumber + 1) % numRegsToUse,
                           index))
            #outfile.write('		asm("vmulsd %s(%%rsi), %%xmm%s, %%xmm%s");\n' % 
            #              ((index * 8), (regNumber + 1), regNumber))

            regNumber = regNumber + 1
            if regNumber == numRegsToUse:
                regNumber = 0
        
        if enableIaca:
            outfile.write('	        IACA_END\n')
    # Random pattern
    else:
        for index in range(0, numElems):
            outfile.write('		asm("vmulsd %s(%%rsi), %%xmm%s, %%xmm%s");\n' % 
                          (random.randint(0, numElems) * 8, (regNumber + 1), regNumber))
            #outfile.write('		asm("movq %%0, %%%%r%s"::"g" (testArray[%s]));\n' %
            #              (regNumber + 4, random.randint(0, numElems)))
            regNumber = regNumber + 1
            if regNumber == numRegsToUse:
                regNumber = 0
                 
elif benchType == "ptr":
    indexNum = 0
    outfile.write('		for (chunkNum = 0; chunkNum < %s; chunkNum++) {\n' % numChunks);
    for index in range(0, loadsIter):
        outfile.write('			index_%s = testArray[index_%s];\n' % (indexNum, indexNum));
        #outfile.write('			printf("%%d \\n", index_%s);\n' % (indexNum));
        #outfile.write('			index_%s = testArray[index_%s];\n' % (indexNum, indexNum));
        #outfile.write('		asm("movq %%0, %%1":"=g" (index_%s):"g" (testArray[index_%s]));\n' % (indexNum, indexNum))
        indexNum = indexNum + 1
        if indexNum is numRegsToUse:
            indexNum = 0
    outfile.write('		}\n')
elif benchType == "instr":
    regNumber = 0
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
                       (regNumber + 1) % numRegsToUse, 
                       (regNumber + 2) % numRegsToUse, 
                       regNumber))
        regNumber = regNumber + 1

        if regNumber == numRegsToUse:
           regNumber = 0
    outfile.write('		}\n')
    #outfile.write('		IACA_END\n')
elif benchType == "instr-load":
    regNumber = 0
    outfile.write('		for (chunkNum = 0; chunkNum < %s; chunkNum++) {\n' % numChunks)
    for index in range(0, loadsIter):
        outfile.write('			asm("vmulsd %%xmm%s, %%xmm%s, %%xmm%s");\n' % 
                      ((regNumber + 1) % numRegsToUse,
                       (regNumber + 2) % numRegsToUse, 
                       regNumber))
        regNumber = regNumber + 1

        if regNumber == numRegsToUse:
           regNumber = 0
    outfile.write('		}\n')
                
