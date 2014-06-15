// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <stdio.h>
#include <stdlib.h>

/// @brief Returns the absolute value of `mA`
double myAbs(double mA) { return mA >= 0 ? mA : -mA; }

/// @brief Returns an approximate square root of `mA`.
double mySqrtAlg1(double mA, double mEpsilon, double mStep, int* mItrCount)
{
	double guess = 0.0;
	*mItrCount = 0;

	while(myAbs(mA - guess * guess) > mEpsilon && guess <= mA) 
	{
		guess += mStep;
		++(*mItrCount);
	}

	return guess;
}


/// @brief Returns an approximate cubic root of `mA`.
double myCubicRootAlg1(double mA, double mEpsilon, double mStep, int* mItrCount)
{
	double guess = 0.0;
	*mItrCount = 0;

	while(myAbs(mA - guess * guess * guess) > mEpsilon && guess <= mA) 
	{
		guess += mStep;
		++(*mItrCount);
	}

	return guess;
}


int main()
{
	int tempItr;
	double step1 = 0.01;
	double ep1 = 0.1;

	printf("Sqrt of 3: %f\n", mySqrtAlg1(3, ep1, step1, &tempItr));
	printf("Iterations: %d\n\n", tempItr);

	printf("Sqrt of 4: %f\n", mySqrtAlg1(4, ep1, step1, &tempItr));
	printf("Iterations: %d\n\n", tempItr);

	printf("Sqrt of 16: %f\n", mySqrtAlg1(16, ep1, step1, &tempItr));
	printf("Iterations: %d\n\n", tempItr);

	printf("Sqrt of 36: %f\n", mySqrtAlg1(36, ep1, step1, &tempItr));
	printf("Iterations: %d\n\n", tempItr);

	printf("Sqrt of 2.71 * 10^6: %f\n", mySqrtAlg1(2.71 * 1000000, ep1, step1, &tempItr));
	printf("Iterations: %d\n\n", tempItr);

	printf("Sqrt of 34: %f\n", mySqrtAlg1(34, ep1, step1, &tempItr));
	printf("Iterations: %d\n\n", tempItr);

	{
		double num = 312;
		double ep = 0.1;
		printf("Sqrt of %f with epsilon %f: %f\n", num, ep, mySqrtAlg1(num, ep, step1, &tempItr));
		printf("Iterations: %d\n\n", tempItr);
	}
	{
		double num = 312;
		double ep = 0.01;
		printf("Sqrt of %f with epsilon %f: %f\n", num, ep, mySqrtAlg1(num, ep, step1, &tempItr));
		printf("Iterations: %d\n\n", tempItr);
	}
	{
		double num = 312;
		double ep = 0.001;
		printf("Sqrt of %f with epsilon %f: %f\n", num, ep, mySqrtAlg1(num, ep, step1, &tempItr));
		printf("Iterations: %d\n\n", tempItr);
	}
	{
		double num = 312;
		double step = 0.001;
		printf("Sqrt of %f with step %f: %f\n", num, step, mySqrtAlg1(num, ep1, step, &tempItr));
		printf("Iterations: %d\n\n", tempItr);
	}
	{
		double num = 312;
		double step = 0.01;
		printf("Sqrt of %f with step %f: %f\n", num, step, mySqrtAlg1(num, ep1, step, &tempItr));
		printf("Iterations: %d\n\n", tempItr);
	}
	{
		double num = 312;
		double step = 0.1;
		printf("Sqrt of %f with step %f: %f\n", num, step, mySqrtAlg1(num, ep1, step, &tempItr));
		printf("Iterations: %d\n\n", tempItr);
	}
	{
		double num = 312;
		double step = 0.5;
		printf("Sqrt of %f with step %f: %f\n", num, step, mySqrtAlg1(num, ep1, step, &tempItr));
		printf("Iterations: %d\n\n", tempItr);
	}

	printf("Cubic root of 8: %f\n", myCubicRootAlg1(8, ep1, step1, &tempItr));
	printf("Iterations: %d\n\n", tempItr);

	printf("Cubic root of 32: %f\n", myCubicRootAlg1(32, ep1, 0.00001, &tempItr));
	printf("Iterations: %d\n\n", tempItr);

	printf("Cubic root of 128: %f\n", myCubicRootAlg1(128, ep1, step1, &tempItr));
	printf("Iterations: %d\n\n", tempItr);

	printf("Cubic root of 756: %f\n", myCubicRootAlg1(756, ep1, step1, &tempItr));
	printf("Iterations: %d\n\n", tempItr);

	return 0;
}	