// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef VL_UTILS_CONSOLE
#define VL_UTILS_CONSOLE

#include "VeeLib/Global/Common.h"
#include "VeeLib/Utils/Utility.h"

#define VLC_STYLE_COUNT 13
#define VLC_COLOR_COUNT 16

enum Style_impl
{
    vlc_StyleNone = 0,
    vlc_StyleBold = 1,
    vlc_StyleDim = 2,
    vlc_StyleUnderline = 3,
    vlc_StyleBlink = 4,
    vlc_StyleReverseFGBG = 5,
    vlc_StyleHidden = 6,
    vlc_StyleResetBold = 7,
    vlc_StyleResetDim = 8,
    vlc_StyleResetUnderline = 9,
    vlc_StyleResetBlink = 10,
    vlc_StyleResetReverse = 11,
    vlc_StyleResetHidden = 12
};
enum Color_impl
{
    vlc_ColorDefault = 0,
    vlc_ColorBlack = 1,
    vlc_ColorRed = 2,
    vlc_ColorGreen = 3,
    vlc_ColorYellow = 4,
    vlc_ColorBlue = 5,
    vlc_ColorMagenta = 6,
    vlc_ColorCyan = 7,
    vlc_ColorLightGray = 8,
    vlc_ColorDarkGray = 9,
    vlc_ColorLightRed = 10,
    vlc_ColorLightGreen = 11,
    vlc_ColorLightYellow = 12,
    vlc_ColorLightBlue = 13,
    vlc_ColorLightMagenta = 14,
    vlc_ColorLightCyan = 15,
    vlc_ColorLightWhite = 16
};

typedef enum Style_impl Style;
typedef enum Color_impl Color;

#ifdef VL_OS_LINUX
#include "VeeLib/Utils/Console/UtilsConsoleImplUnix.h"
#elif VL_OS_WINDOWS
#include "VeeLib/Utils/Console/UtilsConsoleImplWin.h"
#else
#include "VeeLib/Utils/Console/UtilsConsoleImplNull.h"
#endif

inline const char* vlc_getResetFmtStr() { return vlc_impl_getResetFmtStr(); }
inline const char* vlc_getStyleStr(Style mStyle)
{
    return vlc_impl_getStyleStr(mStyle);
}
inline const char* vlc_getColorFGStr(Color mColor)
{
    return vlc_impl_getColorFGStr(mColor);
}
inline const char* vlc_getColorBGStr(Color mColor)
{
    return vlc_impl_getColorBGStr(mColor);
}

inline void vlc_resetFmt() { printf("%s", vlc_getResetFmtStr()); }
inline void vlc_setStyle(Style mStyle)
{
    printf("%s", vlc_getStyleStr(mStyle));
}
inline void vlc_setColorFG(Color mColor)
{
    printf("%s", vlc_getColorFGStr(mColor));
}
inline void vlc_setColorBG(Color mColor)
{
    printf("%s", vlc_getColorBGStr(mColor));
}

inline void vlc_setFmt(Style mStyle, Color mColor)
{
    vlc_resetFmt();
    vlc_setStyle(mStyle);
    vlc_setColorFG(mColor);
}

/// @brief Gets and returns an integer value using scanf.
inline int vlc_getScanfI()
{
    int result;
    scanf("%d", &result);
    return result;
}

/// @brief Gets and returns a char value using scanf.
inline char vlc_getScanfC()
{
    char result;
    scanf("%c", &result);
    return result;
}

/// @brief Gets and returns a float value using scanf.
inline float vlc_getScanfF()
{
    float result;
    scanf("%f", &result);
    return result;
}

/// @brief Gets and returns a double value using scanf.
inline double vlc_getScanfD()
{
    double result;
    scanf("%f", &result);
    return result;
}

/// @brief Portable-way to wait for the user to enter anything in the console.
inline void vlc_waitForAnyKey()
{
    vlc_getScanfC();
    vlc_getScanfC();
}

/// @brief Clears the console screen bt printing 25*8 newlines.
inline void vlc_clearScreen()
{
    int i;
    for(i = 0; i < 25; ++i) printf("\n\n\n\n\n\n\n\n\n\n\n");
}

/// @brief Displays a multi-choice menu that leads to the execution of a
/// user-chosen function.
/// @param mChoiceCount Number of possible choices.
/// @param mChoiceDescs Array containing labels for the menu choices.
/// @param mFnPtrs Array containing function pointers for the menu choices.
inline void vlc_showMenu(
    size_t mChoiceCount, const char** mChoiceDescs, void (*mFnPtrs[])())
{
    while(true)
    {
        vlc_clearScreen();

        vlc_setFmt(vlc_StyleBold, vlc_ColorCyan);
        printf("\nWelcome! Choose:");

        int i;
        for(i = 0; i < mChoiceCount; ++i)
        {
            vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
            printf("\n(%d)\t", i);

            vlc_setFmt(vlc_StyleUnderline, vlc_ColorYellow);
            printf("%s", mChoiceDescs[i]);
        }

        vlc_setFmt(vlc_StyleBold, vlc_ColorRed);
        printf("\n(%d)\t", i);

        vlc_setFmt(vlc_StyleUnderline, vlc_ColorYellow);
        printf("%s", "EXIT");

        vlc_resetFmt();

        int choice = vlc_getScanfI();

        if(choice >= mChoiceCount) break;

        vlc_clearScreen();
        mFnPtrs[choice]();
        vlc_waitForAnyKey();
    }

    vlc_clearScreen();
}

#endif
