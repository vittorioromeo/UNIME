// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <VeeLib/VeeLib.h>

/// @brief Returns the smallest element in an array [mLB, mUB) range.
/// @param mArray Array to search.
/// @param mLB Inclusive lower-bound.
/// @param mUB Exclusive lower-bound.
ArrayIdx getMinArrayElementIdx(int* mArray, ArrayIdx mLB, ArrayIdx mUB)
{
	// To find an unsorted array's smallest element naively, we can
	// simply loop through the array storing the index of the minimum
	// value, then return the stored index after the loop.

	ArrayIdx iMin;
	for(iMin = mLB; mLB < mUB; ++mLB) if(mArray[mLB] < mArray[iMin]) iMin = mLB;
	return iMin;
}

/// @brief Sorts an integer array, using the "selection sort" algorithm.
/// @param mArray Array to sort.
/// @param mSize Size of the array to sort.
void selectionSort(int* mArray, size_t mSize)
{
	// To sort an array using the naive "seletion sort algorithm", 
	// it is sufficient to continously find the minimum array value
	// and place it at the beginning of the array, then continue (starting
	// from the value after the last found minimum) until the end.

	ArrayIdx i;		 							                            		
	for(i = 0; i < mSize; ++i) vlu_swapI(&mArray[i], &mArray[getMinArrayElementIdx(mArray, i, mSize)]);	
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
|	complete algorithm is (N^2).
|	
*/ 

bool bruteForceSumExistance(int* mArray, size_t mSize, int mDesiredValue)
{
	int i, j;

	for(i = 0; i < mSize - 1; ++i)
		for(j = i + 1; j < mSize; ++j)
			if(mArray[i] + mArray[j] == mDesiredValue) return true;

	return false;
}

// TODO: vvvv

bool sortedSumExistance(int* mArray, size_t mSize, int mDesiredValue)
{
	assert(vla_isSortedI(mArray, mSize));

	// | 1 | 3 | 5 | 5 | 7 | 11 | 14 |

	int pivot;
	for(pivot = 0; pivot < mSize; ++pivot) if(vla_binarySearchI(mArray, mSize, mDesiredValue - pivot) != -1) return true;
	
	return false;
}

void printSmallestSequenceSum(int* mArray, size_t mSize, int mDesiredValue)
{
	assert(vla_isSortedI(mArray, mSize));

	printf("\nSmallest sequence for (>%d): { ", mDesiredValue);

	ArrayIdx lb = 0, mid;

	while(lb <= mSize)
	{
		mid = (lb + mSize) / 2;

		if(mArray[mid] >= mDesiredValue) 
		{
			printf("%d }", mArray[mid]);
			return;
		}
		
		if(mArray[mid] < mDesiredValue) lb = mid + 1;
	}

	int accumulator = 0;
	for(mid = mSize - 1; mid > 0; --mid)
	{
		printf("%d ", mArray[mid]);

		accumulator += mArray[mid];
		if(accumulator >= mDesiredValue) break;
	}

	if(accumulator < mDesiredValue) printf("... not found!");

	printf("}\n");
}

int main()
{
	int array[] = { 4, 3, 6, 2, 7, 11, 5, 8, 9 };
	size_t arraySize = VL_GET_ARRAY_SIZE(array);

	selectionSort(array, arraySize);

	int i;
	for(i = 0; i < arraySize; ++i) printf("%d, ", array[i]);
	
	printf("\n");

	printf("%d\n", bruteForceSumExistance(array, arraySize, 16));
	printf("%d\n", bruteForceSumExistance(array, arraySize, 17));
	printf("%d\n", bruteForceSumExistance(array, arraySize, 1));
	printf("%d\n", bruteForceSumExistance(array, arraySize, 91));

	printf("\n");

	printf("%d\n", sortedSumExistance(array, arraySize, 16));
	printf("%d\n", sortedSumExistance(array, arraySize, 17));
	printf("%d\n", sortedSumExistance(array, arraySize, 1));
	printf("%d\n", sortedSumExistance(array, arraySize, 91));

	printSmallestSequenceSum(array, arraySize, 15);	
	printSmallestSequenceSum(array, arraySize, 20);	
	printSmallestSequenceSum(array, arraySize, 25);	
	printSmallestSequenceSum(array, arraySize, 30);	
	printSmallestSequenceSum(array, arraySize, 18);
	printSmallestSequenceSum(array, arraySize, 1);
	printSmallestSequenceSum(array, arraySize, 3);
	printSmallestSequenceSum(array, arraySize, 0);				
	printSmallestSequenceSum(array, arraySize, 999);	
}	