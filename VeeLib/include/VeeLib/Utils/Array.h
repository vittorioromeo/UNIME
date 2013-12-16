// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef VL_UTILS_ARRAY
#define VL_UTILS_ARRAY

#include "VeeLib/Global/Common.h"
#include "VeeLib/Utils/Utility.h"
#include "VeeLib/Utils/Math.h"

inline int vla_getMinValueIdxI(int* mArray, int mLB, int mUB)
{
	// This functions returns the index of the smallest value in an
	// integer array, in the interval [mLB; mUB).

	int result = mLB, i;
	for(i = mLB; i < mUB; ++i) if(mArray[i] < mArray[result]) result = i;
	return result;
}

inline void vla_sortSelectionI(int* mArray, int mSize)
{
	int i;

	// Swap the smallest value with the leftmost unsorted array value until the array is sorted.
	for(i = 0; i < mSize - 1; ++i) vlu_swapI(&mArray[vla_getMinValueIdxI(mArray, i, mSize)], &mArray[i]);
}

inline void vla_shiftToEndI(int* mArray, int mSize, int mIdx)
{
	// This function shifts an integer array's element towards the end of the array.

	int i;
	for(i = mIdx + 1; i < mSize; ++i) vlu_swapI(&mArray[i], &mArray[i - 1]);
}

inline void vla_uniquifyInPlaceI(int* mArray, int mSize, int* mNewSize)
{
	int newSize = mSize;

	if(mSize >= 2)
	{
		int i, prev = mArray[0];
		for(i = 1; i < mSize; ++i)
		{
			if(mArray[i] == prev)
			{
				vla_shiftToEndI(mArray, mSize, i);
				--newSize;
			}

			prev = mArray[i];
		}
	}

	*mNewSize = newSize;
}

#endif
