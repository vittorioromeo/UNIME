#ifndef VL_VEELIB
#define VL_VEELIB

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#define VL_EXPECT(x) do{ if(!(x)){ printf("Test << " #x " failed! >>\n"); exit(1); } } while(0); 

inline int getNumberOfDigits(int mValue)
{
	int result = 0;
	do { mValue /= 10; ++result; } while(mValue != 0); 
	return result;
}

inline int getDigitFromRightAt(int mNumber, int mPosFromRight)
{
	assert(mPosFromRight < getNumberOfDigits(mNumber));

	int powPosition = pow(10, mPosFromRight + 1);
	return mNumber % powPosition / (powPosition / 10);
}

inline int getReversed(int mNumber)
{
	int result = 0, numberOfDigits = getNumberOfDigits(mNumber), k;
	for(k = 0; k < numberOfDigits; ++k) result += getDigitFromRightAt(mNumber, k) * pow(10, (numberOfDigits - k - 1));
	return result;
}

inline int getScanfI() { int result; scanf("%d", &result); return result; }
inline void waitForAnyKey() { char temp; scanf("%c", &temp); scanf("%c", &temp); }

inline void showMenu(int mNumberOfChoices, const char** mChoiceDescArray, void(*mFnPtrArray[])())
{
	while(1)
	{
		printf("\nWelcome! Choose:");

		int i;
		for(i = 0; i < mNumberOfChoices; ++i) printf("\n(%d): %s", i, mChoiceDescArray[i]);
		printf("\n(%d): %s", i, "Exit.");
		
		int choice;
		scanf("%d", &choice);

		if(choice >= mNumberOfChoices) return;

		mFnPtrArray[choice]();
		waitForAnyKey();
	}
}

#endif