// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct { int a, b, c; } ABC;

ABC solvePenProblemFallback(int mN)
{
	ABC result;
	result.c = result.b = result.a = 0;

	if(mN <= 0) return result;

	while(mN >= 24) { mN -= 24; ++result.c; }
	if(mN <= 0) return result;

	while(mN >= 8) { mN -= 8; ++result.b; }
	if(mN <= 0) return result;	

	while(mN > 0) { mN -= 5; ++result.a; }
	return result;
}

ABC solvePenProblem(int mN)
{
	ABC result;
	result.c = result.b = result.a = 0;

	if(mN % 5 == 0) 	{ result.a = mN / 5; return result; }
	if(mN % 8 == 0) 	{ result.b = mN / 8; return result; }
	if(mN % 24 == 0) 	{ result.a = mN / 24; return result; }

	return solvePenProblemFallback(mN);
}

// WIP

int main()
{
	ABC result = solvePenProblem(58);
	printf("a: %d\tb: %d\t c: %d", result.a, result.b, result.c);

}