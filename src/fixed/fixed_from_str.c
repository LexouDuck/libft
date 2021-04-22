
#include "libccc/bool.h"
#include "libccc/char.h"
#include "libccc/int.h"
#include "libccc/pointer.h"
#include "libccc/fixed.h"



#if LIBCONFIG_HANDLE_NULLPOINTERS
	#define LIBCONFIG_HANDLE_NULLPOINTER_STR_TO_INT	if (str == NULL) return (0);
#else
	#define LIBCONFIG_HANDLE_NULLPOINTER_STR_TO_INT	
#endif



#define DEFINEFUNC_CONVERT_STR_TO_FIXED(BITS) \
t_q##BITS	Q##BITS##_FromString(char const* str)						\
{																		\
	t_u##BITS	result;													\
	t_bool	negative;													\
	t_size	i;															\
LIBCONFIG_HANDLE_NULLPOINTER_STR_TO_INT									\
	for (i = 0; str[i] && Char_IsSpace(str[i]); ++i)					\
		continue;														\
	if (!(str[i] == '+' || str[i] == '-' || Char_IsDigit(str[i])))		\
		return (0);														\
	negative = FALSE;													\
	if (str[i] == '-')													\
	{																	\
		negative = TRUE;												\
		++i;															\
	}																	\
	else if (str[i] == '+')												\
		++i;															\
	result = 0;															\
	while (str[i] && Char_IsDigit(str[i]))								\
	{																	\
		result = result * 10 + (str[i++] - '0');						\
	}																	\
	return (negative ? -(t_q##BITS)result : (t_q##BITS)result);			\
}																		\

DEFINEFUNC_CONVERT_STR_TO_FIXED(16)
DEFINEFUNC_CONVERT_STR_TO_FIXED(32)
DEFINEFUNC_CONVERT_STR_TO_FIXED(64)
#ifdef __int128
DEFINEFUNC_CONVERT_STR_TO_FIXED(128)
#endif
