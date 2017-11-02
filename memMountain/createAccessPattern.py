#!/usr/bin/env python

import sys
import random

if len(sys.argv) < 3 or len(sys.argv) > 6:
    print "Usage:"
    print "%s <stride> <size> <random/strided> <ptr/asm> <loadwidth>" % (sys.argv[0])
    exit()

# largest number of loads we want to do
# too many slows down compiles
maxLoads = 2048
outfile = open("access.h", 'w')

stride = int(sys.argv[1])
size = int(sys.argv[2])
randomPattern = False
asm = True
numIndexesToUse = 8

if len(sys.argv) >= 4:
    if sys.argv[3] == "random":
        randomPattern = True
    elif sys.argv[3] == "strided":
        randomPattern = False
    else:
        print "Did not understand if random or strided, exiting"
        exit()

if len(sys.argv) >= 5:
    if sys.argv[4] == "asm":
        asm = True
    elif sys.argv[4] == "ptr":
        asm = False
    else:
        print "Did not understand if I should do assembly or pointer chasing, exiting"
        exit()

if len(sys.argv) >= 6:
    numIndexesToUse = int(sys.argv[5])    

numLoads = size / stride
numChunks = numLoads / maxLoads
if size % stride is not 0:
    print "Warning stride does not evenly divide size"

if numChunks < 1:
    numChunks = 1
    maxLoads = numLoads
elif numLoads % maxLoads is not 0:
    print "Warning, loads will be split up unevenly"

#print "numChunks %s numLoads %s maxLoads %s" % (numChunks, numLoads, maxLoads)

if asm:
    regNumber = 8
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
else:
    indexNum = 0
    outfile.write('		for (chunkNum = 0; chunkNum < %s; chunkNum++) {\n' % numChunks);
    for index in range(0, maxLoads):
        outfile.write('			index_%s = testArray[index_%s];\n' % (indexNum, indexNum));
        #outfile.write('			printf("%%d \\n", index_%s);\n' % (indexNum));
        #outfile.write('			index_%s = testArray[index_%s];\n' % (indexNum, indexNum));
        #outfile.write('		asm("movq %%0, %%1":"=g" (index_%s):"g" (testArray[index_%s]));\n' % (indexNum, indexNum))
        indexNum = indexNum + 1
        if indexNum is numIndexesToUse:
            indexNum = 0
    outfile.write('		}\n')
 
