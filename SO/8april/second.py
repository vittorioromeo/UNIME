#!/usr/bin/python2

# Copyright (c) 2015 Vittorio Romeo
# License: MIT License | http://opensource.org/licenses/MIT
# http://vittorioromeo.info | vittorio.romeo@outlook.com

# Import required libraries
import random
import copy
import os
import sys

# Generated a random number between 'mMin' and 'mMax'
# 'mMin' is inclusive, 'mMax' is exclusive
def rndI(mMin, mMax):
    return random.randint(mMin, mMax - 1)

# Get the current console window width and height
# Required to draw the ASCII graph properly
consoleHeight, consoleWidth = os.popen('stty size', 'r').read().split()
consoleHeight = int(consoleHeight)
consoleWidth = int(consoleWidth)

# Set reasonable values for the ASCII memory graph
graphHeight = 5
graphWidth = consoleWidth

# Open a log file stream
logFile = open("log.txt", "w")

# Define a class that holds console colors for prettier output
class Color:
    black =     '\033[30m'
    red =       '\033[31m'
    green =     '\033[32m'
    yellow =    '\033[33m'
    blue =      '\033[34m'
    purple =    '\033[35m'
    cyan =      '\033[36m'
    lgray =     '\033[37m'

    dgray =     '\033[90m'
    lred =      '\033[91m'
    lgreen =    '\033[92m'
    lyellow =   '\033[93m'
    lblue =     '\033[94m'
    lpurple =   '\033[95m'
    lcyan =     '\033[96m'
    lwhite =    '\033[97m'

    default =   '\033[39m'

# Set current color
def loC(x):
    sys.stdout.write(x)

# Reset current color
def loCD():
    sys.stdout.write(Color.default)

# Print both to log and console
def lo(x):
    logFile.write(x + "\n")
    sys.stdout.write(x)

# Print a separator line
def loLn():
    loC(Color.dgray)
    lo("_" * consoleWidth)
    loCD()
    lo("\n")

# Print separator line and newline
def loSep():
    loLn()
    lo("")

# Get integer input from user - retry in case of error
def getInputInt():
    while True:
        try:
            result = int(raw_input())
        except ValueError:
            loC(Color.red)
            lo("\nInserted value invalid - retry")
            loCD()
        else:
            break

    return result

# Returns the count of digits of the number 'mX'
def getDigitCount(mX):
    return len(str(mX))

# Class representing a block of memory
class Block:
    # Constructor
    def __init__(self, mAllocator, mStart, mEnd):
        # Allocator that owns the block
        self.allocator = mAllocator

        # Byte where the memory block begins
        self.start = mStart

        # Byte where the memory block ends
        self.end = mEnd

        # Is the memory block currently occupied?
        self.occupied = False

    # Returns the size of the memory block in bytes
    def getSize(self):
        return self.end - self.start

# Class representing the results of an algorithm
class AlgorithmResult:
    # Constructor
    def __init__(self, mSuccess = False, mCost = 0, mBlock = None):
        # 'True' if the algorithm succedeed
        self.success = mSuccess

        # Number of steps taken by the algorithm
        self.cost = mCost

        # Block occupied in case of success
        self.block = mBlock

    # Prints the data to console and log
    def log(self):
        loC(Color.green)
        lo("Success: ")
        loCD()
        lo(str(self.success) + "\n")

        loC(Color.blue)
        lo("Cost: ")
        loCD()
        lo(str(self.cost) + "\n")

        lo("\n")

