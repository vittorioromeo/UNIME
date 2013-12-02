// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef VL_UTILS_CONSOLE
#define VL_UTILS_CONSOLE

#include "VeeLib/Global/Common.h"

/// @brief Gets and returns an integer value using scanf.
inline int vlc_getScanfInt() { int result; scanf("%d", &result); return result; }

/// @brief Gets and returns a char value using scanf.
inline char vlc_getScanfChar() { char result; scanf("%c", &result); return result; }

/// @brief Gets and returns a float value using scanf.
inline float vlc_getScanfFloat() { float result; scanf("%f", &result); return result; }

/// @brief Gets and returns a double value using scanf.
inline double vlc_getScanfDouble() { double result; scanf("%f", &result); return result; }

/// @brief Portable-way to wait for the user to enter anything in the console.
inline void vlc_waitForAnyKey()	{ char temp; scanf("%c", &temp); scanf("%c", &temp); }

/// @brief Clears the console screen bt printing 50*4 newlines.
inline void vlc_clearScreen() { int i; for(i = 0; i < 25; ++i) printf("\n\n\n\n\n\n\n\n\n\n\n"); }

// TODO: docs
inline void vlc_showMenu(int mChoiceCount, const char** mChoiceDescs, void(*mFnPtrs[])())
{
	while(true)
	{
		vlc_clearScreen();
		printf("\nWelcome! Choose:");

		int i;
		for(i = 0; i < mChoiceCount; ++i) printf("\n(%d): %s", i, mChoiceDescs[i]);
		printf("\n(%d): %s", i, "Exit.");
		
		int choice = vlc_getScanfInt();

		if(choice >= mChoiceCount) break;

		vlc_clearScreen();
		mFnPtrs[choice]();
		vlc_waitForAnyKey();
	}

	vlc_clearScreen();
}

#endif