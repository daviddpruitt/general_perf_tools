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

def writeHeaders(cpuType):
    """ Generates the architecture specific header.

        This header includes fp registers and the base
        registers used for indexing 
        
        Arguments:
        cpuType -- The floating point extensions the CPU supports
    """
    try:
        outfile = open("archSpecific.h","w")
    except Error as err:
        print("Error opening architecture specific file {0}".format(err))
        exit(1)

    registerPrefix = ""
    baseRegister   = ""

    registerPrefix = "xmm"
    if cpuType is "avx" or "avx2":
        baseRegister   = "rbx"
        registerPrefix = "xmm"
    elif cpuType is "vfpv4":
        baseRegister   = "r12"  # TODO FIXME might not be right register
        registerPrefix = "d"

    outfile.write('#define FP_PREFIX "{0}"\n'.format(registerPrefix))
    outfile.write('#define BASE_REG  "{0}"\n'.format(baseRegister))

    outfile.close()

        
 
argParser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
argParser.add_argument("--stride",     "-s", 
                       help="Stride length when walking array               ", 
                       type=int, default=1)

argParser.add_argument("--random",     "-r", 
                       help="Use a random memory access pattern             ", 
                       type=bool, default=False)

argParser.add_argument("--num_elems",  "-n", 
                       help="Total number of elements in the array          ", 
                       type=int, default=4096)

argParser.add_argument("--num_regs",   "-g", 
                       help="Total number of registers to use               ", 
                       type=int, default=8)

argParser.add_argument("--bench",      "-b", 
                       help="The type of benchmark to run                   ", 
                       type=str, default="mem_reg", choices=["reg_reg", "mem_reg", "reg_mem", "mem_mem",])

argParser.add_argument("--loads_iter", "-l", 
                       help="Number of elements to load per iteration       ", 
                       type=int, default=2048)

argParser.add_argument("--iaca",       "-i",
                       help="Enable Intel IACA marks                        ",
                       type=bool, default=False)

argParser.add_argument("--stalls",     "-p",
                       help="Force pipeline stalls by adding dependencies   ",
                       type=bool, default=False)


args = argParser.parse_args()

# largest number of loads we want to do
# too many slows down compiles
loadsIter = args.loads_iter
benchType = args.bench
enableIaca = args.iaca
randomPattern = args.random
numRegsToUse = args.num_regs
stalls = args.stalls

cpuType = getVectISA()
print ("CPU type {0}".format(cpuType))

# create register index functions
if stalls:
    destRegIndex = lambda x : (x + 2) % numRegsToUse
    src1RegIndex = lambda x : (x + 1) % numRegsToUse
    src2RegIndex = lambda x : x
else:
    destRegIndex = lambda x : x
    src1RegIndex = lambda x : (x + 1) % numRegsToUse
    src2RegIndex = lambda x : (x + 2) % numRegsToUse

writeHeaders(cpuType)

outfile = open("access.h", 'w')

stride = args.stride 
numElems = args.num_elems
numLoads = numElems / stride
if int(numLoads % stride) is not 0:
    print ("Warning stride does not evenly divide the number of loads {0} leftover".format(numLoads % stride))

numChunks = numLoads / loadsIter
if numChunks < 1:
    numChunks = 1
    loadsIter = numLoads
elif numLoads % loadsIter is not 0:
    print ("Warning, %s loads will be split up unevenly among %s loops".format(numLoads, numChunks))

print ("Generating files with the following parameters")
print ("	Stride:               {0:6}".format(stride      ))
print ("	Number of Elements:   {0:6}".format(numElems    ))
print ("        Number of Registers   {0:6}".format(numRegsToUse))
print ("	Loads per iteration:  {0:6}".format(loadsIter   ))
print ("	Chunks:               {0:6}".format(numChunks   ))
print ("        Pipeline stalls:      {0:6}".format(stalls      ))

regNumber = 0
#strided pattern
if randomPattern is False:
    if enableIaca:
        outfile.write('	        IACA_START\n')
        
    for index in range(0, numElems, stride):
        # memory to memory ops
        if benchType == "mem_mem":
            outfile.write('		asm("vmulsd %xmm{0}, %xmm{1}, %xmm{2}");\n'.format(
                           src1RegIndex(regNumber), src2RegIndex(regNumber), destRegIndex(regNumber)))

        # memory to register ops
        if benchType == "mem_reg":
            outfile.write('		asm("vmulsd {0}(%rbx), %xmm{1}, %xmm{2}");\n'.format(
                           index * 8, src1RegIndex(regNumber), destRegIndex(regNumber)))

        # register to register ops
        if benchType == "reg_reg":
            outfile.write('		asm("vmulsd %xmm{0}, %xmm{1}, %xmm{2}");\n'.format(
                           numRegsToUse, src1RegIndex(regNumber), destRegIndex(regNumber)))

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
                 
# elif benchType == "ptr":
#     indexNum = 0
#     outfile.write('		for (chunkNum = 0; chunkNum < %s; chunkNum++) {\n' % numChunks);
#     for index in range(0, loadsIter):
#         outfile.write('			index_%s = testArray[index_%s];\n' % (indexNum, indexNum));
#         #outfile.write('			printf("%%d \\n", index_%s);\n' % (indexNum));
#         #outfile.write('			index_%s = testArray[index_%s];\n' % (indexNum, indexNum));
#         #outfile.write('		asm("movq %%0, %%1":"=g" (index_%s):"g" (testArray[index_%s]));\n' % (indexNum, indexNum))
#         indexNum = indexNum + 1
#         if indexNum is numRegsToUse:
#             indexNum = 0
#     outfile.write('		}\n')
# elif benchType == "instr":
#     regNumber = 0
#     outfile.write('		for (chunkNum = 0; chunkNum < %s; chunkNum++) {\n' % numChunks)
#     #outfile.write('	        IACA_START\n')

#     if cpuType == "avx":
#         loadsIter = loadsIter / 4
#         instr = "vaddpd"
#     if cpuType == "avx2":
#         loadsIter = loadsIter / 8
#         instr = "vfmadd231pd"
#     for index in range(0, loadsIter):
#         outfile.write('			asm("%s %%ymm%s, %%ymm%s, %%ymm%s");\n' % 
#                       (instr, 
#                        (regNumber + 1) % numRegsToUse, 
#                        (regNumber + 2) % numRegsToUse, 
#                        regNumber))
#         regNumber = regNumber + 1

#         if regNumber == numRegsToUse:
#            regNumber = 0
#     outfile.write('		}\n')
#     #outfile.write('		IACA_END\n')
# elif benchType == "instr-load":
#     regNumber = 0
#     outfile.write('		for (chunkNum = 0; chunkNum < %s; chunkNum++) {\n' % numChunks)
#     for index in range(0, loadsIter):
#         outfile.write('			asm("vmulsd %%xmm%s, %%xmm%s, %%xmm%s");\n' % 
#                       ((regNumber + 1) % numRegsToUse,
#                        (regNumber + 2) % numRegsToUse, 
#                        regNumber))
#         regNumber = regNumber + 1

#         if regNumber == numRegsToUse:
#            regNumber = 0
#     outfile.write('		}\n')
                
