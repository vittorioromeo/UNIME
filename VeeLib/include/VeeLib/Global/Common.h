// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef VL_GLOBAL_COMMON
#define VL_GLOBAL_COMMON

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

// Define bool, true, false macros if C99 is not being used
#ifndef __bool_true_false_are_defined
	#define false 0
	#define true !false
	typedef int bool;
#endif

#define VL_EXPECT(x) do{ if(!(x)){ printf("Test << " #x " failed! >>\n"); exit(1); } } while(false); 

// Prefixes:
// 		vlm_:		math functions
//		vlc_:		console functions
//		vldpr_:		deprecated functions

#endif