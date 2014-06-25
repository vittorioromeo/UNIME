// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/// @brief Returns the count of `mKey` elements in `mArray`.
int f1RecursiveCount(int* mArray, int mSize, int mKey)
{
	if(mSize == 0) return 0;
	return (mArray[0] == mKey) + f1RecursiveCount(mArray + 1, mSize - 1, mKey);	
}

/// @brief Returns -1 if `mKey` is not present in `mArray`, otherwise its index.
int f2RecursiveExists(int* mArray, int mSize, int mKey)
{
	if(mSize == 0) return -1;
	if(mArray[mSize - 1] == mKey) return mSize - 1;
	return f2RecursiveExists(mArray, mSize - 1, mKey);	
}

int f2ImplWithF1(int* mArray, int mSize, int mKey)
{
	return f1RecursiveCount(mArray, mSize, mKey);
}

int f1ImplWithF2(int* mArray, int mSize, int mKey)
{
	int count = -1, temp;
	do
	{	
		++count;
		temp = f2RecursiveExists(mArray, mSize, mKey);
		mSize = temp;
	} while(temp != -1);

	return count;
}

// F1 is more costly than F2, since it always goes through the whole array,
// while F2 will imemdiately stop upon finding the desired key.

// As shown above, it is possible to implement F1 with F2 and viceversa,
// provided that the functions return useful values such as the index of 
// the desired key.

void recursiveInversePrint(int* mArray, int mSize)
{
	if(mSize == 0)
	{
		printf("\n");
	 	return;
	}
	
	printf("%d, ", mArray[mSize - 1]);
	recursiveInversePrint(mArray, mSize - 1);
}

float getFactorial(float mX)
{
	if(mX == 0 || mX == 1) return 1;
	return mX * getFactorial(mX - 1);	
}

float sinMcLaurinRecursive(float mX, int mN)
{
	float value = pow(-1.0, mN) * ((pow(mX, 2.0 * mN + 1))/getFactorial(2.0 * mN + 1));

	if(mN == 0) return value;
	return value + sinMcLaurinRecursive(mX, mN - 1);
}

int main()
{
	int array[] = {1, 2, 4, 1, 4, 3, 2, 1, 1, 7, 12};
	int size = sizeof array / sizeof(int);

	printf("%d\n", 		f1RecursiveCount(array, size, 1));
	printf("%d\n\n", 	f1RecursiveCount(array, size, 4));

	printf("%d\n", 		f2RecursiveExists(array, size, 1) != -1);
	printf("%d\n", 		f2RecursiveExists(array, size, 2) != -1);
	printf("%d\n", 		f2RecursiveExists(array, size, 4) != -1);
	printf("%d\n\n\n", 	f2RecursiveExists(array, size, 122) != -1);

	printf("%d\n", 		f1ImplWithF2(array, size, 1));
	printf("%d\n\n", 	f1ImplWithF2(array, size, 4));

	printf("%d\n", 		f2ImplWithF1(array, size, 1) != 0);
	printf("%d\n", 		f2ImplWithF1(array, size, 2) != 0);
	printf("%d\n", 		f2ImplWithF1(array, size, 4) != 0);
	printf("%d\n", 		f2ImplWithF1(array, size, 122) != 0);

	recursiveInversePrint(array, size);
	printf("sin(3.14 * 0.2): %f\n", sinMcLaurinRecursive(3.14 * 0.2, 9));
	printf("sin(3.14 * 0.4): %f\n", sinMcLaurinRecursive(3.14 * 0.4, 9));
	printf("sin(3.14 * 0.6): %f\n", sinMcLaurinRecursive(3.14 * 0.6, 9));
	printf("sin(3.14 * 0.8): %f\n", sinMcLaurinRecursive(3.14 * 0.8, 9));
	printf("sin(3.14 * 1.0): %f\n", sinMcLaurinRecursive(3.14 * 1.0, 9));
	printf("sin(3.14 * 1.2): %f\n", sinMcLaurinRecursive(3.14 * 1.2, 9));
	printf("sin(3.14 * 1.4): %f\n", sinMcLaurinRecursive(3.14 * 1.4, 9));
	printf("sin(3.14 * 1.6): %f\n", sinMcLaurinRecursive(3.14 * 1.6, 9));
}		