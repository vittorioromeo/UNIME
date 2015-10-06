// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <VeeLib/VeeLib.h>

/// @brief Structure used as a return type for debt functions.
typedef struct
{
    double paidRate;
    double remainingDebt;
} DebtResult;

// Implementation functions to help calculating debt
double calculateVariableMonthlyRate(double mDebt, double mRate)
{
    return mDebt * mRate;
}
double calculateFixedRate(double mDebt, double mRate) { return mRate; }

/// @brief Calculates paid rate and remaining debt.
/// @param mDebt Current debt.
/// @param mRate Rate value.
/// @param mAnnualInterest Annual interest value.
/// @param mRateFn Function pointer to rate calculation function.
DebtResult getRemainingDebt(double mDebt, double mRate, double mAnnualInterest,
    double (*mRateFn)(double, double))
{
    DebtResult result;
    result.paidRate = (*mRateFn)(mDebt, mRate);

    double newDebt = mDebt - result.paidRate;
    double monthlyInterest = mAnnualInterest / 12.0;

    result.remainingDebt = newDebt + (newDebt * monthlyInterest);

    return result;
}

/// @brief Returns the optimal fixed rate to pay a debt in `mExpectedMonths`.
double calculateRequiredFixedRate(
    double mDebt, double mAnnualInterest, int mExpectedMonths)
{
    assert(mExpectedMonths > 0);

    double monthlyInterest = mAnnualInterest / 12.0;

    // I look for two bound values that the optimal rate will never exceed.
    // The lower bound is calculated by assuming zero interest.
    // The upper bound is calculated by assuming full interest on the starting
    // debt value.
    double lb = mDebt / mExpectedMonths;
    double ub = (mDebt + (mDebt * (monthlyInterest * mExpectedMonths))) /
                mExpectedMonths;

    int m;
    DebtResult currentResult;

    // I increase the rate in steps of 10.
    for(; lb < ub; lb += 10.0)
    {
        // I check how many months it will take to pay off the loan with the
        // current rate.
        m = 0;
        currentResult.remainingDebt = mDebt;

        do
        {
            currentResult = getRemainingDebt(currentResult.remainingDebt, lb,
                mAnnualInterest, &calculateFixedRate);
            ++m;

            // If we're still in this loop, it means we haven't exceeded the
            // expected months.
            // Therefore, if the debt was paid off, this is the optimal rate to
            // use.
            if(currentResult.remainingDebt <= 0.0) return lb;
        } while(m < mExpectedMonths && currentResult.remainingDebt > 0.0);
    }

    // The complexity of this algorithm greatly depends on the passed values.
    // As it is composed of two nested loops, it can probably degrade to O(N^2).
    // If `lb` and `ub` do not differ much, the algorithm will run less
    // iterations.
    // If `lb` and `ub` differ a lot, the algorithm will run many more
    // iterations.
    // The first loop will be ran `((ub - lb) / 10)` times.

    return lb;
}

// Example methods that print the steps required to pay a loan
void printDebtForAnYear(
    double mDebt, double mAnnualInterest, double mVariableMonthlyRate)
{
    int m;

    vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
    printf("Printing debts for an year:\n");
    vlc_resetFmt();

    printf("\tAnnual interest -> %f\n", mAnnualInterest);
    printf("\tMonthly rate    -> %f\n\n", mVariableMonthlyRate);

    for(m = 0; m < 12; ++m)
    {
        DebtResult currentResult = getRemainingDebt(mDebt, mVariableMonthlyRate,
            mAnnualInterest, &calculateVariableMonthlyRate);
        mDebt = currentResult.remainingDebt;

        printf("\tPaid: %f\tRemaining: %f\n", currentResult.paidRate, mDebt);
    }
}
void printDebtExpectedMonthsFixedRate(double mDebt, double mAnnualInterest,
    double mFixedRate, int mExpectedMonths)
{
    int m = 0;

    vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
    printf("Printing debts with fixed rates - can it be done in %d months?\n",
        mExpectedMonths);
    vlc_resetFmt();

    printf("\tAnnual interest -> %f\n", mAnnualInterest);
    printf("\tFixed rate      -> %f\n\n", mFixedRate);

    DebtResult currentResult;

    do
    {
        currentResult = getRemainingDebt(
            mDebt, mFixedRate, mAnnualInterest, &calculateFixedRate);
        mDebt = currentResult.remainingDebt;

        printf("\tPaid: %f\tRemaining: %f\n", currentResult.paidRate, mDebt);

        ++m;
    } while(currentResult.remainingDebt > 0.0);

    printf("Possible in %d months? %s\n", mExpectedMonths,
        (m > mExpectedMonths) ? "No" : "Yes");
    printf("Required months: %d\n", m);
}

int main()
{
    printDebtForAnYear(4213, 0.2, 0.04);
    printf("\n");
    printDebtExpectedMonthsFixedRate(4213, 0.2, 872, 5);
    printf("\n");
    printf("Required fixed rate test: %f\n",
        calculateRequiredFixedRate(4213, 0.2, 5));
}