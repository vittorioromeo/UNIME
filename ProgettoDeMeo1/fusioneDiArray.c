// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <stdio.h>
#include <stdlib.h>

/// @brief Computes the union of `mAInc` and `mADec` in `mATarget`.
/// @details `mATarget` will be filled in increasing order.
/// @param mAInc Array in increasing order.
/// @param mADec Array in decreasing order.
int arrayUnionInc(int* mAInc, int mAIncSize, int* mADec, int mADecSize, int* mATarget)
{
 	int itrInc = 0, itrDec = mADecSize - 1, itrTarget = 0, a, b;

 	while(itrInc < mAIncSize && itrDec >= 0)
 	{
 		a = mAInc[itrInc];
 		b = mADec[itrDec];
 		
 		// If a == b, append one to the target array and move both iterators
 		// If a < b, append a and move a iterator.
 		// Otherwise, append b and move b iterator.

 		if(a == b) 
 		{
 			mATarget[itrTarget++] = a;		
 			while(mAInc[++itrInc] == a) { } 	// Skip duplicates in source arrays
			while(mADec[--itrDec] == b) { }
 		}
 		else if(a < b) 
 		{
 			mATarget[itrTarget++] = a;
 			while(mAInc[++itrInc] == a) { }
 		}
 		else
 		{
 			mATarget[itrTarget++] = b;	
 			while(mADec[--itrDec] == b) { }
 		} 		
 	}

 	// Add remaining elements

 	while(itrInc < mAIncSize)
 	{
 		a = mAInc[itrInc];
 		mATarget[itrTarget++] = a;
 		while(mAInc[++itrInc] == a) { }	
 	}

 	while(itrDec >= 0)
 	{
 		b = mADec[itrDec];
 		mATarget[itrTarget++] = b;
 		while(mADec[--itrDec] == b) { }	
 	}

 	return itrTarget;
}

/// @brief Computes the union of `mAInc` and `mADec` in `mATarget`.
/// @details `mATarget` will be filled in decreasing order.
/// @param mAInc Array in increasing order.
/// @param mADec Array in decreasing order.
int arrayUnionDec(int* mAInc, int mAIncSize, int* mADec, int mADecSize, int* mATarget)
{
 	int itrInc = mAIncSize - 1, itrDec = 0, itrTarget = 0, a, b;

 	while(itrInc >= 0 && itrDec < mADecSize)
 	{
 		a = mAInc[itrInc];
 		b = mADec[itrDec];
 		
 		// If a == b, append one to the target array and move both iterators
 		// If a > b, append a and move a iterator.
 		// Otherwise, append b and move b iterator.

 		if(a == b) 
 		{
 			mATarget[itrTarget++] = a;		
 			while(mAInc[--itrInc] == a) { } 	// Skip duplicates in source arrays
			while(mADec[++itrDec] == b) { }
 		}
 		else if(a > b) 
 		{
 			mATarget[itrTarget++] = a;
 			while(mAInc[--itrInc] == a) { }
 		}
 		else
 		{
 			mATarget[itrTarget++] = b;	
 			while(mADec[++itrDec] == b) { }
 		} 		
 	}

 	// Add remaining elements

 	while(itrInc >= 0)
 	{
 		a = mAInc[itrInc];
 		mATarget[itrTarget++] = a;
 		while(mAInc[--itrInc] == a) { }	
 	}

 	while(itrDec < mADecSize)
 	{
 		b = mADec[itrDec];
 		mATarget[itrTarget++] = b;
 		while(mADec[++itrDec] == b) { }	
 	}

 	return itrTarget;
}

/* 
| 
|	/---------------------------\
|	| Computational complexity: |
|	\---------------------------/
|	
|	The computational complexity is obviously linear, O(max(mAIncSize, mADecSize)).
|   In the case that mAIncSize >= mADecSize, then the worst complexity is O(mAIncSize);
|   In the case that mAIncSize < mADecSize, then the worst complexity is O(mADecSize);
|
*/

int main()
{
	int aInc[] = {1, 1, 3, 4, 5, 12, 13, 22, 22, 23};
	int aIncSize = sizeof(aInc) / sizeof(aInc[0]);

	int aDec[] = {30, 23, 22, 21, 14, 13, 12, 11, 10, 6, 4, 2, 1, 0};
	int aDecSize = sizeof(aDec) / sizeof(aDec[0]);

	int aTargetSize = aIncSize + aDecSize;
	int* aTarget = (int*) malloc(aTargetSize * sizeof(int));

	{
		int aTargetNewSize = arrayUnionInc(aInc, aIncSize, aDec, aDecSize, aTarget), i;
		for(i = 0; i < aTargetNewSize; ++i) printf(" %d ", aTarget[i]);
		printf("\n");
	}

	{
		int aTargetNewSize = arrayUnionDec(aInc, aIncSize, aDec, aDecSize, aTarget), i;
		for(i = 0; i < aTargetNewSize; ++i) printf(" %d ", aTarget[i]);
	}


	free(aTarget);
	return 0;
}	