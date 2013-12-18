// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <VeeLib/VeeLib.h>

#define VL_CHOICE_COUNT 14
#define MAX_SIZE 100

void runTests();

void choiceNewVersion()
{
	printf("Insert an integer: ");
	int number = vlc_getScanfI(), reverse = vlm_getReversedI(number);
	printf("\nThe reversed integer is: %d\nThe number is %spalindrome!", reverse, number == reverse ? "" : "not ");
}

void choiceNaiveVersion()
{
	printf("Insert an integer: ");
	int number = vlc_getScanfI(), reverse = vldpr_getReversedNaive(number);
	printf("\nThe reversed integer is: %d\nThe number is %spalindrome!", reverse, number == reverse ? "" : "not ");
}

void choiceNaivePrintfVersion()
{
	printf("Insert an integer: \nThe reversed integer is: ");

	int number = vlc_getScanfI();
	while(number != 0)
	{
		printf("%d", number % 10);
		number /= 10;
	}
}

void choiceSequenceMinMax()
{
	printf("Enter any number of integers, separated by pressing return.\nWhen you're done, enter '-1' to signal the end of the sequence.\n");

	bool increasing = true;

	int number, min, max, prevNumber, evenCount = 0;
	number = min = max = prevNumber = vlc_getScanfI();

	if(min == -1 && max == -1) { printf("The entered sequence is empty."); return; }

	while(number != -1)
	{
		if(number < prevNumber) increasing = false;
		if(number % 2 == 0) ++evenCount;

		min = vlm_getMinI(min, number);
		max = vlm_getMaxI(max, number);

		prevNumber = number;
		number = vlc_getScanfI();
	}

	printf("Min: %d\tMax: %d\tEven count: %d\nThe sequence is in %sincreasing order\n", min, max, evenCount, increasing ? "" : "not in ");
}

void choiceCountMultipleOf()
{
	printf("Enter lower bound (inclusive)\n");
	int lb = vlc_getScanfI();

	printf("Enter upper bound (exclusive)\n");
	int ub = vlc_getScanfI();

	printf("Enter divisor\n");
	int divisor = vlc_getScanfI();

	vlc_clearScreen();

	int i, firstMultiple;
	for(i = lb; i < ub; ++i)
		if(i % divisor == 0)
		{
			firstMultiple = i;
			for(i = firstMultiple; i < ub; i += divisor) printf("%d\n", i);
			return;
		}
}

void choicePowOf2()
{
	printf("Enter power base\n");
	int base = vlc_getScanfI();

	printf("Enter how many powers to calculate\n");
	int count = vlc_getScanfI();

	vlc_clearScreen();

	int i, n = 1;
	for(i = 0; i < count; ++i) printf("%d\n", n *= base);
}

void choiceFactorial()
{
	printf("Enter number\n");
	int number = vlc_getScanfI();

	vlc_clearScreen();

	printf("%d\n", vlm_getFactorial(number));
}

void choiceFibonacci() { printf("%d", vlm_getFibonacci(vlc_getScanfI())); }

void choiceFindLog()
{
	printf("Insert base\n");
	int base = vlc_getScanfI();

	printf("Insert argument\n");
	int arg = vlc_getScanfI();

	int result = -1;

	do
	{
		++result;
		arg /= base;
	} while(arg != 0);

	 printf("Log: %d", result);
}

float getSignF(float mA) { return mA > 0.f ? 1.f : -1.f; }

void choiceFunctionExercise()
{
	float tolerance = 0.0001f;

	printf("Enter lower bound (inclusive)\n");
	float lb = vlc_getScanfF();

	printf("Enter upper bound (exclusive)\n");
	float ub = vlc_getScanfF();

	float mb, sinLb, sinUb, sinMb;

	do
	{
		mb = (lb + ub) / 2.f;
		sinLb = sin(lb);
		sinUb = sin(ub);
		sinMb = sin(mb);

		if(sinMb > 0)
		{
			if(sinLb < 0 && sinUb > 0) ub = mb;
			else if(sinLb > 0 && sinUb < 0) lb = mb;
		}
		else
		{
			if(sinLb > 0 && sinUb < 0) ub = mb;
			else if(sinLb < 0 && sinUb > 0) lb = mb;
		}

		printf("sin(%f) -> %f\n", mb, sinMb);
	} while(fabs(sinMb) > tolerance);
}

