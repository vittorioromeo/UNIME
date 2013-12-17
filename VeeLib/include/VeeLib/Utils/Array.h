// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef VL_UTILS_ARRAY
#define VL_UTILS_ARRAY

#include "VeeLib/Global/Common.h"
#include "VeeLib/Utils/Utility.h"
#include "VeeLib/Utils/Math.h"

inline ArrayIdx vla_getMinValueIdxI(int* mArray, size_t mLB, size_t mUB)
{
	// This functions returns the index of the smallest value in an
	// integer array, in the interval [mLB; mUB).

	ArrayIdx result = mLB, i;
	for(i = mLB; i < mUB; ++i) if(mArray[i] < mArray[result]) result = i;
	return result;
}

inline void vla_sortSelectionI(int* mArray, size_t mSize)
{
	// Swap the smallest value with the leftmost unsorted array value until the array is sorted.

	ArrayIdx i;
	for(i = 0; i < mSize - 1; ++i) vlu_swapI(&mArray[vla_getMinValueIdxI(mArray, i, mSize)], &mArray[i]);
}

inline void vla_shiftToEndI(int* mArray, size_t mSize, int mIdx)
{
	// This function shifts an integer array's element towards the end of the array.

	ArrayIdx i;
	for(i = mIdx + 1; i < mSize; ++i) vlu_swapI(&mArray[i], &mArray[i - 1]);
}

inline bool vla_isSortedI(int* mArray, size_t mSize)
{
	ArrayIdx i;
	for(i = 0; i < mSize - 1; ++i) if(mArray[i] > mArray[i + 1]) return false;
	return true;
}

inline void vla_uniquifyI(int* mArray, size_t mSize, size_t* mNewSize)
{
	assert(vla_isSortedI(mArray, mSize));

	ArrayIdx i, p = 0;
	for(i = 1; i < mSize; ++i) if(mArray[i] != mArray[p]) mArray[++p] = mArray[i];
	*mNewSize = p + 1;
}

inline ArrayIdx vla_linearSearchI(int* mArray, size_t mSize, int mValue)
{
	ArrayIdx i;
	for(i = 0; i < mSize; ++i) if(mArray[i] == mValue) return i;
	return -1;
}

inline ArrayIdx vla_binarySearchI(int* mArray, size_t mSize, int mValue)
{
	assert(vla_isSortedI(mArray, mSize));

	ArrayIdx lb = 0, ub = mSize, mid;

	while(lb <= ub)
	{
		mid = (lb + ub) / 2;

		if(mArray[mid] > mValue) ub = mid - 1;
		else if(mArray[mid] < mValue) lb = mid + 1;
		else return mid;
	}

	return -1;
}

#endif
