// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <VeeLib/VeeLib.h>

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

	// Print array
	{
		int i;
		vlc_setFmt(vlc_StyleBold, vlc_ColorYellow);
		printf("Array: ");
		vlc_resetFmt();
		for(i = 0; i < size; ++i) printf("%d, ", array[i]);
		printf("\n\n");
	}

	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("F1 recursive count:\n");
	vlc_resetFmt();
	printf("Count of 1: %d\n", 		f1RecursiveCount(array, size, 1));
	printf("Count of 4: %d\n\n", 	f1RecursiveCount(array, size, 4));

	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("F2 recursive exists:\n");
	vlc_resetFmt();
	printf("Exists? (1): %d\n", 		f2RecursiveExists(array, size, 1) != -1);
	printf("Exists? (2): %d\n", 		f2RecursiveExists(array, size, 2) != -1);
	printf("Exists? (4): %d\n", 		f2RecursiveExists(array, size, 4) != -1);
	printf("Exists? (122): %d\n\n\n", 	f2RecursiveExists(array, size, 122) != -1);

	vlc_setFmt(vlc_StyleBold, vlc_ColorGreen);
	printf("F1 implemented with F2:\n");
	vlc_resetFmt();
	printf("Count of 1: %d\n", 		f1ImplWithF2(array, size, 1));
	printf("Count of 4: %d\n\n", 	f1ImplWithF2(array, size, 4));

	vlc_setFmt(vlc_StyleBold, vlc_ColorGreen);
	printf("F2 implemented with F1:\n");
	vlc_resetFmt();
	printf("Exists? (1): %d\n", 		f2ImplWithF1(array, size, 1) != 0);
	printf("Exists? (2): %d\n", 		f2ImplWithF1(array, size, 2) != 0);
	printf("Exists? (4): %d\n", 		f2ImplWithF1(array, size, 4) != 0);
	printf("Exists? (122): %d\n\n\n", 	f2ImplWithF1(array, size, 122) != 0);

	vlc_setFmt(vlc_StyleBold, vlc_ColorYellow);
	printf("Reverse recursive print:\n");
	vlc_resetFmt();
	recursiveInversePrint(array, size);
	printf("\n");

	vlc_setFmt(vlc_StyleBold, vlc_ColorYellow);
	printf("Sin value tests:\n");
	vlc_resetFmt();
	printf("sin(3.14 * 0.2): %f\n", sinMcLaurinRecursive(3.14 * 0.2, 9));
	printf("sin(3.14 * 0.4): %f\n", sinMcLaurinRecursive(3.14 * 0.4, 9));
	printf("sin(3.14 * 0.6): %f\n", sinMcLaurinRecursive(3.14 * 0.6, 9));
	printf("sin(3.14 * 0.8): %f\n", sinMcLaurinRecursive(3.14 * 0.8, 9));
	printf("sin(3.14 * 1.0): %f\n", sinMcLaurinRecursive(3.14 * 1.0, 9));
	printf("sin(3.14 * 1.2): %f\n", sinMcLaurinRecursive(3.14 * 1.2, 9));
	printf("sin(3.14 * 1.4): %f\n", sinMcLaurinRecursive(3.14 * 1.4, 9));
	printf("sin(3.14 * 1.6): %f\n", sinMcLaurinRecursive(3.14 * 1.6, 9));
}		