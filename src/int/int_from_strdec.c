
#include "libccc/bool.h"
#include "libccc/int.h"
#include "libccc/char.h"
#include "libccc/pointer.h"
#include "libccc/string.h"

#include LIBCONFIG_ERROR_INCLUDE



#define DEFINEFUNC_UINT_FROMSTRDEC(BITS) \
t_u##BITS	U##BITS##_FromString_Dec(t_char const* str)								\
{																					\
	t_u##BITS	result;																\
	t_u##BITS	tmp;																\
	t_size	i = 0;																	\
																					\
	HANDLE_ERROR(NULLPOINTER, (str == NULL), return (0);)							\
	while (*str && Char_IsSpace(*str))												\
	{																				\
		++str;																		\
	}																				\
	HANDLE_ERROR_SF(PARSE, !(*str == '+' || Char_IsDigit(*str)),					\
		return (0);,																\
		": expected a number (with spaces/sign), but instead got \"%s\"", str)		\
	if (str[i] == '+')																\
		++i;																		\
	result = 0;																		\
	while (str[i] && Char_IsDigit(str[i]))											\
	{																				\
		tmp = result * 10 + (str[i++] - '0');										\
		HANDLE_ERROR_SF(RESULTRANGE, (tmp < result),								\
			LIBCONFIG_ERROR_HANDLEOVERFLOW(U##BITS##_MAX),							\
			" (integer overflow for \"%s\" at "SF_U##BITS")", str, U##BITS##_MAX)	\
		result = tmp;																\
	}																				\
	return (result);																\
}																					\

DEFINEFUNC_UINT_FROMSTRDEC(8)
DEFINEFUNC_UINT_FROMSTRDEC(16)
DEFINEFUNC_UINT_FROMSTRDEC(32)
DEFINEFUNC_UINT_FROMSTRDEC(64)
#ifdef __int128
DEFINEFUNC_UINT_FROMSTRDEC(128)
#endif



#define DEFINEFUNC_SINT_FROMSTRDEC(BITS) \
t_s##BITS	S##BITS##_FromString_Dec(t_char const* str)								\
{																					\
	t_u##BITS	result;																\
	t_u##BITS	tmp;																\
	t_bool	negative;																\
	t_size	i = 0;																	\
																					\
	HANDLE_ERROR(NULLPOINTER, (str == NULL), return (0);)							\
	while (*str && Char_IsSpace(*str))												\
	{																				\
		++str;																		\
	}																				\
	HANDLE_ERROR_SF(PARSE, !(*str == '+' || *str == '-' || Char_IsDigit(*str)),		\
		return (0);,																\
		": expected a number (with spaces/sign), but instead got \"%s\"", str)		\
	negative = FALSE;																\
	if (str[i] == '-')																\
	{																				\
		negative = TRUE;															\
		++i;																		\
	}																				\
	else if (str[i] == '+')															\
		++i;																		\
	result = 0;																		\
	while (str[i] && Char_IsDigit(str[i]))											\
	{																				\
		tmp = result * 10 + (str[i++] - '0');										\
		HANDLE_ERROR_SF(RESULTRANGE, (negative && tmp > (t_u##BITS)-S##BITS##_MIN),	\
			LIBCONFIG_ERROR_HANDLEOVERFLOW(S##BITS##_MIN),							\
			" (integer underflow for \"%s\" at "SF_S##BITS")", str, S##BITS##_MIN)	\
		HANDLE_ERROR_SF(RESULTRANGE, (!negative && tmp > (t_u##BITS)S##BITS##_MAX),	\
			LIBCONFIG_ERROR_HANDLEOVERFLOW(S##BITS##_MAX),							\
			" (integer overflow for \"%s\" at "SF_S##BITS")", str, S##BITS##_MAX)	\
		result = tmp;																\
	}																				\
	return (negative ? -(t_s##BITS)result : (t_s##BITS)result);						\
}																					\

DEFINEFUNC_SINT_FROMSTRDEC(8)
DEFINEFUNC_SINT_FROMSTRDEC(16)
DEFINEFUNC_SINT_FROMSTRDEC(32)
DEFINEFUNC_SINT_FROMSTRDEC(64)
#ifdef __int128
DEFINEFUNC_SINT_FROMSTRDEC(128)
#endif