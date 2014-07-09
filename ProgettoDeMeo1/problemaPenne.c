// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <VeeLib/VeeLib.h>

/// @brief This struct represents a 3d vector of (a, b, c) values plus one
/// validity flag.
typedef struct 
{ 
	int a;	// 5  * a 
	int b;	// 8  * b
	int c;	// 24 * c	
	int valid;
} ABC;

// Equation: 5a + 8b + 24c = n
// Unsolvable for:
//	    n < 5
//  5 < n < 8
// 		n = 9
// 10 < n < 15
// 15 < n < 20
// 20 < n < 24
// 		n = 27
// 		n = 29
// 		n = 37

// 5a + 8b + 24c = n 
// is a diophantine equation.
// Usually, such an equation has solutions if GCD(5, 8, 24) divides n.
// However, that takes into account negative values for a, b or c, which
// do not make sense for this hypotetical pen-buying situation.
// Thus, the only reliable way I found to determine impossible values is brute force.

/// @brief Initializes and returns a new instance of an ABC struct.
ABC createABC()
{
	ABC result; 
	result.a = result.b = result.c = result.valid = 0;
	return result;
}

/// @brief Tries to solve the pen problem for `mN`. 
/// @return Returns an ABC instance with the solution values, or with
/// `valid` set to 1 if no suitable values were found.
ABC solvePenProblem(int mN)
{
	ABC r = createABC();	

	// We begin by checking if `mN` is a multiple of 5, 8 or 24.
	// If it is, just return the dividend.
	
	if(mN % 5 == 0) 	{ r.a = mN / 5; return r; }
	if(mN % 8 == 0) 	{ r.b = mN / 8; return r; }
	if(mN % 24 == 0) 	{ r.c = mN / 24; return r; }

	// If `mN` is not perfectly divisible by any of the constants,
	// we try to find a solution through brute force.

	// We begin by getting the upper-bounds for `mN`.
	// It's obvious that the equation will go over `mN` if:
	// 		a > mN / 5 or
	// 		b > mN / 8 or
	//		c > mN / 24

	int ubA = mN / 5;
	int ubB = mN / 8;
	int ubC = mN / 24;

	// We then loop through all possible values of a, b, c, checking if 
	// we found a valid solution.

	for(r.a = 0; r.a <= ubA; ++r.a)
		for(r.b = 0; r.b <= ubB; ++r.b)
			for(r.c = 0; r.c <= ubC; ++r.c)							
				if(5 * r.a + 8 * r.b + 24 * r.c == mN) return r;
				
	// In case we couldn't find a suitable solution, set `r.valid` to 1.

	r.valid = 1;
	return r;
}

void solveAndPrint(int mN)
{
	ABC result = solvePenProblem(mN);
	
	vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
	printf("Solve:\t5 * a + 8 * b + 24 * c = %d\n", mN); 

	vlc_resetFmt();
	
	if(result.valid == 0) 
	{
		printf("----->\t5 * %d + 8 * %d + 24 * %d = %d\n", result.a, result.b, result.c, mN); 
		printf("----->\t%d + %d + %d = %d\n", 5 * result.a, 8 * result.b, 24 * result.c, mN); 
		
		vlc_setFmt(vlc_StyleBold, vlc_ColorGreen);
		printf("----->\t%d = %d\n", 5 * result.a + 8 * result.b + 24 * result.c, mN);
	}
	else 
	{
		vlc_setFmt(vlc_StyleBold, vlc_ColorYellow);
		printf("----->\tNo solution.\n");
	}

	vlc_resetFmt();
	printf("\n");
}

int main()
{
	int i;
	for(i = 0; i < 100; ++i) solveAndPrint(i);
}