# Class representing a memory allocator
class Allocator:
    # Constructor
    def __init__(self, mSize):
        # Total size of the allocator
        self.size = mSize

        # Reset state
        self.reset()

    # Restores the allocator to its original state
    def reset(self):
        # List of the blocks owned by the allocator
        self.blocks = []

        # Sorted list of block ids by size
        self.sortedBlocks = []

        # Last occupied block index
        self.lastIdx = 0

        # Initialize the allocator a single memory block including 'self.size'
        firstBlock = Block(self, 0, self.size)
        self.blocks.append(firstBlock)
        self.sortedBlocks.append(firstBlock)

    # Return a tuple containing the block that includes the byte 'mX' and its index
    def getBlockAt(self, mX):
        # Loop through all the blocks...
        for i, b in enumerate(self.blocks):
            # If the current block ends before 'mX', skip it
            if b.end < mX:
                continue

            # Return the first suitable block
            return (b, i)

        # If no block was found, 'mX' is an invalid value
        raise Exception("'mX' is an invalid value")

    # Inserts a block in the sorted list, in the correct position
    def insertSorted(self, mX):
        i = 0
        for i in range(0, len(self.sortedBlocks)):
            c = self.sortedBlocks[i]

            if c.getSize() >= mX.getSize():
                break

        self.sortedBlocks.insert(i, mX)

    # Splits the memory owned by the allocator at the byte 'mX'
    # Returns a tuple containing the two halves in which the block was split and the
    # index of the first half
    def splitAt(self, mX):
        # The block we're going to split is the block including 'mX'
        toSplit, toSplitIdx = self.getBlockAt(mX)

        # If the block is occupied, throw an exception
        if toSplit.occupied:
            raise Exception("Cannot split occupied block")

        # To split a block in a specific byte location, we're gonna
        # resize it to end at the specific byte location, then we're
        # going to create an additional block to fill the empty space

        # 'l' is the beginning of the first block
        l = toSplit.start

        # 'm' is the end of the first block and the beginning of the second block
        m = mX

        # 'r' is the end of the second block
        r = toSplit.end

        # Remove the old block from the sorted list
        self.sortedBlocks.remove(toSplit)

        # Resize the first block to [l, m)
        toSplit.start = l
        toSplit.end = m

        # Create an additional block including [m, r)
        newBlock = Block(self, m, r)
        self.blocks.insert(toSplitIdx + 1, newBlock)

        # Refresh the sorted list
        self.insertSorted(toSplit)
        self.insertSorted(newBlock)

        # Return a tuple containing the two halves and the index of the first half
        return (toSplit, newBlock, toSplitIdx)

    # Merges adjacent unoccupied blocks
    def reclaim(self):
        # If we have less than two blocks, there's nothing to merge
        if len(self.blocks) < 2:
            return

        # Loop through all the blocks but the last...
        for i in range(0, len(self.blocks) - 1):
            # Keep track of the current two adjacent blocks
            b1 = self.blocks[i]
            b2 = self.blocks[i + 1]

            # If the first one is occupied, step forward
            if b1.occupied:
                continue

            # If the second one is occupied, step forward twice
            if b2.occupied:
                i += 1
                continue

            # If both blocks are unoccupied, merge them by removing
            # the second one and resizing the first one to fill the empty space
            b1.end = b2.end
            self.sortedBlocks.remove(b2)
            self.blocks.remove(b2)

            # Call this function recursively if reclaiming was successful
            return self.reclaim()

    # Free the memory in all the blocks, making them unoccupied
    def free(self):
        for b in self.blocks:
            b.occupied = False

    # Print an ASCII graph of the state of the allocator
    def printInfo(self):
        # Size of a graph unit
        graphStep = self.size / (graphWidth - 5 - (len(self.blocks) * 2))

        aStr = "0"
        for i in range(0, self.size / graphStep):
            aStr += " "

        aStr += str(self.size)

        bStr = ""
        pStr = ""
        iStr = ""

        lastIdx = 0

        for b in self.blocks:
            bStr += Color.dgray + "|" + Color.default
            pStr += Color.dgray + "|" + Color.default

            if lastIdx == 0:
                iStr += "0"
            else:
                iStr += "|"

            for i in range(0, b.getSize() / graphStep):
                if b.occupied:
                    bStr += Color.cyan + "#" + Color.default
                else:
                    bStr += Color.dgray + "_" + Color.default

                pStr += " "
                iStr += " "

            if b.occupied:
                bStr += Color.cyan + "#" + Color.default
            else:
                bStr += Color.dgray + "_" + Color.default

            pStr += " "
            iStr = iStr[:-getDigitCount(b.end) + 1]
            iStr += str(b.end)

            lastIdx += 1

        bStr += Color.dgray + "|\n" + Color.default
        pStr += Color.dgray + "|\n" + Color.default
        iStr += "|\n"

        lo("\n")
        lo("\n")
        lo("\n")
        lo(pStr)
        lo(pStr)
        lo(bStr)
        lo(iStr)
        lo(pStr)
        lo(pStr)

    # Return a number representing the average fragmentation of the allocator
    def getFragmentation(self):
        if self.blocks == False:
            return 0

        result = 0
        lastOcc = self.blocks[0].occupied

        for i in range(1, len(self.blocks)):
            b = self.blocks[i]

            if lastOcc != b.occupied:
                lastOcc = b.occupied
                result += 1

        return result

    # Execute an algorithm and print its result
    def executeAlgorithm(self, mAlgorithm, mX):
        lo("\n")
        result = AlgorithmResult()

        mAlgorithm(result, mX)

        result.log()
        return result

    # Function that's called when an algorithm succeeds
    def successOn(self, result, b, mX):
        l, r, idx = self.splitAt(b.start + mX)
        l.occupied = True
        result.success = True
        result.block = b
        self.lastIdx = idx

    # Implementation: first-fit algorithm
    def implFirstFit(self, result, mX):
        for b in self.blocks:
            result.cost += 1

            if b.occupied == False and b.getSize() >= mX:
                self.successOn(result, b, mX)
                return

    # Implementation: best-fit algorithm (naive)
    def implBestFitNaive(self, result, mX):
        best = None
        for b in self.blocks:
            result.cost += 1

            if (best == None or b.getSize() < best.getSize()) and b.getSize() >= mX and b.occupied == False:
                best = b

        if best != None:
            self.successOn(result, best, mX)

    # Implementation: worst-fit algorithm (naive)
    def implWorstFitNaive(self, result, mX):
        best = None
        for b in self.blocks:
            result.cost += 1

            if (best == None or b.getSize() < best.getSize()) and b.getSize() >= mX and b.occupied == False:
                best = b

        if best != None:
            self.successOn(result, best, mX)

    # Implementation: next-fit algorithm
    def implNextFit(self, result, mX):
        for i in range(0, len(self.blocks)):
            result.cost += 1

            b = self.blocks[(i + self.lastIdx + 1) % len(self.blocks)]

            if b.occupied or b.getSize() < mX:
                continue

            self.successOn(result, b, mX)
            break

    # Implementation: best-fit algorithm (sorted list)
    def implBestFitSL(self, result, mX):
        for b in self.sortedBlocks:
            result.cost += 1

            if b.getSize() >= mX and b.occupied == False:
                self.successOn(result, b, mX)
                break

    # Implementation: worst-fit algorithm (sorted list)
    def implWorstFitSL(self, result, mX):
        for i in range(0, len(self.sortedBlocks)):
            result.cost += 1

            b = self.sortedBlocks[len(self.sortedBlocks) - i - 1]

            if b.getSize() >= mX and b.occupied == False:
                self.successOn(result, b, mX)
                break

    # The "first fit" algorithm simply iterates over all blocks
    # until it finds a block which is suitable for the desired
    # memory allocation
    def insertFirstFit(self, mX):
        return self.executeAlgorithm(self.implFirstFit, mX)

    # The "best fit" algorithm iterates over all available blocks
    # and selects the one that wastes less space for the memory allocation
    # This algorithm could be improved by keeping a separate list of memory blocks,
    # ordered by size
    def insertBestFitNaive(self, mX):
        return self.executeAlgorithm(self.implBestFitNaive, mX)

    # The "next fit" algorithm uses the same logic as the "first fit" algorithm,
    # but starts looking for an unoccupied block at the last block index
    def insertNextFit(self, mX):
        return self.executeAlgorithm(self.implNextFit, mX)

    # The "worst fit" algorithm iterates over all available blocks
    # and selects the one that wastes most space for the memory allocation
    # This algorithm could be improved by keeping a separate list of memory blocks,
    # ordered by size
    def insertWorstFitNaive(self, mX):
        return self.executeAlgorithm(self.implWorstFitNaive, mX)

    # The "best fit" algorithm starts from the beginning of the sorted list
    def insertBestFitSL(self, mX):
        return self.executeAlgorithm(self.implBestFitSL, mX)

    # The "worst fit" algorithm starts from the end of the sorted list
    def insertWorstFitSL(self, mX):
        return self.executeAlgorithm(self.implWorstFitSL, mX)

