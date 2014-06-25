// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int multIterative(int mA, int mB)
{
	if(mA == 0 || mB == 0) return 0;

	int result = mA;

	while(mB != 1)
	{
		mA *= 2;
		mB = floor(mB / 2.0);

		result += mA;
	}

	return result;
}

int multRecursive(int mA, int mB)
{
	if(mA == 0 || mB == 0) return 0;
	if(mA == 1) return mB;
	if(mB == 1) return mA;

	return mA + multRecursive(mA * 2, floor(mB / 2.0));
}

// Proof:
// a * 0 = 0
// a * 1 = a
// a * 2 = 2a
// a * 3 = a * 2 + a = 2a + a
// a * 4 = a * 2 + a * 2 = 2a + 2a
// ...
// a * even = a * (even / 2) + a * (even / 2)
// a * odd = a + a * (odd - 1)

// Complexity of `multRecursive`:
// O(log2(mB)) - number of times `mB` can be divided by two

int main()
{
	printf("%d\n", multIterative(71, 15));
	printf("%d\n", multRecursive(71, 15));
}