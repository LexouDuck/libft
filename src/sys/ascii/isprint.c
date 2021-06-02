
#ifndef __NOSTD__
	#include <limits.h>
#else
	#define UCHAR_MAX	0xFF
	#define SCHAR_MAX	0x7F
	#define SCHAR_MIN	-0x80
#endif

#include "libccc/bool.h"
#include "libccc/char.h"

#if LIBCONFIG_USE_STD_FUNCTIONS_ALWAYS
#include <ctype.h>
#endif

#include LIBCONFIG_HANDLE_INCLUDE



#if LIBCONFIG_USE_STD_FUNCTIONS_ALWAYS
inline
t_bool	ASCII_IsSpace(t_ascii c)
{ return (isspace(c)); }
#else
inline
t_bool	ASCII_IsSpace(t_ascii c)
{
	return ((c == ' ') || ('\t' <= c && c <= '\r'));
}
#endif



#if LIBCONFIG_USE_STD_FUNCTIONS_ALWAYS
inline
t_bool	ASCII_IsPunctuation(t_ascii c)
{ return (ispunct(c)); }
#else
inline
t_bool	ASCII_IsPunctuation(t_ascii c)
{
	return (
		('!' <= c && c <= '/') ||
		(':' <= c && c <= '@') ||
		('[' <= c && c <= '`') ||
		('{' <= c && c <= '~'));
}
#endif



#if LIBCONFIG_USE_STD_FUNCTIONS_ALWAYS
inline
t_bool	ASCII_IsPrintable(t_ascii c)
{ return (isprint(c)); }
#else
inline
t_bool	ASCII_IsPrintable(t_ascii c)
{
	return (!(c == 0x7F)	// DEL character
#if ((t_ascii)-1 == UCHAR_MAX)
		&& !(c < 0x20)				// C0 control code char
		&& !(0x80 <= c && c < 0xA0)	// C1 control code char
#elif (SCHAR_MAX >= 0x80)
		&& !(0 <= c && c < 0x20)	// C0 control code char
		&& !(0x80 <= c && c < 0xA0)	// C1 control code char
#else
		&& !(0 <= c && c < 0x20)	// C0 control code char
#endif
		&& ASCII_IsValid(c));
}
#endif



inline
t_bool	ASCII_IsValid(t_ascii c)
{
#if ((t_ascii)-1 == UCHAR_MAX)
	return (c < 0x80);
#elif (SCHAR_MAX >= 0x80)
	return (0 <= c && c < 0x80);
#else
	return (0 <= c);
#endif
}
