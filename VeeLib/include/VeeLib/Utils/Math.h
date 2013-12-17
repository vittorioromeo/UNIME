// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef VL_UTILS_MATH
#define VL_UTILS_MATH

#include "VeeLib/Global/Common.h"
#include "VeeLib/Utils/Utility.h"

/// @brief Returns the number of digits in an integer.
/// @details Internally repeatedly divides the number by 10
///		     mathematically - does not make use of arrays.
/// @param mValue Number to use.
inline int vlm_getNumberOfDigitsI(int mValue)
{
	int result = 0;
	do { mValue /= 10; ++result; } while(mValue != 0);

	assert(result > 0);
	return result;
}

/// @brief Returns a digit in a specific position in a number
/// @details Internally uses pow and division. The position
///		     of the digit is counted from the right of the number.
/// @param mValue Number to use.
/// @param mPosFromRight Position of the digit, from the right. (0 == last digit)
inline int vlm_getDigitFromRightAtI(int mValue, int mPosFromRight)
{
	assert(mPosFromRight < vlm_getNumberOfDigitsI(mValue));

	int powPosition = pow(10, mPosFromRight + 1);
	return mValue % powPosition / (powPosition / 10);
}

/// @brief Returns the reverse of an integer.
/// @details Internally uses mod, division and mutliplication.
/// @param mValue Number to reverse.
inline int vlm_getReversedI(int mValue)
{
	int result = 0;

	do
	{
		result = result * 10;
		result += mValue % 10;
		mValue /= 10;
	} while(mValue != 0);

	return result;
}

/// @brief Returns the max between two integers.
/// @param mA First integer.
/// @param mB Second integer.
inline int vlm_getMaxI(int mA, int mB) { return mA > mB ? mA : mB; }

/// @brief Returns the min between two integers.
/// @param mA First integer.
/// @param mB Second integer.
inline int vlm_getMinI(int mA, int mB) { return mA < mB ? mA : mB; }

// TODO: docs
inline int vlm_getFactorial(int mValue)
{
	if(mValue == 0 || mValue == 1) return 1;
	int last;
	for(last = mValue - 1; last > 0; --last) mValue *= last;
	return mValue;
}

inline int vlm_getFibonacci(int mValue)
{
	int n1 = 1, n2 = 1, result = 0, i;

	for(i = 0; i < mValue; ++i)
	{
		result = n1 + n2;
		n1 = n2;
		n2 = result;
	}

	return result;
}

#endif