void prettyPrintArray(int* mArray, int mSize)
{
	int i;
	for(i = 0; i < mSize; ++i) printf("%d%s", mArray[i], i < mSize - 1 ? ", " : "");
}

bool isInArray(int* mArray, int mSize, int mNumber)
{
	int i;
	for(i = 0; i < mSize; ++i) if(mArray[i] == mNumber) return true;
	return false;
}

void getArrayUnique(int* mArray, int mSize, int* mTarget, int* mNewSize)
{
	// Possible alternative solution: sort the array first, then skip adjacent
	// identical values.

	int targetSize = 0, i;

	for(i = 0; i < mSize; ++i)
		if(!isInArray(mTarget, targetSize, mArray[i]))
			mTarget[targetSize++] = mArray[i];

	*mNewSize = targetSize;
}


void getArrayIntersection(int* mUniqueA, int mSizeA, int* mUniqueB, int mSizeB, int* mTarget, int* mNewSize)
{
	int targetSize = 0, i;

	for(i = 0; i < mSizeA; ++i)
		if(isInArray(mUniqueB, mSizeB, mUniqueA[i]))
			mTarget[targetSize++] = mUniqueA[i];

	*mNewSize = targetSize;
}

void getArrayUnion(int* mUniqueA, int mSizeA, int* mUniqueB, int mSizeB, int* mTarget, int* mNewSize)
{
	int tempArray[MAX_SIZE], tempSize = 0, i;

	for(i = 0; i < mSizeA; ++i) tempArray[tempSize++] = mUniqueA[i];
	for(i = 0; i < mSizeB; ++i) tempArray[tempSize++] = mUniqueB[i];

	getArrayUnique(tempArray, tempSize, mTarget, mNewSize);
}

void calcIntersection(int* mA, int mSizeA, int* mB, int mSizeB, int* mTarget, int* mNewSize)
{
	int uniqueA[MAX_SIZE], uniqueB[MAX_SIZE], sizeUniqueA, sizeUniqueB;
	getArrayUnique(mA, mSizeA, uniqueA, &sizeUniqueA);
	getArrayUnique(mB, mSizeB, uniqueB, &sizeUniqueB);

	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("A:\t\t\t");
	vlc_setFmt(vlc_StyleBold, vlc_ColorCyan);
	prettyPrintArray(mA, mSizeA);
	printf("\n"); vlc_resetFmt();

	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("B:\t\t\t");
	vlc_setFmt(vlc_StyleBold, vlc_ColorCyan);
	prettyPrintArray(mB, mSizeB);
	printf("\n"); vlc_resetFmt();

	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("Unique A:\t\t");
	vlc_setFmt(vlc_StyleBold, vlc_ColorYellow);
	prettyPrintArray(uniqueA, sizeUniqueA);
	printf("\n"); vlc_resetFmt();

	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("Unique B:\t\t");
	vlc_setFmt(vlc_StyleBold, vlc_ColorYellow);
	prettyPrintArray(uniqueB, sizeUniqueB);
	printf("\n"); vlc_resetFmt();

	int sizeIntersection;
	getArrayIntersection(uniqueA, sizeUniqueA, uniqueB, sizeUniqueB, mTarget, &sizeIntersection);

	*mNewSize = sizeIntersection;
}

