// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

ABC createABC()
{
	ABC result; 
	result.a = result.b = result.c = result.valid = 0;
	return result;
}

void solvePenProblemFallback(ABC* mR, int mN)
{
	int ubA = mN / 5;
	int ubB = mN / 8;
	int ubC = mN / 24;

	for(mR->a = 0; mR->a < ubA; ++mR->a)
		for(mR->b = 0; mR->b < ubB; ++mR->b)
			for(mR->c = 0; mR->c < ubC; ++mR->c)							
				if(5 * mR->a + 8 * mR->b + 24 * mR->c == mN) return;
								
	mR->valid = 1;
}

ABC solvePenProblem(int mN)
{
	ABC result = createABC();	

	if(mN % 5 == 0) 	{ result.a = mN / 5; return result; }
	if(mN % 8 == 0) 	{ result.b = mN / 8; return result; }
	if(mN % 24 == 0) 	{ result.c = mN / 24; return result; }

	solvePenProblemFallback(&result, mN);
	return result;
}

void solveAndPrint(int mN)
{
	ABC result = solvePenProblem(mN);
	printf("Solve:\t5 * a + 8 * b + 24 * c = %d\n", mN); 

	if(result.valid == 0) 
	{
		printf("----->\t5 * %d + 8 * %d + 24 * %d = %d\n", result.a, result.b, result.c, mN); 
		printf("----->\t%d + %d + %d = %d\n", 5 * result.a, 8 * result.b, 24 * result.c, mN); 
		printf("----->\t%d = %d\n", 5 * result.a + 8 * result.b + 24 * result.c, mN);
	}
	else printf("----->\tNo solution.\n");

	printf("\n");
}

// WIP
// "ax+by+cz=d has a solution if gcd(a, b, c) divides d"

int main()
{
	int i;
	for(i = 0; i < 100; ++i) solveAndPrint(i);
}