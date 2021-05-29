
#ifndef __NOSTD__
	#include <stdlib.h>
#else
	float	atof(char const* str);
	double	atod(char const* str);
#endif

#include "libccc/float.h"
#include "libccc/char.h"
#include "libccc/memory.h"
#include "libccc/string.h"
#include "libccc/math/math.h"

#include LIBCONFIG_HANDLE_INCLUDE



//! The maximum amount of digits that can be printed to the mantissa
#define MAXLEN_MANTISSA	(64)



//! Returns 1 if the given 'number' is either NaN, or +/- infinity
static
t_float	Float_FromString_CheckSpecial(t_char const* str)
{
	char sign = str[0];
	if (sign == '-' || sign == '+')
		++str;
	if (String_Equals_N_IgnoreCase(str, "NAN", 3))
	{
		if (sign == '-')	return (-NAN);
		if (sign == '+')	return (+NAN);
		return (NAN);
	}
	if (String_Equals_N_IgnoreCase(str, "INFINITY", 8) ||
		String_Equals_N_IgnoreCase(str, "INF", 3) ||
		UTF32_FromUTF8((t_utf8*)str) == 0x221E) // infinity unicode char
	{
		if (sign == '-')	return (-INF);
		if (sign == '+')	return (+INF);
		return (INF);
	}
	return (0.);
}

//! Returns TRUE if the given 'str' contains any invalid characters for float parsing, or FALSE otherwise
static
t_bool	Float_FromString_CheckInvalid(t_char const* str)
{
	t_size	count_expon;
	t_size	count_signs;

	HANDLE_ERROR(NULLPOINTER, (str == NULL), return (TRUE);)
	if (str[0] == '\0')
		return (TRUE);
	if (str[0] != '+' &&
		str[0] != '-' &&
		str[0] != '.' &&
		!Char_IsDigit(str[0]))
		return (TRUE);
	if (String_HasOnly(str, "0123456789.+-eE"))
		count_expon = String_Count_Charset(str, "eE");
	else if (String_HasOnly(str, "0123456789aAbBcCdDeEfF.+-pPxX"))
		count_expon = String_Count_Charset(str, "pP");
	else if (String_HasOnly(str, "01.+-pPbB"))
		count_expon = String_Count_Charset(str, "pP");
	else return (TRUE);

	if (count_expon > 1)
		return (TRUE);
	count_signs = String_Count_Charset(str, "-+");
	if (count_expon)	{ if (count_signs > 2)	return (TRUE); }
	else				{ if (count_signs > 1)	return (TRUE); }
	return (FALSE);
}



