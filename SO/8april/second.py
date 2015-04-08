def getDigitCount(mX):
	return len(str(mX))

class Block:
	def __init__(self, mAllocator, mStart, mEnd):
		self.allocator = mAllocator
		self.start = mStart
		self.end = mEnd
		self.occupied = False

	def getSize(self):
		return self.end - self.start

class Allocator:
	def __init__(self, mSize):
		self.blocks = []
		self.size = mSize

		# First block 
		firstBlock = Block(self, 0, mSize)
		self.blocks.append(firstBlock)
		#firstBlock.occupied = True

		#self.blocks.append(Block(self, mSize - 2500, mSize))

	def splitAt(self, mX):
		for i in range(0, len(self.blocks)):
			b = self.blocks[i]

			if b.end < mX:
				continue

			toSplit = b
			if b.getSize() == mX:
				b.occupied = True
				return

			if toSplit.occupied:
				print "Cannot split occupied block"
				return

			self.blocks.remove(b)

			l = toSplit.start
			m = mX
			r = toSplit.end

			nb1 = Block(self, l, m)
			nb2 = Block(self, m, r)

			nb1.occupied = True
			nb2.occupied = False

			self.blocks.insert(i, nb1)
			self.blocks.insert(i + 1, nb2)

			return

	def reclaim(self):
		if len(self.blocks) < 2: 
			return

		for i in range(0, len(self.blocks) - 1):
			
			b1 = self.blocks[i]
			b2 = self.blocks[i + 1]

			if b1.occupied or b2.occupied:
				continue

			self.blocks.remove(b1)
			self.blocks.remove(b2)

			self.blocks.insert(i, Block(self, b1.start, b2.end))
			return self.reclaim()

	def free(self):
		for b in self.blocks:
			b.occupied = False

	def printInfo(self):
		stepSize = 100
		
		aStr = "0"
		for i in range(0, self.size / stepSize):
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

			# iStr = iStr[:-getDigitCount(b.start) + 1]
			# iStr += str(b.start)

			for i in range(0, b.getSize() / stepSize):
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
			#pStr += str(lastIdx)

			lastIdx += 1

		bStr += "|"
		pStr += "|"
		iStr += "|"

		print ""
		print ""
		print ""
		#print aStr			
		print pStr
		print pStr
		print bStr
		print iStr
		print pStr
		print pStr

	def insertFirstFit(self, mX):
		print ""

		for b in self.blocks:
			if b.occupied or b.getSize() < mX:
				continue

			self.splitAt(b.start + mX)			
			return

	def insertBestFit(self, mX):
		print ""

	def insertNextFit(self, mX):
		print ""

def main():
	size = 10000
	allocator = Allocator(10000)
	allocator.printInfo()

	while True:
		print "\nChoose:"
		print "\t0. First-fit"
		print "\t1. Best-fit"
		print "\t2. Next-fit"
		print "\t3. Reclaim memory"
		print "\t4. Free blocks"
		print "\t5. Exit"

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
			print "Reclaim selected"
			allocator.reclaim()

		elif choice == 4:
			print "Reclaim selected"
			allocator.free()

		elif choice == 5:
			break

		allocator.printInfo()


if __name__ == "__main__":
    main()
