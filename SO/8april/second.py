#!/usr/bin/python2

import random
import copy
import os
import sys

# Generated a random number between 'mMin' and 'mMax'
def rndI(mMin, mMax):
    return random.randint(mMin, mMax - 1)

# Get the current co1nsole window width and height
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

        loSep()

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
        for i in range(0, len(self.blocks)):
            b = self.blocks[i]

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
            self.blocks.remove(b2)

            # Call this function recursively if reclaiming was successful
            return self.reclaim()

    # Free the memory in all the blocks, making them unoccupied
    def free(self):
        for b in self.blocks:
            b.occupied = False

    def printInfo(self):                
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
                    bStr += Color.red + "#" + Color.default
                else:
                    bStr += Color.dgray + "_" + Color.default
                
                pStr += " "
                iStr += " "

            if b.occupied:
                bStr += Color.red + "#" + Color.default
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

    def getFragmentation(self):
        result = len(self.blocks)

        for b in self.blocks:
            if b.occupied == False:
                result += b.getSize()

        return result

    def executeAlgorithm(self, mAlgorithm, mX):
        loSep()
        result = AlgorithmResult()

        mAlgorithm(result, mX)

        result.log()
        return result

    def successOn(self, result, b, mX):
        l, r, idx = self.splitAt(b.start + mX)          
        l.occupied = True
        result.success = True
        result.block = b
        self.lastIdx = idx

    def implFirstFit(self, result, mX):
        for b in self.blocks:
            result.cost += 1

            if b.occupied == False and b.getSize() >= mX:
                self.successOn(result, b, mX)
                return

    def implBestFitNaive(self, result, mX):
        best = None
        for i in range(0, len(self.blocks)):
            result.cost += 1
            b = self.blocks[i]

            if (best == None or b.getSize() < best.getSize()) and b.getSize() >= mX and b.occupied == False:
                best = b

        if best != None:
            self.successOn(result, best, mX)

    def implWorstFitNaive(self, result, mX):
        best = None
        for i in range(0, len(self.blocks)):
            result.cost += 1
            b = self.blocks[i]

            if (best == None or b.getSize() < best.getSize()) and b.getSize() >= mX and b.occupied == False:
                best = b

        if best != None:
            self.successOn(result, best, mX)

    def implNextFit(self, result, mX):
        for i in range(0, len(self.blocks)):
            result.cost += 1

            b = self.blocks[(i + self.lastIdx + 1) % len(self.blocks)]

            if b.occupied or b.getSize() < mX:
                continue

            self.successOn(result, b, mX)
            break

    def implBestFitSL(self, result, mX):
        for b in self.sortedBlocks:
            result.cost += 1

            if b.getSize() >= mX and b.occupied == False:
                self.successOn(result, b, mX)
                break

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

    # TODO
    def insertBestFitSL(self, mX):
        return self.executeAlgorithm(self.implBestFitSL, mX)

    # TODO
    def insertWorstFitSL(self, mX):
        return self.executeAlgorithm(self.implWorstFitSL, mX)

# Gets a valid memory size integer from the user
def getInputMemorySize(mMax):
    lo("\nInsert requested " + Color.cyan + "memory size: " + Color.default)

    while True:
        result = getInputInt()

        if result <= 0 or result >= mMax:
            lo("Invalid memory size value")
            continue

        return result

# TODO
class Choice:
    # TODO
    def __init__(self, mTitle):        
        self.number = -1
        self.title = mTitle

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

# TODO
class Menu:
    # TODO
    def __init__(self):
        self.choices = []
        self.nextNumber = 0

    # TODO
    def add(self, mChoice):
        self.choices.append(mChoice)
        mChoice.number = self.nextNumber
        self.nextNumber += 1

    # TODO
    def loAllChoices(self):
        lo("\nChoose:\n")
        for c in self.choices:
            c.loChoice()

    # TODO
    def selectChoice(self):
        inputNumber = getInputInt()
        c = self.choices[inputNumber]
        c.loSelection()
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

# Simula un algoritmo
def simulate(mName, mProcesses, mAllocator, mAlgorithm):
    # Copia lista dei processi da far partire
    toAdd = copy.deepcopy(mProcesses)

    # Lista dei processi in corso
    toRem = []

    # Allocatore
    mAllocator.reset()

    # Dati statistici
    comparisons = 0
    fragmentation = 0

    # Tempo corrente
    t = 0

    # Finche' ci sono processi da aggiungere o in corso...
    while len(toAdd) > 0 or len(toRem) > 0:
        # Per ogni processo da aggiungere...
        for p in toAdd:
            # Se il processo entra al tempo t...
            if p.start <= t:
                lo("t{0}: P{1} deve entrare\n".format(t, p.id))

                # Prova ad inserirlo
                res = mAlgorithm(p.size)
                comparisons += res.cost

                if res.success == False:
                    lo("Inserimento di P{0} fallito\n".format(p.id))
                else:
                    lo("Inserimento di P{0} riuscito\n".format(p.id))
                    p.block = res.block
                    toAdd.remove(p)
                    toRem.append(p)

        # Per ogni processo in esecuzione...
        for p in toRem:
            # Decrementa tempo necessario
            p.required -= 1

            # Se il processo e' terminato...
            if p.finished() == True:
                lo("t{0}: P{1} terminato\n".format(t, p.id))

                # Disalloca il blocco e prova a riunificare la memoria
                lo("Rimozione di P{0}\n".format(p.id))
                p.block.occupied = False
                mAllocator.reclaim()
                toRem.remove(p)

        # Incrementa il tempo
        t += 1

        # Accumula frammentazione
        fragmentation += mAllocator.getFragmentation()

        # Stampa stato
        mAllocator.printInfo()
        lo("\n")

    # Restituisci risultato
    rstr = "RISULTATO {0}:".format(mName) + "\n"
    rstr += "Comparazioni: {0}".format(comparisons) + "\n"
    rstr += "Frammentazione media: {0}".format(fragmentation / t) + "\n"
    rstr += "Punteggio: {0}".format(((fragmentation / t) * comparisons) / 1000000.0) + "\n"

    return rstr

