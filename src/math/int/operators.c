
#include "libccc/fixed.h"
#include "libccc/math/fixed.h"

#include LIBCONFIG_ERROR_INCLUDE



#define DEFINEFUNC_UINT_ADD(BITS) \
inline t_u##BITS	U##BITS##_Add(t_u##BITS a, t_u##BITS b)	\
{															\
	HANDLE_ERROR(RESULTRANGE, (a > U##BITS##_MAX - b), LIBCONFIG_ERROR_HANDLEOVERFLOW)\
	return (a + b);											\
}



#define DEFINEFUNC_UINT_SUB(BITS) \
inline t_u##BITS	U##BITS##_Sub(t_u##BITS a, t_u##BITS b)	\
{															\
	HANDLE_ERROR(RESULTRANGE, (a < b), LIBCONFIG_ERROR_HANDLEOVERFLOW)\
	return (a - b);											\
}



#define DEFINEFUNC_UINT_MUL(BITS) \
inline t_u##BITS	U##BITS##_Mul(t_u##BITS a, t_u##BITS b)	\
{															\
/*	HANDLE_ERROR(RESULTRANGE, (a * b), LIBCONFIG_ERROR_HANDLEOVERFLOW)*/\
	return (a * b);											\
} // TODO fix this and test (implement CLZ functions, and check if leading bits go beyond range)



#define DEFINEFUNC_UINT_DIV(BITS) \
inline t_u##BITS	U##BITS##_Div(t_u##BITS a, t_u##BITS b)	\
{															\
	HANDLE_ERROR(MATHDOMAIN, (b == 0), return (0);)			\
	return (a / b);											\
} // TODO fix this and test



#define DEFINEFUNC_UINT_MOD(BITS) \
inline t_u##BITS	U##BITS##_Mod(t_u##BITS a, t_u##BITS b)	\
{															\
	HANDLE_ERROR(MATHDOMAIN, (b == 0), return (0);)			\
	return (a % b);											\
} // TODO fix this and test



DEFINEFUNC_UINT_ADD(8)
DEFINEFUNC_UINT_SUB(8)
DEFINEFUNC_UINT_MUL(8)
DEFINEFUNC_UINT_DIV(8)
DEFINEFUNC_UINT_MOD(8)

DEFINEFUNC_UINT_ADD(16)
DEFINEFUNC_UINT_SUB(16)
DEFINEFUNC_UINT_MUL(16)
DEFINEFUNC_UINT_DIV(16)
DEFINEFUNC_UINT_MOD(16)

DEFINEFUNC_UINT_ADD(32)
DEFINEFUNC_UINT_SUB(32)
DEFINEFUNC_UINT_MUL(32)
DEFINEFUNC_UINT_DIV(32)
DEFINEFUNC_UINT_MOD(32)

DEFINEFUNC_UINT_ADD(64)
DEFINEFUNC_UINT_SUB(64)
DEFINEFUNC_UINT_MUL(64)
DEFINEFUNC_UINT_DIV(64)
DEFINEFUNC_UINT_MOD(64)

#ifdef __int128
DEFINEFUNC_UINT_ADD(128)
DEFINEFUNC_UINT_SUB(128)
DEFINEFUNC_UINT_MUL(128)
DEFINEFUNC_UINT_DIV(128)
DEFINEFUNC_UINT_MOD(128)
#endif
