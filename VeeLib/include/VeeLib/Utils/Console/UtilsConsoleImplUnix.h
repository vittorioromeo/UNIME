// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef VL_UTILS_CONSOLE_IMPL
#define VL_UTILS_CONSOLE_IMPL

#define VLC_FMT_PREFIX "\033["
#define VLC_FMT_POSTFIX "m"

inline int vlc_impl_getColorFGCode(size_t mIdx)
{
	static int codes[] =
	{
		39,		// Default
		30,		// Black
		31,		// Red
		32,		// Green
		33,		// Yellow
		34,		// Blue
		35,		// Magenta
		36,		// Cyan
		37,		// Light gray
		90,		// Dark gray
		91,		// Light red
		92,		// Light green
		93,		// Light yellow
		94,		// Light blue
		95,		// Light magenta
		96,		// Light cyan
		97		// Light white
	};

	return codes[mIdx];
}
inline int vlc_impl_getColorBGCode(size_t mIdx) { return vlc_impl_getColorFGCode(mIdx) + 10; }
inline int vlc_impl_getModCode(size_t mIdx)
{
	static int codes[] =
	{
		0,	// None
		1,	// Bold
		2,	// Dim
		4,	// Underline
		5,	// Blink
		7,	// Reverse
		8,	// Hidden
		21,	// Reset bold
		22,	// Reset dim
		24,	// Reset underline
		25,	// Reset blink
		27,	// Reset reverse
		28	// Reset hidden
	};

	return codes[mIdx];
}

inline Style* vlc_impl_getLastStyle() 
{
	static Style lastStyle = vlc_StyleNone;
	return &lastStyle;
}

inline Color* vlc_impl_getLastColorFG() 
{
	static Color lastColorFG = vlc_ColorDefault;
	return &lastColorFG;
}

inline Color* vlc_impl_getLastColorBG() 
{
	static Color lastColorBG = vlc_ColorDefault;
	return &lastColorBG;
}

inline const char* vlc_impl_getFmtString()
{
	static char buffer[255];	
	int modCode = vlc_impl_getModCode((int)(*vlc_impl_getLastStyle()));
	int colorFGCode = vlc_impl_getColorFGCode((int)(*vlc_impl_getLastColorFG()));
	int colorBGCode = vlc_impl_getColorBGCode((int)(*vlc_impl_getLastColorBG()));
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
inline const char* vlc_impl_getStyleStr(Style mStyle)		{ *vlc_impl_getLastStyle() = mStyle; return vlc_impl_getFmtString(); }
inline const char* vlc_impl_getColorFGStr(Color mColor) 	{ *vlc_impl_getLastColorFG() = mColor; return vlc_impl_getFmtString(); }
inline const char* vlc_impl_getColorBGStr(Color mColor) 	{ *vlc_impl_getLastColorBG() = mColor; return vlc_impl_getFmtString(); }

#endif