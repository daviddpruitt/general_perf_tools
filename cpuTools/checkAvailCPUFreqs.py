#!/usr/bin/env python3

import time
import threading

def burnCycles(seed):
    """ Function that just burns cycles on multiple threads """
    sum = seed + 1
    for number in range(0, 30000000):
        sum = sum * number
    return number

def setSpeeds(minSpeed,maxSpeed):
    """sets speeds back to specified values"""

    # set speeds
    try:
        outputFile = open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq","w")
        outputFile.write("{0}\n".format(maxSpeed))
        outputFile.close()
    except Exception as E:
        print("Error setting max speed of {0}: {1}".format(speed, E))

    try:
        outputFile = open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq","w")
        outputFile.write("{0}\n".format(minSpeed))
        outputFile.close()
    except Exception as E:
        print("Error setting min speed of {0}: {1}".format(speed, E))

# the number of threads used to burn cpu cycles 8 seems like a good middle ground
totalThreads = 8

# get min max speeds
inputFile = open("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq","r")
minSpeed = int(inputFile.readline())
inputFile.close()

inputFile = open("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq","r")
maxSpeed = int(inputFile.readline())
inputFile.close()

# get range of speeds to check
speedsToTest = [x for x in range(minSpeed, maxSpeed + 200000, 100000)]

# where to save the speeds
speedFile = open("speeds.txt", "w")

#print status
print("Min speed {0}".format(minSpeed))
print("Max speed {0}".format(minSpeed))
print("Speeds to check {0}".format(speedsToTest))

# set speeds to original
setSpeeds(minSpeed,maxSpeed)

oldSpeed = 0
# run through each speed
for speed in speedsToTest:
    # set speeds to original
    setSpeeds(minSpeed,maxSpeed)

    # set speeds
    setSpeeds(speed, speed)

    print("Testing speed {0}".format(speed))

    # setup all the threads
    threads = [threading.Thread(target=burnCycles, args={count}) for count in range(0, totalThreads)]

    #start all threads
    for thread in threads:
        thread.start()

    time.sleep(1)

    inputFile = open("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq","r")
    curSpeed = int(inputFile.readline())
    inputFile.close()

    # if the speed has changed by more than 50 Mhz log it
    # this
    if (oldSpeed - 50000 > curSpeed) or (curSpeed > oldSpeed + 50000):
        speedFile.write("{0},{1}\n".format(speed,curSpeed))
        oldSpeed = curSpeed

    inputFile = open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq","r")
    curMinSpeed = int(inputFile.readline())
    inputFile.close()

    inputFile = open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq","r")
    curMaxSpeed = int(inputFile.readline())
    inputFile.close()

    print("Speeds after min: {0:8d} max: {1:8d} cur: {2:8d}".format(curMinSpeed,curMaxSpeed,curSpeed))

    # wait for threads to complete
    for thread in threads:
        thread.join()


# set speeds to original
setSpeeds(minSpeed,maxSpeed)
