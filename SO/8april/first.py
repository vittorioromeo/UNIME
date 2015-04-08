import random

def rnd(mMin, mMax):
	return random.randint(mMin, mMax - 1)

class Block:
	def __init__(self, mId, mAllocator, mStart, mEnd):
		self.id = mId
		self.allocator = mAllocator
		self.start = mStart
		self.end = mEnd
		self.occupied = False

	def getSize(self):
		return self.end - self.start

	def printInfo(self):
		print "Block %d\t\t[%d, %d)\tsize: %d" % (self.id, self.start, self.end, self.getSize())

class Allocator:
	def __init__(self, mSize):
		self.size = mSize
		self.blocks = []
		self.lastSet = False

	def makeBlocks(self, mCount):
		sliceSize = self.size / mCount		
		last = 0
		lastId = 0

		# Add all random blocks but the last
		for i in range(0, mCount - 1):
			bSize = sliceSize + rnd(-sliceSize / (mCount / 3), +sliceSize / (mCount / 3))
			b = Block(lastId, self, last, last + bSize)
			lastId += 1
			last = last + bSize
			self.blocks.append(b)

		# Add last block
		b = Block(lastId, self, last, self.size)
		self.blocks.append(b)

	def printBlocks(self):
		for b in self.blocks:
			b.printInfo()

	def insertFirstFit(self, mRMem):
		for b in self.blocks:
			if b.getSize() < mRMem:
				continue

			print "\tFound suitable block for %d" % mRMem
			b.printInfo()
			self.lastBlock = b
			self.lastSet = True
			return True

		print "\tCould not find a suitable block for %d" % mRMem
		return False

	def insertBestFit(self, mRMem):
		found = False

		for b in self.blocks:
			if b.getSize() < mRMem:
				continue

			print "\tFound suitable block for %d" % mRMem
			b.printInfo()
			
			if found == False:
				print "\tFirst suitable block"
				bestBlock = b
			elif bestBlock.getSize() > b:
				print "\tBetter than last found block"
				bestBlock = b
				self.lastBlock = b
				self.lastSet = True
			else:
				print "\tWorse than last found block"

			found = True
		
		if found == False:
			print "Could not find a suitable block for %d" % mRMem
			return False

		return True

	def insertNextFit(self, mRMem):
		if self.lastSet == False:
			self.lastBlock = self.blocks[0]
			self.lastSet = True

		lastId = self.lastBlock.id

		for i in range(0, len(self.blocks)):
			b = self.blocks[(lastId + i + 1) % len(self.blocks)]

			if b.getSize() < mRMem:
				continue

			print "\tFound suitable block for %d" % mRMem
			b.printInfo()
			self.lastBlock = b
			return True

		print "\tCould not find a suitable block for %d" % mRMem
		return False

def main():
	size = 10000
	allocator = Allocator(size)
	print "Allocator initialized with size %d" % size

	blockCount = 10
	print "Allocator divided in %d blocks" % blockCount

	allocator.makeBlocks(blockCount)
	allocator.printBlocks()

	while True:
		print "\nChoose:"
		print "\t0. First-fit"
		print "\t1. Best-fit"
		print "\t2. Next-fit"
		print "\t3. Exit"

		choice = int(raw_input())

		if choice == 0:
			print "First-fit selected"

			print "\tInsert requested memory size"
			rMem = int(raw_input())

			allocator.insertFirstFit(rMem)

		elif choice == 1:
			print "Best-fit selected"

			print "\tInsert requested memory size"
			rMem = int(raw_input())

			allocator.insertBestFit(rMem)

		elif choice == 2:
			print "Next-fit selected"

			print "\tInsert requested memory size"
			rMem = int(raw_input())

			allocator.insertNextFit(rMem)

		elif choice == 3:
			break



if __name__ == "__main__":
    main()