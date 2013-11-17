#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <VeeLib/VeeLib.h>

void runTests();

void choiceNewVersion()
{
	printf("Insert an integer: ");
	int number = getScanfI(), reverse = getReversed(number);
	printf("\nThe reversed integer is: %d\nThe number is %spalindrome!", reverse, number == reverse ? "" : "not ");
}

void choiceNaiveVersion()
{
	printf("Insert an integer: \nThe reversed integer is: ");
	
	int number = getScanfI();
	while(number != 0)
	{ 
		printf("%d", number % 10);
		number /= 10;
	}
}

int main()
{	
	runTests();

	const char* choiceDescs[] = 
	{
		"Print a number's reverse and whether it's palindrome.",
		"Print a number's reverse. (naive version)"
	};

	void(*fnPtrs[])() = 
	{
		&choiceNewVersion,
		&choiceNaiveVersion
	};

	showMenu(2, choiceDescs, fnPtrs);
	return 0;
}

void runTests()
{
	VL_EXPECT(getNumberOfDigits(1000) == 4);
	VL_EXPECT(getNumberOfDigits(111) == 3);
	VL_EXPECT(getNumberOfDigits(452890) == 6);
	VL_EXPECT(getNumberOfDigits(-3) == 1);
	VL_EXPECT(getNumberOfDigits(-36324) == 5);
	VL_EXPECT(getNumberOfDigits(0) == 1);
	VL_EXPECT(getNumberOfDigits(1) == 1);

	VL_EXPECT(getDigitFromRightAt(1234, 0) == 4);
	VL_EXPECT(getDigitFromRightAt(1234, 1) == 3);
	VL_EXPECT(getDigitFromRightAt(1234, 2) == 2);
	VL_EXPECT(getDigitFromRightAt(1234, 3) == 1);
}