// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef VL_UTILS_UTILITY
#define VL_UTILS_UTILITY

#include "VeeLib/Global/Common.h"

/// @brief Swaps two integers.
inline void vlu_swapI(int* mA, int* mB) { int temp = *mA; *mA = *mB; *mB = temp; }

/// @brief Swaps two chars.
inline void vlu_swapC(char* mA, char* mB) { char temp = *mA; *mA = *mB; *mB = temp; }

/// @brief Swaps two floats.
inline void vlu_swapF(float* mA, float* mB) { float temp = *mA; *mA = *mB; *mB = temp; }

/// @brief Swaps two doubles.
inline void vlu_swapD(double* mA, double* mB) { double temp = *mA; *mA = *mB; *mB = temp; }

/// @brief Swaps two longs.
inline void vlu_swapL(long* mA, long* mB) { long temp = *mA; *mA = *mB; *mB = temp; }

/// @brief Swaps two unsigned ints.
inline void vlu_swapUI(unsigned int* mA, unsigned int* mB) { unsigned int temp = *mA; *mA = *mB; *mB = temp; }

/// @brief Swaps two unsigned longs.
inline void vlu_swapUL(unsigned long* mA, unsigned long* mB) { unsigned long temp = *mA; *mA = *mB; *mB = temp; }

#endif
