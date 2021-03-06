
#include "libccc/int.h"
#include "libccc/memory.h"

#include LIBCONFIG_ERROR_INCLUDE



#define MAXDIGITS_8BIT	 ( 3)	//!< The amount of digits needed to represent an 8-bit integer in decimal (max: 255)
#define MAXDIGITS_16BIT	 ( 5)	//!< The amount of digits needed to represent a 16-bit integer in decimal (max: 65535)
#define MAXDIGITS_32BIT	 (10)	//!< The amount of digits needed to represent a 32-bit integer in decimal (max: 4294967295)
#define MAXDIGITS_64BIT	 (20)	//!< The amount of digits needed to represent a 64-bit integer in decimal (max: 18446744073709551615)
#define MAXDIGITS_128BIT (41)	//!< The amount of digits needed to represent a 128-bit integer in decimal (max: 340282366920938463463374607431768211455)



#define DEFINEFUNC_UINT_TOSTR(BITS) \
t_char*	U##BITS##_ToString(t_u##BITS number)		\
{													\
	t_char*	result;									\
	t_u8	digits[MAXDIGITS_##BITS##BIT];			\
	t_u8	i;										\
	t_u##BITS	n;									\
													\
	n = number;										\
	i = 0;											\
	while (n > 0)									\
	{												\
		digits[i++] = n % 10;						\
		n /= 10;									\
	}												\
	if (i == 0)										\
		digits[i++] = 0;							\
	result = (t_char*)Memory_Allocate(i + 1);		\
	HANDLE_ERROR(ALLOCFAILURE, (result == NULL),	\
		return (NULL);)								\
	n = 0;											\
	while (i--)										\
	{												\
		result[n++] = '0' + digits[i];				\
	}												\
	result[n] = '\0';								\
	return (result);								\
}													\

DEFINEFUNC_UINT_TOSTR(8)
DEFINEFUNC_UINT_TOSTR(16)
DEFINEFUNC_UINT_TOSTR(32)
DEFINEFUNC_UINT_TOSTR(64)
#ifdef __int128
DEFINEFUNC_UINT_TOSTR(128)
#endif



#define DEFINEFUNC_SINT_TOSTR(BITS) \
t_char*	S##BITS##_ToString(t_s##BITS number)		\
{													\
	t_char*	result;									\
	t_u8	digits[MAXDIGITS_##BITS##BIT];			\
	t_u8	i;										\
	t_u##BITS	n;									\
													\
	if (number < 0)									\
		n = -number;								\
	else n = number;								\
	i = 0;											\
	while (n > 0)									\
	{												\
		digits[i++] = n % 10;						\
		n /= 10;									\
	}												\
	result = (t_char*)Memory_Allocate(i + 2);		\
	HANDLE_ERROR(ALLOCFAILURE, (result == NULL),	\
		return (NULL);)								\
	result[0] = (number == 0) ? '0' : '-';			\
	n = (number <= 0) ? 1 : 0;						\
	while (i--)										\
	{												\
		result[n++] = '0' + digits[i];				\
	}												\
	result[n] = '\0';								\
	return (result);								\
}													\

DEFINEFUNC_SINT_TOSTR(8)
DEFINEFUNC_SINT_TOSTR(16)
DEFINEFUNC_SINT_TOSTR(32)
DEFINEFUNC_SINT_TOSTR(64)
#ifdef __int128
DEFINEFUNC_SINT_TOSTR(128)
#endif