void calcUnion(int* mA, int mSizeA, int* mB, int mSizeB, int* mTarget, int* mNewSize)
{
	int uniqueA[MAX_SIZE], uniqueB[MAX_SIZE], sizeUniqueA, sizeUniqueB;
	getArrayUnique(mA, mSizeA, uniqueA, &sizeUniqueA);
	getArrayUnique(mB, mSizeB, uniqueB, &sizeUniqueB);

	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("A:\t\t\t");
	vlc_setFmt(vlc_StyleBold, vlc_ColorCyan);
	prettyPrintArray(mA, mSizeA);
	printf("\n"); vlc_resetFmt();

	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("B:\t\t\t");
	vlc_setFmt(vlc_StyleBold, vlc_ColorCyan);
	prettyPrintArray(mB, mSizeB);
	printf("\n"); vlc_resetFmt();

	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("Unique A:\t\t");
	vlc_setFmt(vlc_StyleBold, vlc_ColorYellow);
	prettyPrintArray(uniqueA, sizeUniqueA);
	printf("\n"); vlc_resetFmt();

	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("Unique B:\t\t");
	vlc_setFmt(vlc_StyleBold, vlc_ColorYellow);
	prettyPrintArray(uniqueB, sizeUniqueB);
	printf("\n"); vlc_resetFmt();

	int sizeUnion;
	getArrayUnion(uniqueA, sizeUniqueA, uniqueB, sizeUniqueB, mTarget, &sizeUnion);

	*mNewSize = sizeUnion;
}

void choiceVExercise1()
{
	#define SIZE_A 11
	#define SIZE_B 7
	#define SIZE_TARGET 10

	int a[SIZE_A] = { 1, 2, 3, 4, 5, 6, 1, 7, 8, 9, 10 };
	int b[SIZE_B] = { 1, 5, 2, 6, 9, 1000, 9 };
	int target[SIZE_TARGET];

	int intersectionSize;
	calcIntersection(a, SIZE_A, b, SIZE_B, target, &intersectionSize);

	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("Intersection:\t\t");
	vlc_setFmt(vlc_StyleBold, vlc_ColorBlue);
	prettyPrintArray(target, intersectionSize);
	printf("\n"); vlc_resetFmt();

	#undef SIZE_A
	#undef SIZE_B
	#undef SIZE_TARGET
}

void choiceVExercise2()
{
	#define SIZE_A 11
	#define SIZE_B 7
	#define SIZE_TARGET 10

	int a[SIZE_A] = { 1, 2, 3, 4, 5, 6, 1, 7, 8, 9, 10 };
	int b[SIZE_B] = { 1, 5, 2, 6, 9, 1000, 9 };
	int target[SIZE_TARGET];

	int unionSize;
	calcUnion(a, SIZE_A, b, SIZE_B, target, &unionSize);

	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("Union:\t\t\t");
	vlc_setFmt(vlc_StyleBold, vlc_ColorBlue);
	prettyPrintArray(target, unionSize);
	printf("\n"); vlc_resetFmt();

	#undef SIZE_A
	#undef SIZE_B
	#undef SIZE_TARGET
}

void choiceVExercise3()
{
	// This can be improved by sorting the array first, then using a binary search
	// to check if the value minus the present value is available in the array

	#define SIZE_A 11

	printf("\nInsert desired sum: ");
	int desiredSum = vlc_getScanfI();
	printf("\n");

	int a[SIZE_A] = { 1, 2, 3, 4, 5, 6, 1, 7, 8, 9, 10 };

	int uniqueA[MAX_SIZE], sizeUniqueA;
	getArrayUnique(a, SIZE_A, uniqueA, &sizeUniqueA);

	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("A:\t\t\t");
	vlc_setFmt(vlc_StyleBold, vlc_ColorCyan);
	prettyPrintArray(a, SIZE_A);
	printf("\n"); vlc_resetFmt();

	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("Unique A:\t\t");
	vlc_setFmt(vlc_StyleBold, vlc_ColorYellow);
	prettyPrintArray(uniqueA, sizeUniqueA);
	printf("\n"); vlc_resetFmt();

	bool found = false;
	int iA, iB, n1, n2;
	for(iA = 0; iA < sizeUniqueA; ++iA)
		for(iB = 0; iB < sizeUniqueA; ++iB)
			if(iA != iB && (uniqueA[iA] + uniqueA[iB] == desiredSum))
			{
				n1 = uniqueA[iA];
				n2 = uniqueA[iB];
				found = true;
				goto end;
			}

	end:

	printf("%s", found ? "Number found!" : "Number not found :(");
	if(found) printf(" (%d + %d)", n1, n2);

	#undef SIZE_A
}

