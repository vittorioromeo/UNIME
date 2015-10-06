// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <stdio.h>
#include <stdlib.h>

void printArray(int* mA, int mASize)
{
    int i;
    for(i = 0; i < mASize; ++i) printf("%d, ", mA[i]);
    printf("\n");
}

void swap(int* mA, int* mB)
{
    int temp = *mA;
    *mA = *mB;
    *mB = temp;
}

int partition(int* mA, int mLB, int mUB, int mPivot)
{
    while(mLB < mUB)
    {
        while(mA[mLB] < mPivot) ++mLB;
        while(mA[mUB] > mPivot) --mUB;

        if(mA[mLB] == mA[mUB])
            ++mLB;
        else if(mLB < mUB)
            swap(&mA[mLB], &mA[mUB]);
    }

    return mLB;
}

void quickSort(int* mA, int mLB, int mUB)
{
    if(mLB >= mUB) return;

    int pIdx = partition(mA, mLB, mUB, mA[mLB + (mUB - mLB) / 2]);

    quickSort(mA, mLB, pIdx - 1);
    quickSort(mA, pIdx + 1, mUB);
}

void insertionSort(int* mA, int mASize)
{
    int i, j;

    for(i = 1; i < mASize; ++i)
        for(j = i; j > 0 && mA[j - 1] > mA[j]; --j) swap(&mA[j], &mA[j - 1]);
}

void insertionSortByStep(int* mA, int mASize, int mStep)
{
    int i, j;

    for(i = 1; i < mASize; i += mStep)
        for(j = i; j > 0 && mA[j - mStep] > mA[j]; j -= mStep)
            swap(&mA[j], &mA[j - mStep]);
}

void shellSort(int* mA, int mLB, int mUB)
{
    int size = mUB - mLB + 1;
    int step = size;
    int i;

    while(step > 1)
    {
        step /= 2;
        insertionSortByStep(mA, size, step);
    }
}

int main()
{
    {
        int array[] = {4, 1, 6, 33, 5, 2, 19, 1, 6, 3, 9, 10, 34, 32, 11, 20};
        int arraySize = sizeof(array) / sizeof(array[0]);

        {
            int pIdx = partition(array, 0, arraySize - 1, 15);
            printArray(array, arraySize);
            printf("pIdx: %d\n", pIdx);
        }

        {
            quickSort(array, 0, arraySize - 1);
            printArray(array, arraySize);
        }
    }

    {
        int array[] = {4, 1, 6, 33, 5, 2, 19, 1, 6, 3, 9, 10, 34, 32, 11, 20};
        int arraySize = sizeof(array) / sizeof(array[0]);

        {
            shellSort(array, 0, arraySize - 1);
            printArray(array, arraySize);
        }
    }
}