# Gets a valid memory size integer from the user
def getInputMemorySize(mMax):
    loCD()
    lo("\nInsert requested ")
    loC(Color.cyan)
    lo("memory size: ")
    loCD()

    while True:
        result = getInputInt()

        if result <= 0 or result >= mMax:
            loC(Color.red)
            lo("Invalid memory size value\n")
            loCD()
            continue

        return result

# Get a valid simulation count fro the user
def getInputSimulationCount():
    loCD()
    lo("\nInsert number of ")
    loC(Color.cyan)
    lo("simulations: ")
    loCD()
    numSim = int(raw_input())
    lo("\n")

    return numSim


# Class representing a possible menu choice
class Choice:
    # Constructor
    def __init__(self, mTitle):
        self.number = -1
        self.title = mTitle
        self.action = None

    # Print menu choice
    def loChoice(self):
        lo("\t")
        loC(Color.cyan)
        lo(str(self.number) + ". ")
        loC(Color.default)
        lo(self.title)
        lo("\n")

    # Print selection
    def loSelection(self):
        loC(Color.cyan)
        lo(self.title)
        loCD()
        lo(" selected\n")

# Class representing the command line menu
class Menu:
    # Constructor
    def __init__(self):
        self.choices = []
        self.nextNumber = 0

    # Insert a choice in the menu
    def add(self, mTitle, mAction):
        c = Choice(mTitle)
        c.action = mAction
        c.number = self.nextNumber

        self.choices.append(c)
        self.nextNumber += 1

    # Print all possible choices
    def loAllChoices(self):
        lo("\nChoose:\n")
        for c in self.choices:
            c.loChoice()

    # Select one of the choice from user input
    def selectChoice(self):
        inputNumber = getInputInt()
        c = self.choices[inputNumber]
        c.loSelection()
        c.action()
        return inputNumber