bool isPrime(int mValue)
{
	int i;
	for(i = 2; i < mValue; ++i) if(mValue % i == 0) return false;
	return true;
}

void fillWithPrimes(int* mArray, int mSize)
{
	int i = 2, currentIdx = 0;
	while(currentIdx < mSize)
	{
		if(isPrime(i)) mArray[currentIdx++] = i;
		++i;
	}
}

void factorize(int* mPrimesArray, int mPrimesArraySize, int* mOutputArray, int mValue)
{
	int i;
	for(i = 0; i < mPrimesArraySize; ++i) mOutputArray[i] = 0;

	int idxPrime = 0, idxOutput = 0;

	while(idxPrime < mPrimesArraySize && mValue != 1)
	{
		if(mValue % mPrimesArray[idxPrime] == 0)
		{
			mValue /= mPrimesArray[idxPrime];
			++mOutputArray[idxPrime];
			++idxOutput;
		}
		else ++idxPrime;
	}
}

#define PRIME_COUNT 1000

int eulerFunction(int* mPrimesArray, int mPrimesArraySize, int mValue)
{
	// Returns the count of numbers coprime of mValue.

	int outputArray[PRIME_COUNT];

	factorize(mPrimesArray, mPrimesArraySize, outputArray, mValue);

	printf("\nFactorized %d: ", mValue);
	int i; for(i = 0; i < PRIME_COUNT; ++i) if(outputArray[i] > 0) printf("%d^%d, ", mPrimesArray[i], outputArray[i]);

	int result = 1;
	int idxFactor;
	for(idxFactor = 0; idxFactor < PRIME_COUNT; ++idxFactor)
	{
		if(outputArray[idxFactor] == 0) continue;
		result *= pow(mPrimesArray[idxFactor], outputArray[idxFactor]) - pow(mPrimesArray[idxFactor], outputArray[idxFactor] - 1);
	}

	printf("\nRESULT: %d", result);
	printf("\n\n");
	return -1;
}


void choiceEuler()
{

	#define OUTPUT_ARRAY_SIZE 100

	printf("Generating %d primes...\n\n", PRIME_COUNT);

	int primeArray[PRIME_COUNT];
	fillWithPrimes(primeArray, PRIME_COUNT);
	prettyPrintArray(primeArray, PRIME_COUNT);

	while(true)
	{
		int choice = vlc_getScanfI();
		vlc_clearScreen();

		if(choice == -1) return;

		printf("%d", eulerFunction(primeArray, PRIME_COUNT, choice));
	}
}

int main()
{
	runTests();

	const char* choiceDescs[VL_CHOICE_COUNT] =
	{
		"Print a number's reverse and whether it's palindrome.",
		"Print an integer sequence's min and max numbers.",
		"Print nth fibonacci number",
		"Print multiples of n between m1 and m2",
		"Print n powers of m",
		"Print factorial of n",
		"Print a number's reverse and whether it's palindrome. (deprecated naive version)",
		"Print a number's reverse. (deprecated naive printf version)",
		"Find log by using divisions",
		"Find function using bisection",
		"Find intersection of two vectors",
		"Find union of two vectors",
		"Given a vector, does the sum of two number exist?",
		"Euler function",
	};

	void(*fnPtrs[VL_CHOICE_COUNT])() =
	{
		&choiceNewVersion,
		&choiceSequenceMinMax,
		&choiceFibonacci,
		&choiceCountMultipleOf,
		&choicePowOf2,
		&choiceFactorial,
		&choiceNaiveVersion,
		&choiceNaivePrintfVersion,
		&choiceFindLog,
		&choiceFunctionExercise,
		&choiceVExercise1,
		&choiceVExercise2,
		&choiceVExercise3,
		&choiceEuler
	};

	vlc_showMenu(VL_CHOICE_COUNT, choiceDescs, fnPtrs);
	return 0;
}