# TODO
def runSimulations(mAllocator):
    # Inserisci il numero di simulazioni da tastiera
    loCD()
    lo("Insert number of ")
    loC(Color.cyan)
    lo("simulations: ")
    loCD()
    numSim = int(raw_input())
    lo("\n")

    for s in range(0, numSim):
        lo("SIMULAZIONE " + str(s) + "\n")

        # Genera una situazione iniziale di processi
        lo("Generazione processi\n")
        
        loC(Color.cyan)
        lo("PROC\t")
        loCD()
        lo("IN\t")
        loC(Color.cyan)
        lo("OUT\t")
        loCD()
        lo("SIZE\n")        

        processes = []

        # Genera N processi casuali
        for i in range(0, rndI(10, 50)):
            # Tempo di entrata del processo i-esimo
            enter = rndI(0, 45)

            # Tempo necessario del processo i-esimo
            exit = rndI(4, 40)

            # Memoria richiesta dal processo i-esimo
            size = rndI(10, 1500)

            # Instanzia il processo ed aggiungilo alla lista
            p = Process(i, enter, exit, size)
            processes.append(p)

            # Stampa informazioni sul processo generato
            loC(Color.cyan)
            lo("{0}\t".format(i))
            loCD()
            lo("{0}\t".format(enter))
            loC(Color.cyan)
            lo("{0}\t".format(exit))
            loCD()
            lo("{0}\n".format(size))

        raw_input()

        # Crea una funzione lambda chiamata sim che prende come input una stringa ed una funzione binaria
        # Quando sim viene chiamata, genera un test di simulazione con il nome e l'algoritmo passato
        sim = lambda n, l: simulate(n, processes, mAllocator, l)

        # Instanzia 5 simulazioni utilizzando i 5 algoritmi implementati
        r0 = sim("first-fit", lambda x: mAllocator.insertFirstFit(x))
        r1 = sim("next-fit", lambda x: mAllocator.insertNextFit(x))
        r2 = sim("best-fit naive", lambda x: mAllocator.insertBestFitNaive(x))
        r3 = sim("worst-fit naive", lambda x: mAllocator.insertWorstFitNaive(x))
        r4 = sim("best-fit ordinato", lambda x: mAllocator.insertBestFitSL(x))
        r5 = sim("worst-fit ordinato", lambda x: mAllocator.insertWorstFitSL(x))

        # Stampa i risultati delle 5 simulazioni
        lo(r0)
        lo(r1)
        lo(r2)
        lo(r3)
        lo(r4)
        lo(r5)

def main():
    size = 10000
    allocator = Allocator(size)
    allocator.printInfo()

    menu = Menu()
    menu.add(Choice("First-fit"))
    menu.add(Choice("Next-fit"))
    menu.add(Choice("Best-fit (naive)"))
    menu.add(Choice("Worst-fit (naive)"))
    menu.add(Choice("Best-fit (sorted-list)"))
    menu.add(Choice("Worst-fit (sorted-list)"))
    menu.add(Choice("Reclaim memory"))
    menu.add(Choice("Free blocks"))
    menu.add(Choice("Run simulations"))
    menu.add(Choice("Exit"))

    while True:
        menu.loAllChoices()    
        choice = menu.selectChoice()        
        
        if choice == 0:
            rMem = getInputMemorySize(size)
            allocator.insertFirstFit(rMem)

        elif choice == 1:
            rMem = getInputMemorySize(size)
            allocator.insertNextFit(rMem)

        elif choice == 2:
            rMem = getInputMemorySize(size)
            allocator.insertBestFitNaive(rMem)

        elif choice == 3:
            rMem = getInputMemorySize(size)
            allocator.insertWorstFitNaive(rMem)

        elif choice == 4:
            rMem = getInputMemorySize(size)
            allocator.insertBestFitSL(rMem)

        elif choice == 5:
            rMem = getInputMemorySize(size)
            allocator.insertWorstFitSL(rMem)

        elif choice == 6:
            allocator.reclaim()

        elif choice == 7:
            allocator.free()

        elif choice == 8:  
            runSimulations(allocator)
            break

        elif choice == 9:
            break

        allocator.printInfo()

if __name__ == "__main__":
    loLn()
    main()