# Class representing a single process
class Process:
    # Constructor
    def __init__(self, mID, mStart, mRequired, mMem):
        # Process ID
        self.id = mID

        # Execution start time
        self.start = mStart

        # Time required for execution
        self.required = mRequired

        # Memory required for execution
        self.size = mMem

        # Block being occupied by the process
        self.block = None

    # Returns 'True' if the process has ended
    def finished(self):
        return self.required <= 0

# Pretty-print a time unit
def loTime(mTime):
    loC(Color.default)
    lo("T(")
    loC(Color.cyan)
    lo(str(mTime))
    loC(Color.default)
    lo("): ")

# Pretty-print a process ID
def loProcess(mProcess):
    loC(Color.default)
    lo("P[")
    loC(Color.red)
    lo(str(mProcess.id))
    loC(Color.default)
    lo("] ")

# Runs a simulation with the selected algorithm of a list of process
def simulate(mName, mProcesses, mAllocator, mAlgorithm):
    # Perform a deep copy of the process list
    toAdd = copy.deepcopy(mProcesses)

    # List of processes that are being executed
    toRem = []

    # Reset the state of the allocator
    mAllocator.reset()

    # Initialize statistic variables
    comparisons = 0
    fragmentation = 0

    # Current time unit
    t = 0

    # While there are processes that need to be started or must end...
    while toAdd or toRem:
        # The graph must be printed only in case of modifications
        mustPrint = False

        # For every process that needs to begin...
        for p in toAdd:
            # If the process should start...
            if p.start <= t:
                loTime(t)
                loProcess(p)
                lo("must start\n")

                # Prova ad inserirlo
                res = mAlgorithm(p.size)
                comparisons += res.cost

                loProcess(p)

                if res.success == False:
                    lo("- insertion failed\n")
                else:
                    lo("successfully inserted\n")
                    p.block = res.block
                    toAdd.remove(p)
                    toRem.append(p)

                    mustPrint = True

        # For every process in execution...
        for p in toRem:
            # Decrement required execution time
            p.required -= 1

            # If the process has finished execution...
            if p.finished() == True:
                loTime(t)
                loProcess(p)
                lo("finished its execution\n")

                # Deallocate block and reclaim memory
                p.block.occupied = False
                mAllocator.reclaim()
                toRem.remove(p)

                mustPrint = True

        # Increment current time
        t += 1

        # Accumulate allocator fragmentation
        fragmentation += mAllocator.getFragmentation()

        # Print allocator graph if needed
        if mustPrint == True:
            mAllocator.printInfo()
            lo("\n")

    # Calculate simulation statistics
    avgFragmentation = fragmentation / t
    score = ((fragmentation / t) * comparisons) / 100.0

    # Print simulation results
    rstr = Color.default + "Results for " + Color.cyan + mName + ":\n" + Color.default
    rstr += "\t" + Color.cyan + "Comparisons: " + Color.default + str(comparisons) + "\n"
    rstr += "\t" + Color.cyan + "Avg. fragmentation: " + Color.default + str(avgFragmentation) + "\n"
    rstr += "\t" + Color.cyan + "Score: " + Color.default + str(score) + "\n\n"

    return rstr