void runTests()
{
	VL_EXPECT(vlm_getNumberOfDigitsI(1000) == 4);
	VL_EXPECT(vlm_getNumberOfDigitsI(111) == 3);
	VL_EXPECT(vlm_getNumberOfDigitsI(452890) == 6);
	VL_EXPECT(vlm_getNumberOfDigitsI(-3) == 1);
	VL_EXPECT(vlm_getNumberOfDigitsI(-36324) == 5);
	VL_EXPECT(vlm_getNumberOfDigitsI(0) == 1);
	VL_EXPECT(vlm_getNumberOfDigitsI(1) == 1);

	VL_EXPECT(vlm_getDigitFromRightAtI(1234, 0) == 4);
	VL_EXPECT(vlm_getDigitFromRightAtI(1234, 1) == 3);
	VL_EXPECT(vlm_getDigitFromRightAtI(1234, 2) == 2);
	VL_EXPECT(vlm_getDigitFromRightAtI(1234, 3) == 1);

	VL_EXPECT(vlm_getReversedI(1234) == 4321);
	VL_EXPECT(vlm_getReversedI(12345) == 54321);
	VL_EXPECT(vlm_getReversedI(43211234) == 43211234);
	VL_EXPECT(vlm_getReversedI(152354) == 453251);

	VL_EXPECT(vldpr_getReversedNaive(1234) == 4321);
	VL_EXPECT(vldpr_getReversedNaive(12345) == 54321);
	VL_EXPECT(vldpr_getReversedNaive(43211234) == 43211234);
	VL_EXPECT(vldpr_getReversedNaive(152354) == 453251);

	{
		int a = 5, b = 10;

		vlu_swapI(&a, &b);
		VL_EXPECT(a == 10);
		VL_EXPECT(b == 5);
	}

	{
		int array[] = { 2, 5, 1, 4, 7, 1 };

		vla_sortSelectionI(array, 6);
		VL_EXPECT(array[0] == 1);
		VL_EXPECT(array[1] == 1);
		VL_EXPECT(array[2] == 2);
		VL_EXPECT(array[3] == 4);
		VL_EXPECT(array[4] == 5);
		VL_EXPECT(array[5] == 7);
	}

	{
		int array[] = { 2, 5, 1, 4, 7, 1 };

		VL_EXPECT(vla_isSortedI(array, 6) == false);

		vla_shiftToEndI(array, 6, 0);
		VL_EXPECT(array[0] == 5);
		VL_EXPECT(array[5] == 2);

		vla_shiftToEndI(array, 6, 0);
		VL_EXPECT(array[0] == 1);
		VL_EXPECT(array[4] == 2);
		VL_EXPECT(array[5] == 5);
	}

	{
		int array[] = { 2, 5, 1, 4, 7, 1, 4 };
		size_t newSize;

		VL_EXPECT(vla_isSortedI(array, 7) == false);

		vla_sortSelectionI(array, 7);
		vla_uniquifyI(array, 7, &newSize);
		VL_EXPECT(newSize == 5);
		VL_EXPECT(array[0] == 1);
		VL_EXPECT(array[1] == 2);
		VL_EXPECT(array[2] == 4);
		VL_EXPECT(array[3] == 5);
		VL_EXPECT(array[4] == 7);

		VL_EXPECT(vla_linearSearchI(array, 5, 4) == 2);
		VL_EXPECT(vla_linearSearchI(array, 5, 7) == 4);
		VL_EXPECT(vla_linearSearchI(array, 5, 99) == -1);

		VL_EXPECT(vla_binarySearchI(array, 5, 4) == 2);
		VL_EXPECT(vla_binarySearchI(array, 5, 7) == 4);
		VL_EXPECT(vla_binarySearchI(array, 5, 99) == -1);

		VL_EXPECT(vla_isSortedI(array, 5) == true);
	}
}
