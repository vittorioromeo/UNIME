// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef VL_DEPRECATED
#define VL_DEPRECATED

#include "VeeLib/Global/Common.h"
#include "VeeLib/Utils/Math.h"

/// @brief Returns the reverse of an integer. (naive version)
/// @details Internally uses mod, division and pow. 
/// @param mValue Number to reverse.
inline int vldpr_getReversedNaive(int mValue)
{
	int result = 0, numberOfDigits = vlm_getNumberOfDigits(mValue), k;
	for(k = 0; k < numberOfDigits; ++k) result += vlm_getDigitFromRightAt(mValue, k) * pow(10, (numberOfDigits - k - 1));
	return result;
}

#endif