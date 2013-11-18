// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef VL_UTILS_MATH
#define VL_UTILS_MATH

#include "VeeLib/Global/Common.h"

/// @brief Returns the number of digits in an integer.
/// @details Internally repeatedly divides the number by 10
///		     mathematically - does not make use of arrays.
/// @param mValue Number to use.
inline int vlm_getNumberOfDigits(int mValue)
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
inline int vlm_getDigitFromRightAt(int mValue, int mPosFromRight)
{
	assert(mPosFromRight < vlm_getNumberOfDigits(mValue));

	int powPosition = pow(10, mPosFromRight + 1);
	return mValue % powPosition / (powPosition / 10);
}

/// @brief Returns the reverse of an integer.
/// @details Internally uses mod, division and mutliplication. 
/// @param mValue Number to reverse.
inline int vlm_getReversed(int mValue)
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

// TODO: docs
inline int vlm_max(int mA, int mB) { return mA > mB ? mA : mB; }
inline int vlm_min(int mA, int mB) { return mA < mB ? mA : mB; }

#endif