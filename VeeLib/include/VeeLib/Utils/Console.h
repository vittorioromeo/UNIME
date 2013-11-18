// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef VL_UTILS_CONSOLE
#define VL_UTILS_CONSOLE

#include "VeeLib/Global/Common.h"

inline int vlc_getScanfInt() 		{ int result; 		scanf("%d", &result); return result; }
inline char vlc_getScanfChar() 		{ char result; 		scanf("%c", &result); return result; }
inline float vlc_getScanfFloat() 	{ float result; 	scanf("%f", &result); return result; }
inline double vlc_getScanfDouble() 	{ double result; 	scanf("%f", &result); return result; }

inline void vlc_waitForAnyKey()	{ char temp; scanf("%c", &temp); scanf("%c", &temp); }
inline void vlc_clearScreen() 	{ int i; for(i = 0; i < 50; ++i) printf("\n"); }

inline void vlc_showMenu(int mChoiceCount, const char** mChoiceDescs, void(*mFnPtrs[])())
{
	while(1)
	{
		vlc_clearScreen();
		printf("\nWelcome! Choose:");

		int i;
		for(i = 0; i < mChoiceCount; ++i) printf("\n(%d): %s", i, mChoiceDescs[i]);
		printf("\n(%d): %s", i, "Exit.");
		
		int choice;
		scanf("%d", &choice);

		if(choice >= mChoiceCount) break;

		vlc_clearScreen();
		mFnPtrs[choice]();
		vlc_waitForAnyKey();
	}

	vlc_clearScreen();
}

#endif