# Run 'mCount' simulations on an allocator and report statistics
def runSimulations(mAllocator, mCount):
    for s in range(0, mCount):
        loCD()
        lo("Simulation: ")
        loC(Color.cyan)
        lo(str(s))
        loCD()
        lo("\n")

        # Generate a random starting situation (processes) 
        lo("Generated processes:\n")

        loC(Color.dgray)
        lo("----------------------------\n")
        loCD()

        loC(Color.cyan)
        lo("P.ID\t")
        loCD()
        lo("T.In\t")
        loC(Color.cyan)
        lo("T.Out\t")
        loCD()
        lo("Mem\n")

        loC(Color.dgray)
        lo("----------------------------\n")
        loCD()

        processes = []

        # Generate a number of random process
        for i in range(0, rndI(8, 16)):
            # Generate random start time for process number i
            enter = rndI(0, 45)

            # Generate random required time for process number i
            exit = rndI(4, 40)

            # Generate random required memory for process number i
            size = rndI(10, 3500)

            # Create a process instance and append it to the list
            p = Process(i, enter, exit, size)
            processes.append(p)

            # Display information about the generated process
            loC(Color.cyan if i % 2 else Color.default)
            lo("{0}\t".format(i))
            loC(Color.default if i % 2 else Color.cyan)
            lo("{0}\t".format(enter))
            loC(Color.cyan if i % 2 else Color.default)
            lo("{0}\t".format(exit))
            loC(Color.default if i % 2 else Color.cyan)
            lo("{0}\n".format(size))

        raw_input()

        # Storage for simulation results
        simResults = []

        # Define a lambda shortcut function that takes the simulation title and an unary allocator 
        # function - it begins testing the passed algorithm
        sim = lambda n, l: simResults.append(simulate(n, processes, mAllocator, l))

        # Start a simulation for every allocator algorithm
        sim("first-fit",               lambda x: mAllocator.insertFirstFit(x))
        sim("next-fit",                lambda x: mAllocator.insertNextFit(x))
        sim("best-fit (naive)",        lambda x: mAllocator.insertBestFitNaive(x))
        sim("worst-fit (naive)",       lambda x: mAllocator.insertWorstFitNaive(x))
        sim("best-fit (sorted list)",  lambda x: mAllocator.insertBestFitSL(x))
        sim("worst-fit (sorted list)", lambda x: mAllocator.insertWorstFitSL(x))

        loC(Color.cyan)
        lo("\nResults:\n")
        loCD()
        lo("(lower score = better)\n\n")

        # Print out the results of the simulations
        for r in simResults:
            lo(r)
        
# Manual execution, display a menu with choices
def mainManual(mAllocator):
    inputSz = lambda: getInputMemorySize(size)

    m = Menu()
    m.add("First-fit",               lambda: mAllocator.insertFirstFit(inputSz()))
    m.add("Next-fit",                lambda: mAllocator.insertNextFit(inputSz()))
    m.add("Best-fit (naive)",        lambda: mAllocator.insertBestFitNaive(inputSz()))
    m.add("Worst-fit (naive)",       lambda: mAllocator.insertWorstFitNaive(inputSz()))
    m.add("Best-fit (sorted-list)",  lambda: mAllocator.insertBestFitSL(inputSz()))
    m.add("Worst-fit (sorted-list)", lambda: mAllocator.insertWorstFitSL(inputSz()))
    m.add("Reclaim memory",          lambda: mAllocator.reclaim())
    m.add("Free blocks",             lambda: mAllocator.free())
    m.add("Run simulations",         lambda: runSimulations(mAllocator, getInputSimulationCount()))
    m.add("Exit",                    lambda: sys.exit())

    while True:
        m.loAllChoices()
        choice = m.selectChoice()
        mAllocator.printInfo()

# Automatic execution, runs a number of simulations passed as a program argument
def mainAutomatic(mAllocator):
    runSimulations(mAllocator, int(sys.argv[1]))
    sys.exit()

# Initialize an 'Allocator' an start manual or automatic execution
def main():
    size = 10000
    allocator = Allocator(size)
    allocator.printInfo()

    if len(sys.argv) > 1:
        mainAutomatic(allocator)
    else:
        mainManual(allocator)

# If the program is ran as an executable, start 'main'
if __name__ == "__main__":
    loLn()
    main()
