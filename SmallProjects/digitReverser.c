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

		min = vlm_min(min, number);
		max = vlm_max(max, number);
		
		prevNumber = number;
		number = vlc_getScanfInt();
	}

	printf("Min: %d\tMax: %d\tEven count: %d\nThe sequence is in %sincreasing order\n", min, max, evenCount, increasing ? "" : "not in ");
}

int main()
{	
	runTests();

	const char* choiceDescs[] = 
	{
		"Print a number's reverse and whether it's palindrome.",
		"Print a number's reverse and whether it's palindrome. (naive version)",
		"Print a number's reverse. (naive printf version)",
		"Print an integer sequence's min and max numbers."
	};

	void(*fnPtrs[])() = 
	{
		&choiceNewVersion,
		&choiceNaiveVersion,
		&choiceNaivePrintfVersion,
		&choiceSequenceMinMax
	};

	vlc_showMenu(4, choiceDescs, fnPtrs);
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