#define DEFINEFUNC_STR_TO_FLOAT(BITS) \
t_f##BITS			F##BITS##_FromString(t_char const* str)			\
{																	\
	t_f##BITS	result;												\
																	\
	HANDLE_ERROR(NULLPOINTER, (str == NULL), return (0);)			\
	while (*str && Char_IsSpace(*str))								\
		++str;														\
	result = Float_FromString_CheckSpecial(str);					\
	if (result != 0.)												\
		return (result);											\
	if (Float_FromString_CheckInvalid(str))							\
		return (NAN);												\
	else if (String_Equals_N(str, "0b", 2))							\
		return (F##BITS##_FromString_Bin(str));						\
	else if (String_Equals_N(str, "0x", 2))							\
		return (F##BITS##_FromString_Hex(str));						\
	else															\
		return (atof(str));											\
/*\
	else if (String_Find_Charset(str, "eE") == NULL)				\
		return (F##BITS##_FromString_Dec(str));						\
	else															\
		return (F##BITS##_FromString_Exp(str));						\
*/\
}																	\



#define DEFINEFUNC_STREXP_TO_FLOAT(BITS) \
t_f##BITS	F##BITS##_FromString_Exp(t_char const* str)				\
{																	\
	t_f##BITS	result;												\
	t_char const* str_mantissa;										\
	t_char const* str_exponent;										\
	t_size		frac_digits;										\
	t_bool		negative;											\
	t_s16		exponent;											\
	t_char*		tmp;												\
	t_size	i = 0;													\
																	\
	HANDLE_ERROR(NULLPOINTER, (str == NULL), return (0);)			\
	while (*str && Char_IsSpace(*str))								\
		++str;														\
	result = Float_FromString_CheckSpecial(str);					\
	if (result != 0.)												\
		return (result);											\
	if (Float_FromString_CheckInvalid(str))							\
		return (NAN);												\
	if (!(str[i] == '+' || str[i] == '-' || Char_IsDigit(str[i])))	\
		return (NAN);												\
	negative = FALSE;												\
	if (str[i] == '-')												\
	{																\
		negative = TRUE;											\
		++i;														\
	}																\
	else if (str[i] == '+')											\
		++i;														\
	str_mantissa = str + i;											\
	str_exponent = String_Find_Charset(str, "eE");					\
	if (str_exponent)	++str_exponent;								\
	result = 0.;													\
	while (str_mantissa[i] && Char_IsDigit(str_mantissa[i]))		\
	{																\
		result = result * 10 + (str_mantissa[i++] - '0');			\
	}																\
	exponent = 0;													\
	if (str_exponent)												\
	{																\
		exponent = S16_FromString(str_exponent);					\
		if (exponent > F##BITS##_EXPONENT_BIAS)						\
			return (negative ? -INFINITY : INFINITY);				\
		else if (exponent < 1 - F##BITS##_EXPONENT_BIAS)			\
			return (0.);											\
	}																\
	tmp = String_Find_Char(str_mantissa, '.');						\
	if (tmp && (frac_digits = String_Length(++tmp)) > 0)			\
		exponent -= frac_digits;									\
	if (String_Length(str_mantissa) > MAXLEN_MANTISSA)				\
		exponent += String_Length(str_mantissa) - MAXLEN_MANTISSA;	\
	return (result * Float_Pow(10., exponent) * (negative ? -1 : 1));\
}																	\



#define DEFINEFUNC_STRDEC_TO_FLOAT(BITS) \
t_f##BITS	F##BITS##_FromString_Dec(t_char const* str)				\
{																	\
	t_f##BITS	result;												\
	t_bool	negative;												\
	t_size	i = 0;													\
																	\
	HANDLE_ERROR(NULLPOINTER, (str == NULL), return (0);)							\
	while (*str && Char_IsSpace(*str))								\
		++str;														\
	result = Float_FromString_CheckSpecial(str);					\
	if (result != 0.)												\
		return (result);											\
	if (Float_FromString_CheckInvalid(str))							\
		return (NAN);												\
	if (!(str[i] == '+' || str[i] == '-' || Char_IsDigit(str[i])))	\
		return (NAN);												\
	negative = FALSE;												\
	if (str[i] == '-')												\
	{																\
		negative = TRUE;											\
		++i;														\
	}																\
	else if (str[i] == '+')											\
		++i;														\
	result = 0.;													\
	while (str[i] && Char_IsDigit(str[i]))							\
	{																\
		result = result * 10 + (str[i++] - '0');					\
	}																\
	return (negative ? -result : result);							\
}																	\



#define DEFINEFUNC_STRHEX_TO_FLOAT(BITS) \
t_f##BITS	F##BITS##_FromString_Hex(t_char const* str)						\
{																			\
	t_f##BITS	result;														\
	t_char const* str_mantissa;												\
	t_char const* str_exponent;												\
	t_bool		negative;													\
	t_u##BITS	mantissa;													\
	t_s16		exponent;													\
	t_char*		tmp;														\
																			\
	HANDLE_ERROR(NULLPOINTER, (str == NULL), return (0);)									\
	while (*str && Char_IsSpace(*str))										\
		++str;																\
	result = Float_FromString_CheckSpecial(str);							\
	if (result != 0.)														\
		return (result);													\
	if (Float_FromString_CheckInvalid(str))									\
		return (NAN);														\
																			\
	HANDLE_ERROR(NULLPOINTER, (str == NULL), return (0);)									\
	negative = (str[0] == '-');												\
	str_mantissa = (negative || str[0] == '+') ? str + 1 : str;				\
	str_exponent = String_Find_Charset(str, "pP");							\
	if (str_exponent)	++str_exponent;										\
	result = (negative ? -1. : 1.);											\
	tmp = String_Remove(str_mantissa, ".");									\
	if (String_Equals(tmp, "0") || String_Equals(tmp, "00"))				\
	{																		\
		Memory_Free(tmp);													\
		return (0. * result);												\
	}																		\
	mantissa = U64_FromString_Hex(tmp);										\
	result *= (mantissa * F##BITS##_INIT_VALUE) *							\
		Float_Pow(2., (String_Length(tmp) - 1) * 4);						\
	exponent = (str_exponent ? S16_FromString(str_exponent) : 0);			\
	if (exponent > F##BITS##_EXPONENT_BIAS)									\
		return ((negative ? -1. : 1.) / 0.);								\
	else if (exponent < 1 - F##BITS##_EXPONENT_BIAS)						\
		return (0.);														\
	Memory_Copy(&mantissa, &result, sizeof(result));						\
	mantissa &= F##BITS##_MANTISSA_SIGNED;									\
	mantissa |= F##BITS##_EXPONENT & ((t_u##BITS)							\
		(exponent + F##BITS##_EXPONENT_BIAS) << F##BITS##_MANTISSA_BITS);	\
	Memory_Copy(&result, &mantissa, sizeof(result));						\
	Memory_Free(tmp);														\
	return (result);														\
}																			\



// TODO Float_ToString_Bin()
#define DEFINEFUNC_STRBIN_TO_FLOAT(BITS) \
t_f##BITS	F##BITS##_FromString_Bin(t_char const* str)						\
{ return (str == NULL ? NAN : 0.); }



DEFINEFUNC_STR_TO_FLOAT(   32)
DEFINEFUNC_STREXP_TO_FLOAT(32)
DEFINEFUNC_STRDEC_TO_FLOAT(32)
DEFINEFUNC_STRHEX_TO_FLOAT(32)
DEFINEFUNC_STRBIN_TO_FLOAT(32)

DEFINEFUNC_STR_TO_FLOAT(   64)
DEFINEFUNC_STREXP_TO_FLOAT(64)
DEFINEFUNC_STRDEC_TO_FLOAT(64)
DEFINEFUNC_STRHEX_TO_FLOAT(64)
DEFINEFUNC_STRBIN_TO_FLOAT(64)

#ifdef __float80
DEFINEFUNC_STR_TO_FLOAT(   80)
DEFINEFUNC_STREXP_TO_FLOAT(80)
DEFINEFUNC_STRDEC_TO_FLOAT(80)
DEFINEFUNC_STRHEX_TO_FLOAT(80)
DEFINEFUNC_STRBIN_TO_FLOAT(80)
#endif

#ifdef __float128
DEFINEFUNC_STR_TO_FLOAT(   128)
DEFINEFUNC_STREXP_TO_FLOAT(128)
DEFINEFUNC_STRDEC_TO_FLOAT(128)
DEFINEFUNC_STRHEX_TO_FLOAT(128)
DEFINEFUNC_STRBIN_TO_FLOAT(128)
#endif
