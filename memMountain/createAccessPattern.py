#!/usr/bin/env python3

import sys
import random
import argparse
import StatementGenerator

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
#argParser.add_argument("--random",     "-r",
#                       help="Use a random memory access pattern             ",
#                       type=bool, default=False)

argParser.add_argument("--num_elems",  "-e",
                       help="Total number of elements in the array          ",
                       type=int, default=4096)

argParser.add_argument("--num_regs",   "-n",
                       help="Total number of registers to use               ",
                       type=int, default=8)

argParser.add_argument("--random",   "-r",
                       help="If indices should be randomized                ",
                       type=bool, default=False)

argParser.add_argument("--stride",     "-s",
                       help="Stride length when walking operands            ",
                       type=int, default=1)

argParser.add_argument("--stride_2",   "-2",
                       help="Stride length when walking second operand      ",
                       type=int, default=1)

argParser.add_argument("--optypes",    "-t",
                       help="Operand types for operations                   ",
                       type=str, default="reg_to_reg", choices=["reg_to_reg", "reg_to_mem", "mem_to_reg", "mem_to_mem",])

argParser.add_argument("--bench",      "-b",
                       help="The type of benchmark to run                   ",
                       type=str, default="ptr", choices=["ptr", "fma", "add", "mul",])
                       #type=str, default="mem_reg", choices=["reg_reg", "mem_reg", "reg_mem", "mem_mem",])

argParser.add_argument("--operations", "-o",
                       help="Number of operations per iteration             ",
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
numElements     = args.num_elems
numRegsToUse    = args.num_regs
randomIdx       = args.random
opTypes         = args.optypes
stride          = args.stride
stride2         = args.stride_2
benchType       = args.bench
operations      = args.operations
enableIaca      = args.iaca
stalls          = args.stalls

cpuType = getVectISA()
print ("CPU type {0}".format(cpuType))

# create register index functions
#if stalls:
#    destRegIndex = lambda x : (x + 2) % numRegsToUse
#    src1RegIndex = lambda x : (x + 1) % numRegsToUse
#    src2RegIndex = lambda x : x
#else:
#    destRegIndex = lambda x : x
#    src1RegIndex = lambda x : (x + 1) % numRegsToUse
#    src2RegIndex = lambda x : (x + 2) % numRegsToUse

# create index functions
#if randomPattern:
#    arrayOffset = lambda x : random.randrange(0, numElems)
#else:
#    arrayOffset = lambda x : x * 8   # TODO FIXME so this isn't a constant

#writeHeaders(cpuType)

outfile = open("access.h", 'w')
if int(operations % stride) is not 0:
    print ("Warning stride does not evenly divide the number of loads {0} leftover".format(numLoads % stride))

print ("Generating files with the following parameters")
print ("	Stride:                    {0:6}".format(stride      ))
print ("	Number of Elements:        {0:6}".format(numElements ))
print ("    Number of Registers:       {0:6}".format(numRegsToUse))
print ("	Operations per iteration:  {0:6}".format(operations  ))
#print ("	Chunks:                    {0:6}".format(numChunks   ))
print ("    Pipeline stalls:           {0:6}".format(stalls      ))

firstOpGenerator  = None
secondOpGenerator = None
thirdOpGenerator  = None

# if destination is memory
if "_mem" in opTypes:
    firstOpGenerator = StatementGenerator.MemOpGenerator({"opName":"testArray","startIndex":0,"numIndex":numElements,"stride":stride})
else:
    firstOpGenerator = StatementGenerator.RegOpGenerator({"opName":"dest_","startIndex":0,"numIndex":numRegsToUse,"stride":stride})

# if source is memory
if "mem_" in opTypes:
    #secondOpGenerator = StatementGenerator.MemOpGenerator({"opName":"testArray","startIndex":(1 * stride),"numIndex":numElements,"stride":stride})
    thirdOpGenerator  = StatementGenerator.MemOpGenerator({"opName":"testArray","startIndex":(2 * stride),"numIndex":numElements,"stride":stride2,"random":randomIdx})
else:
    #secondOpGenerator = StatementGenerator.RegOpGenerator({"opName":"dest_","startIndex":(1 * stride),"numIndex":numRegsToUse,"stride":stride})
    thirdOpGenerator  = StatementGenerator.RegOpGenerator({"opName":"dest_","startIndex":(2 * stride),"numIndex":numRegsToUse,"stride":stride2,"random":randomIdx})

secondOpGenerator = StatementGenerator.RegOpGenerator({"opName":"dest_","startIndex":(1 * stride),"numIndex":numRegsToUse,"stride":stride})


stmntGenerator = None
if benchType == "fma":
    stmntGenerator = StatementGenerator.FMAStatementGenerator({"firstOpGenerator":firstOpGenerator,
                                                               "secondOpGenerator":secondOpGenerator,
                                                               "thirdOpGenerator":thirdOpGenerator})
elif benchType == "mul":
    stmntGenerator = StatementGenerator.MulStatementGenerator({"firstOpGenerator":firstOpGenerator,
                                                               "secondOpGenerator":secondOpGenerator,
                                                               "thirdOpGenerator":thirdOpGenerator})
elif benchType == "add":
    stmntGenerator = StatementGenerator.AddStatementGenerator({"firstOpGenerator":firstOpGenerator,
                                                               "secondOpGenerator":secondOpGenerator,
                                                               "thirdOpGenerator":thirdOpGenerator})
elif benchType == "ptr":
    stmntGenerator = StatementGenerator.PtrChaseStatementGenerator({"firstOpGenerator":firstOpGenerator,
                                                               "secondOpGenerator":secondOpGenerator,
                                                               "thirdOpGenerator":thirdOpGenerator})

if enableIaca:
    outfile.write('	        IACA_START\n')

for index in range(0, operations):
    outfile.write("\t\t{0}\n".format(stmntGenerator.getNextStatement()))

if enableIaca:
    outfile.write('	        IACA_END\n')
# # Random pattern
# else:
#     for index in range(0, numElems):
#         outfile.write('		asm("vmulsd {0}(%rbx), %%xmm{1}, %%xmm{1}");\n' %
#                       (random.randint(0, numElems) * 8, (regNumber + 1), regNumber))
#         #outfile.write('		asm("movq %%0, %%%%r%s"::"g" (testArray[%s]));\n' %
#         #              (regNumber + 4, random.randint(0, numElems)))
#         regNumber = regNumber + 1
#         if regNumber == numRegsToUse:
#             regNumber = 0
