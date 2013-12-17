// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef VL_UTILS_CONSOLE_IMPL
#define VL_UTILS_CONSOLE_IMPL

#define VLC_FMT_PREFIX "\033["
#define VLC_FMT_POSTFIX "m"

inline int vlc_impl_getColorFGCode(Color mColor)
{
	static int codes[] =
	{
		39,		// 0 = Color::Default
		30,		// 1 = Color::Black
		31,		// 2 = Color::Red
		32,		// 3 = Color::Green
		33,		// 4 = Color::Yellow
		34,		// 5 = Color::Blue
		35,		// 6 = Color::Magenta
		36,		// 7 = Color::Cyan
		37,		// 8 = Color::LightGray
		90,		// 9 = Color::DarkGray
		91,		// 10 = Color::LightRed
		92,		// 11 = Color::LightGreen
		93,		// 12 = Color::LightYellow
		94,		// 13 = Color::LightBlue
		95,		// 14 = Color::LightMagenta
		96,		// 15 = Color::LightCyan
		97		// 16 = Color::LightWhite
	};

	return codes[(int)(mColor)];
}
inline int vlc_impl_getColorBGCode(Color mColor) { return vlc_impl_getColorFGCode(mColor) + 10; }
inline int vlc_impl_getStyleCode(Style mStyle)
{
	static int codes[] =
	{
		0,	// 0 = Style::None
		1,	// 1 = Style::Bold
		2,	// 2 = Style::Dim
		4,	// 3 = Style::Underline
		5,	// 4 = Style::Blink
		7,	// 5 = Style::Reverse
		8,	// 6 = Style::Hidden
		21,	// 7 = Style::ResetBold
		22,	// 8 = Style::ResetDim
		24,	// 9 = Style::ResetUnderline
		25,	// 10 = Style::ResetBlink
		27,	// 11 = Style::ResetReverse
		28	// 12 = Style::ResetHidden
	};

	return codes[(int)(mStyle)];
}

inline Style* vlc_impl_getLastStyle()	{ static Style lastStyle = vlc_StyleNone;		return &lastStyle; }
inline Color* vlc_impl_getLastColorFG()	{ static Color lastColorFG = vlc_ColorDefault;	return &lastColorFG; }
inline Color* vlc_impl_getLastColorBG()	{ static Color lastColorBG = vlc_ColorDefault;	return &lastColorBG; }

inline const char* vlc_impl_getFmtString()
{
	static char buffer[255];
	int modCode = vlc_impl_getStyleCode(*vlc_impl_getLastStyle());
	int colorFGCode = vlc_impl_getColorFGCode(*vlc_impl_getLastColorFG());
	int colorBGCode = vlc_impl_getColorBGCode(*vlc_impl_getLastColorBG());
	sprintf(buffer, "%s%d;%d;%d%s", VLC_FMT_PREFIX, modCode, colorFGCode, colorBGCode, VLC_FMT_POSTFIX);
	return buffer;
}

inline const char* vlc_impl_getResetFmtStr()
{
	*vlc_impl_getLastStyle() = vlc_StyleNone;
	*vlc_impl_getLastColorFG() = vlc_ColorDefault;
	*vlc_impl_getLastColorBG() = vlc_ColorDefault;
	return vlc_impl_getFmtString();
}
inline const char* vlc_impl_getStyleStr(Style mStyle)		{ *vlc_impl_getLastStyle() = mStyle;	return vlc_impl_getFmtString(); }
inline const char* vlc_impl_getColorFGStr(Color mColor) 	{ *vlc_impl_getLastColorFG() = mColor;	return vlc_impl_getFmtString(); }
inline const char* vlc_impl_getColorBGStr(Color mColor) 	{ *vlc_impl_getLastColorBG() = mColor;	return vlc_impl_getFmtString(); }

#endif
