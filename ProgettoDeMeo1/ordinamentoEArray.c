// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <stdio.h>
#include <stdlib.h>
#include <VeeLib/VeeLib.h>

/// @brief Utility function to swap two int values.
void swapInt(int* mA, int *mB)
{
	int temp = *mA;
	*mA = *mB;
	*mB = temp;
}



/// @brief Returns the smallest element in an array [mLB, mUB) range.
/// @param mLB Inclusive lower-bound.
/// @param mUB Exclusive lower-bound.
int getMinArrayElementIdx(int* mArray, int mLB, int mUB)
{
	// To find an unsorted array's smallest element naively, we can
	// simply loop through the array storing the index of the minimum
	// value, then return the stored index after the loop.

	int iMin;
	for(iMin = mLB; mLB < mUB; ++mLB) if(mArray[mLB] < mArray[iMin]) iMin = mLB;
	return iMin;
}

/// @brief Sorts an integer array, using the "selection sort" algorithm.
/// @details O(N^2) computational complexity.
void selectionSort(int* mArray, int mSize)
{
	// To sort an array using the naive "seletion sort algorithm", 
	// it is sufficient to continously find the minimum array value
	// and place it at the beginning of the array, then continue (starting
	// from the value after the last found minimum) until the end.

	int i;		 							                            		
	for(i = 0; i < mSize; ++i) swapInt(&mArray[i], &mArray[getMinArrayElementIdx(mArray, i, mSize)]);	
}

/* 
| 
|	/---------------------------\
|	| Computational complexity: |
|	\---------------------------/
|	
|	We can think of the lower and upper bounds as (N + offset).
|	
|	/---- getMinArrayElementIdx()
|	|
|	|	C * ((N + offset1) - (N + offset2))
|	|	C * N
|	| 	N
|	\_________________________________________
|	
|	getMinArrayElementIdx() has (N) worst time complexity, if the lower bound is 0 and the upper bound is 
|	the array size.
|	
|	/---- selectionSort()
|	|
|	|	C * N * getMinArrayElementIdx()
|	|	C * N * N
|	| 	N^2
|	L_________________________________________
|	
|	selectionSort() calls getMinArrayElementIdx() (N) times. Therefore, the worst case complexity for the 
|	complete algorithm is O(N^2).
|	
*/ 



/// @brief Returns whether a pair of number whose sum equals `mDesiredValue` exists in `mArray`.
/// @details Brute force version. `mArray` can be unsorted. O(N^2) computational complexity.
int bruteForceSumExistance(int* mArray, int mSize, int mDesiredValue)
{
	int i, j;

	for(i = 0; i < mSize - 1; ++i)
		for(j = i + 1; j < mSize; ++j)
			if(mArray[i] + mArray[j] == mDesiredValue) return 0;

	return 1;
}



/// @brief Binary search algorithm for int arrays.
/// @return Returns -1 if the element wasn't found, the index of the element otherwise.
inline int binarySearchInt(int* mArray, int mSize, int mValue)
{
	int lb = 0, ub = mSize, mid;

	while(lb <= ub)
	{
		mid = (lb + ub) / 2;

		if(mArray[mid] > mValue) ub = mid - 1;
		else if(mArray[mid] < mValue) lb = mid + 1;
		else return mid;
	}

	return -1;
}

/// @brief Returns whether a pair of number whose sum equals `mDesiredValue` exists in `mArray`.
/// @details `mArray` must be sorted. O(N log N) computational complexity.
int sortedSumExistance(int* mArray, int mSize, int mDesiredValue)
{
	// Strategy: pick a pivot, then use binary search to look for `mDesiredValue - pivot` in the array

	int pivot;
	for(pivot = 0; pivot < mSize; ++pivot) if(binarySearchInt(mArray, mSize, mDesiredValue - pivot) != -1) return 0;
	
	return 1;
}

/// @brief Prints the smallest sequence greater than `mDesiredValue` found in `mArray`.
/// @details Assumes the array is sorted. Linear computational complexity.
void printSmallestSequenceSumSorted(int* mArray, int mSize, int mDesiredValue)
{
	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("Smallest sequence for (>%d): ", mDesiredValue);
	vlc_resetFmt();

	printf("{ ");

	int ub, accumulator = 0;

	for(ub = mSize - 1; accumulator <= mDesiredValue && ub >= 0; --ub)
	{		
		accumulator += mArray[ub];	
		printf("%d, ", mArray[ub]);
	}

	if(accumulator <= mDesiredValue) printf("... not found ... ");

	printf("}\n");
}

int main()
{
	int i;
	int array[] = {4, 3, 6, 2, 7, 11, 5, 8, 9};
	int arraySize = (sizeof(array) / sizeof(array[0]));

	// Print array
	{
		vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
		printf("Array: ");
		vlc_resetFmt();
		for(i = 0; i < arraySize; ++i) printf("%d, ", array[i]);
		printf("\n\n");
	}

	vlc_setFmt(vlc_StyleBold, vlc_ColorGreen);
	printf("Sorting array...\n\n");
	
	selectionSort(array, arraySize);

	// Print sorted array
	{
		vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
		printf("Sorted array: ");
		vlc_resetFmt();
		for(i = 0; i < arraySize; ++i) printf("%d, ", array[i]);
		printf("\n\n");
	}
	
	vlc_setFmt(vlc_StyleBold, vlc_ColorGreen);
	printf("Checking sum existance (brute force):\n");
	vlc_resetFmt();
	printf("(x + y = %d ?) -> %d\n", 16, bruteForceSumExistance(array, arraySize, 16));
	printf("(x + y = %d ?) -> %d\n", 17, bruteForceSumExistance(array, arraySize, 17));
	printf("(x + y = %d ?) -> %d\n", 1, bruteForceSumExistance(array, arraySize, 1));
	printf("(x + y = %d ?) -> %d\n", 91, bruteForceSumExistance(array, arraySize, 91));
	printf("\n");

	vlc_setFmt(vlc_StyleBold, vlc_ColorGreen);
	printf("Checking sum existance (binary search):\n");
	vlc_resetFmt();
	printf("(x + y = %d ?) -> %d\n", 16, sortedSumExistance(array, arraySize, 16));
	printf("(x + y = %d ?) -> %d\n", 17, sortedSumExistance(array, arraySize, 17));
	printf("(x + y = %d ?) -> %d\n", 1, sortedSumExistance(array, arraySize, 1));
	printf("(x + y = %d ?) -> %d\n", 91, sortedSumExistance(array, arraySize, 91));
	printf("\n");

	vlc_setFmt(vlc_StyleBold, vlc_ColorGreen);
	printf("Printing smallest sequences:\n");
	vlc_resetFmt();
	printSmallestSequenceSumSorted(array, arraySize, 15);	
	printSmallestSequenceSumSorted(array, arraySize, 20);	
	printSmallestSequenceSumSorted(array, arraySize, 25);	
	printSmallestSequenceSumSorted(array, arraySize, 30);	
	printSmallestSequenceSumSorted(array, arraySize, 18);
	printSmallestSequenceSumSorted(array, arraySize, 1);
	printSmallestSequenceSumSorted(array, arraySize, 3);
	printSmallestSequenceSumSorted(array, arraySize, 0);				
	printSmallestSequenceSumSorted(array, arraySize, 999);	
}	