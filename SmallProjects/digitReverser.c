// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <VeeLib/VeeLib.h>

void runTests();

void choiceNewVersion()
{
	printf("Insert an integer: ");
	int number = vlc_getScanfInt(), reverse = vlm_getReversed(number);
	printf("\nThe reversed integer is: %d\nThe number is %spalindrome!", reverse, number == reverse ? "" : "not ");
}

void choiceNaiveVersion()
{
	printf("Insert an integer: ");
	int number = vlc_getScanfInt(), reverse = vldpr_getReversedNaive(number);
	printf("\nThe reversed integer is: %d\nThe number is %spalindrome!", reverse, number == reverse ? "" : "not ");
}

void choiceNaivePrintfVersion()
{
	printf("Insert an integer: \nThe reversed integer is: ");
	
	int number = vlc_getScanfInt();
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
	number = min = max = prevNumber = vlc_getScanfInt();

	if(min == -1 && max == -1) { printf("The entered sequence is empty."); return; }
	
	while(number != -1) 
	{
		if(number < prevNumber) increasing = false;
		if(number % 2 == 0) ++evenCount;

		min = vlm_getMin(min, number);
		max = vlm_getMax(max, number);
		
		prevNumber = number;
		number = vlc_getScanfInt();
	}

	printf("Min: %d\tMax: %d\tEven count: %d\nThe sequence is in %sincreasing order\n", min, max, evenCount, increasing ? "" : "not in ");
}

void choiceCountMultipleOf()
{
	printf("Enter lower bound (inclusive)\n");
	int lb = vlc_getScanfInt();

	printf("Enter upper bound (exclusive)\n");
	int ub = vlc_getScanfInt();

	printf("Enter divisor\n");
	int divisor = vlc_getScanfInt();

	vlc_clearScreen();

	int i, firstMultiple;
	for(i = lb; i < ub; ++i) if(i % divisor == 0) { firstMultiple = i; break; }

	for(i = firstMultiple; i < ub; i += divisor) printf("%d\n", i);
}

void choicePowOf2()
{
	printf("Enter power base\n");
	int base = vlc_getScanfInt();

	printf("Enter how many powers to calculate\n");
	int count = vlc_getScanfInt();

	vlc_clearScreen();

	int i, n = 1;
	for(i = 0; i < count; ++i) printf("%d\n", n *= base);
}

void choiceFactorial()
{
	printf("Enter number\n");
	int number = vlc_getScanfInt();	

	vlc_clearScreen();

	printf("%d\n", vlm_getFactorial(number));
}

void choiceFibonacci() { printf(vlm_getFibonacci(vlc_getScanfInt())); }

void choiceFindLog()
{
	printf("Insert base\n");
	int base = vlc_getScanfInt();

	printf("Insert argument\n");
	int arg = vlc_getScanfInt();

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
	float lb = vlc_getScanfFloat();

	printf("Enter upper bound (exclusive)\n");
	float ub = vlc_getScanfFloat();

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

int main()
{	
	runTests();

	const size_t choiceCount = 10;

	const char* choiceDescs[choiceCount] = 
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
		"Find function using bisection"
	};

	void(*fnPtrs[choiceCount])() = 
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
		&choiceFunctionExercise
	};

	vlc_showMenu(choiceCount, choiceDescs, fnPtrs);
	return 0;
}

void runTests()
{
	VL_EXPECT(vlm_getNumberOfDigits(1000) == 4);
	VL_EXPECT(vlm_getNumberOfDigits(111) == 3);
	VL_EXPECT(vlm_getNumberOfDigits(452890) == 6);
	VL_EXPECT(vlm_getNumberOfDigits(-3) == 1);
	VL_EXPECT(vlm_getNumberOfDigits(-36324) == 5);
	VL_EXPECT(vlm_getNumberOfDigits(0) == 1);
	VL_EXPECT(vlm_getNumberOfDigits(1) == 1);

	VL_EXPECT(vlm_getDigitFromRightAt(1234, 0) == 4);
	VL_EXPECT(vlm_getDigitFromRightAt(1234, 1) == 3);
	VL_EXPECT(vlm_getDigitFromRightAt(1234, 2) == 2);
	VL_EXPECT(vlm_getDigitFromRightAt(1234, 3) == 1);

	VL_EXPECT(vlm_getReversed(1234) == 4321);
	VL_EXPECT(vlm_getReversed(12345) == 54321);
	VL_EXPECT(vlm_getReversed(43211234) == 43211234);
	VL_EXPECT(vlm_getReversed(152354) == 453251);

	VL_EXPECT(vldpr_getReversedNaive(1234) == 4321);
	VL_EXPECT(vldpr_getReversedNaive(12345) == 54321);
	VL_EXPECT(vldpr_getReversedNaive(43211234) == 43211234);
	VL_EXPECT(vldpr_getReversedNaive(152354) == 453251);
}