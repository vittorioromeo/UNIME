import os

# Get the current console window width and height
consoleHeight, consoleWidth = os.popen('stty size', 'r').read().split()
consoleHeight = int(consoleHeight)
consoleWidth = int(consoleWidth)

# Set reasonable values for the printed memory graph
graphHeight = 5
graphWidth = consoleWidth

# Open a log file stream
logFile = open("log.txt", "w")

# Print both to log and console
def lo(x):
	logFile.write(x + "\n")
	print(x)

# Print a separator line
def loLn():
	lo("_" * consoleWidth)

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
			lo("\nvInserted value invalid - retry")
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
	def __init__(self, mSuccess = False, mCost = 0, mBIdx = None):
		# 'True' if the algorithm succedeed
		self.success = mSuccess

		# Number of steps taken by the algorithm
		self.cost = mCost

		# Index of the block occupied in case of success
		self.bIdx = mBIdx

	# Prints the data to console and log
	def log(self):
		lo("Success: " + str(self.success))
		lo("Cost: " + str(self.cost))
		loSep()

# Class representing a memory allocator 
class Allocator:
	# Constructor
	def __init__(self, mSize):
		# List of the blocks owned by the allocator
		self.blocks = []

		# Total size of the allocator
		self.size = mSize

		# Initialize the allocator a single memory block including 'mSize'
		self.blocks.append(Block(self, 0, mSize))

		# Last occupied block index
		self.lastIdx = 0

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

		# Resize the first block to [l, m)
		toSplit.start = l
		toSplit.end = m

		# Create an additional block including [m, r)
		newBlock = Block(self, m, r)
		self.blocks.insert(toSplitIdx + 1, newBlock)

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
			bStr += "|"
			pStr += "|"
			
			if lastIdx == 0:
				iStr += "0"
			else:
				iStr += "|"

			for i in range(0, b.getSize() / graphStep):
				if b.occupied:
					bStr += "#"
				else:
					bStr += "_"
				
				pStr += " "
				iStr += " "

			if b.occupied:
				bStr += "#"
			else:
				bStr += "_"

			pStr += " "
			iStr = iStr[:-getDigitCount(b.end) + 1]
			iStr += str(b.end)

			lastIdx += 1

		bStr += "|"
		pStr += "|"
		iStr += "|"

		lo("")
		lo("")
		lo("")
		lo(pStr)
		lo(pStr)
		lo(bStr)
		lo(iStr)
		lo(pStr)
		lo(pStr)

	# The "first fit" algorithm simply iterates over all blocks
	# until it finds a block which is suitable for the desired
	# memory allocation
	def insertFirstFit(self, mX):
		loSep()
		result = AlgorithmResult()

		for b in self.blocks:
			result.cost += 1

			if b.occupied or b.getSize() < mX:
				continue

			l, r, idx = self.splitAt(b.start + mX)			
			l.occupied = True
			result.success = True
			self.lastIdx = idx
			break

		result.log()
		return result

	# The "best fit" algorithm iterates over all available blocks 
	# and selects the one that wastes less space for the memory allocation
	# This algorithm could be improved by keeping a separate list of memory blocks,
	# ordered by size
	def insertBestFit(self, mX):
		loSep()
		result = AlgorithmResult()

		best = None
		for i in range(0, len(self.blocks)):
			result.cost += 1
			b = self.blocks[i]

			if (best == None or b.getSize() < best.getSize()) and b.getSize() >= mX and b.occupied == False:
				best = b

		if best != None:
			result.success = True
			l, r, idx = self.splitAt(best.start + mX)			
			l.occupied = True
			self.lastIdx = idx
		
		result.log()
		return result

	# The "next fit" algorithm uses the same logic as the "first fit" algorithm,
	# but starts looking for an unoccupied block at the last block index
	def insertNextFit(self, mX):
		loSep()
		result = AlgorithmResult()

		for i in range(0, len(self.blocks)):
			result.cost += 1

			b = self.blocks[(i + self.lastIdx + 1) % len(self.blocks)]

			if b.occupied or b.getSize() < mX:
				continue

			l, r, idx = self.splitAt(b.start + mX)			
			l.occupied = True
			result.success = True
			self.lastIdx = idx
			break

		result.log()
		return result

# Gets a valid memory size integer from the user
def getInputMemorySize(mMax):
	lo("\tInsert requested memory size")

	while True:
		result = getInputInt()

		if result <= 0 or result >= mMax:
			lo("Invalid memory size value")
			continue

		return result

def main():
	size = 10000
	allocator = Allocator(size)
	allocator.printInfo()

	while True:
		lo("\nChoose:")
		lo("\t0. First-fit")
		lo("\t1. Best-fit")
		lo("\t2. Next-fit")
		lo("\t3. Reclaim memory")
		lo("\t4. Free blocks")
		lo("\t5. Exit")

		choice = getInputInt()

		if choice == 0:
			lo("First-fit selected")
			rMem = getInputMemorySize(size)
			allocator.insertFirstFit(rMem)

		elif choice == 1:
			lo("Best-fit selected")
			rMem = getInputMemorySize(size)
			allocator.insertBestFit(rMem)

		elif choice == 2:
			lo("Next-fit selected")
			rMem = getInputMemorySize(size)
			allocator.insertNextFit(rMem)

		elif choice == 3:
			lo("Reclaim selected")
			allocator.reclaim()

		elif choice == 4:
			lo("Free selected")
			allocator.free()

		elif choice == 5:
			break

		allocator.printInfo()

if __name__ == "__main__":
	loLn()
	main()
