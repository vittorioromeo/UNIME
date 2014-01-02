// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef VL_UTILS_UTILITY
#define VL_UTILS_UTILITY

#include "VeeLib/Global/Common.h"

inline void vlu_swapI(int* mA, int* mB)			{ int temp = *mA; *mA = *mB; *mB = temp; } \
inline void vlu_swapC(char* mA, char* mB)		{ char temp = *mA; *mA = *mB; *mB = temp; } \
inline void vlu_swapF(float* mA, float* mB)		{ float temp = *mA; *mA = *mB; *mB = temp; } \
inline void vlu_swapD(double* mA, double* mB)	{ double temp = *mA; *mA = *mB; *mB = temp; } \

#endif
