/*============================================================================*/
/*                                            ______________________________  */
/*  libccc/math/math.h                       |    __    __  ___      _____  | */
/*                                           |   / /\  / /\/ . |\   /  __|\ | */
/*  https://github.com/LexouDuck/libccc.git  |  / /_/ / / / . <_/  |  /___| | */
/*                                           | /___/\/_/ /___-'\   \____/\  | */
/* Comprehensive|Cross-platform|Customizable | \____/\__/\____-'    \____/  | */
/* This source code follows the MIT License. |______________________________| */
/*                                                                            */
/*============================================================================*/

#ifndef __LIBCCC_MATH_H
#define __LIBCCC_MATH_H
/*!@group{libccc_math_math}
** @{
**	This header defines the common standard math functions and macro defines.
**
**	@isostd{https://en.cppreference.com/w/c/numeric/math}
**
**	@file
*/

/*
** ************************************************************************** *|
**                                   Includes                                 *|
** ************************************************************************** *|
*/

#include "libccc.h"

#include "libccc/math/int.h"
#include "libccc/math/fixed.h"
#include "libccc/math/float.h"

HEADER_CPP

/*
** ************************************************************************** *|
**                                 Definitions                                *|
** ************************************************************************** *|
*/

//! Expands to the absolute value of `x` (ie: gives the positive equivalent of `x`)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/abs}
**	@isostd{https://en.cppreference.com/w/c/numeric/math/fabs}
**
**	- Math: @f$ {|x|} @f$
*/
#ifndef ABS
#define ABS(x)		((x) < 0 ? -(x) : (x))
#endif

//! Expands to +1 or -1 according to the sign of `x` (NB: if `x` is zero, gives +1)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/signbit} (differs)
**
**	- Math: @f$ {\frac{x}{|x|}} @f$
*/
#ifndef SIGN
#define SIGN(x)		((x) < 0 ? -1 : +1)
#endif



//!	Expands to the minimum value between `x` and `y` (the smaller of the two)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/fmin}
*/
#ifndef MIN
#define MIN(x, y)	((x) < (y) ? (x) : (y))
#endif

//! Expands to the maximum value between `x` and `y` (the larger of the two)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/fmax}
*/
#ifndef MAX
#define MAX(x, y)	((x) < (y) ? (y) : (x))
#endif



#if (LIBCONFIG_USE_FAST_APPROX_MATH == 0)

	#if LIBCONFIG_BITS_FLOAT == 32
		#define MATH_DECL_FUNCTION(RETURN, FT_NAME, NAME)	inline RETURN c_##FT_NAME(t_float x)				{ return (__builtin_##NAME##f(x)); }
		#define MATH_DECL_REALFUNCTION(FT_NAME, NAME)		inline t_float c_##FT_NAME(t_float x)				{ return (__builtin_##NAME##f(x)); }
		#define MATH_DECL_REALOPERATOR(FT_NAME, NAME)		inline t_float c_##FT_NAME(t_float x, t_float y)	{ return (__builtin_##NAME##f(x, y)); }
	#endif

	#if LIBCONFIG_BITS_FLOAT == 64
		#define MATH_DECL_FUNCTION(RETURN, FT_NAME, NAME)	inline RETURN c_##FT_NAME(t_float x)				{ return (__builtin_##NAME(x)); }
		#define MATH_DECL_REALFUNCTION(FT_NAME, NAME)		inline t_float c_##FT_NAME(t_float x)				{ return (__builtin_##NAME(x)); }
		#define MATH_DECL_REALOPERATOR(FT_NAME, NAME)		inline t_float c_##FT_NAME(t_float x, t_float y)	{ return (__builtin_##NAME(x, y)); }
	#endif

	#if LIBCONFIG_BITS_FLOAT == 80
		#define MATH_DECL_FUNCTION(RETURN, FT_NAME, NAME)	inline RETURN c_##FT_NAME(t_float x)				{ return (__builtin_##NAME##l(x)); }
		#define MATH_DECL_REALFUNCTION(FT_NAME, NAME)		inline t_float c_##FT_NAME(t_float x)				{ return (__builtin_##NAME##l(x)); }
		#define MATH_DECL_REALOPERATOR(FT_NAME, NAME)		inline t_float c_##FT_NAME(t_float x, t_float y)	{ return (__builtin_##NAME##l(x, y)); }
	#endif

	#if LIBCONFIG_BITS_FLOAT == 128
		#define MATH_DECL_FUNCTION(RETURN, FT_NAME, NAME)	inline RETURN c_##FT_NAME(t_float x)				{ return (__builtin_##NAME##q(x)); }
		#define MATH_DECL_REALFUNCTION(FT_NAME, NAME)		inline t_float c_##FT_NAME(t_float x)				{ return (__builtin_##NAME##q(x)); }
		#define MATH_DECL_REALOPERATOR(FT_NAME, NAME)		inline t_float c_##FT_NAME(t_float x, t_float y)	{ return (__builtin_##NAME##q(x, y)); }
	#endif

#endif



#define DEFINE_GENERIC_MATH_FUNCTION(FUNCTION, X) \
	_Generic((X),	\
		t_u8:	U8##FUNCTION,	\
		t_u16:	U16##FUNCTION,	\
		t_u32:	U32##FUNCTION,	\
		t_u64:	U64##FUNCTION,	\
		t_u128:	U128##FUNCTION,	\
		t_uint:	UInt##FUNCTION,	\
		t_s8:	S8##FUNCTION,	\
		t_s16:	S16##FUNCTION,	\
		t_s32:	S32##FUNCTION,	\
		t_s64:	S64##FUNCTION,	\
		t_s128:	S128##FUNCTION,	\
		t_sint:	SInt##FUNCTION,	\
		t_q16:	Q16##FUNCTION,	\
		t_q32:	Q32##FUNCTION,	\
		t_q64:	Q64##FUNCTION,	\
		t_q128:	Q128##FUNCTION,	\
		t_fixed:Fixed##FUNCTION,\
		t_f32:	F32##FUNCTION,	\
		t_f64:	F64##FUNCTION,	\
		t_f80:	F80##FUNCTION,	\
		t_f128:	F128##FUNCTION,	\
		t_float:Float##FUNCTION,\
	)(X)

#define DEFINE_GENERIC_MATH_OPERATOR(FUNCTION, X, Y) \
	_Generic((X),	\
		t_u8:	U8##FUNCTION,	\
		t_u16:	U16##FUNCTION,	\
		t_u32:	U32##FUNCTION,	\
		t_u64:	U64##FUNCTION,	\
		t_u128:	U128##FUNCTION,	\
		t_uint:	UInt##FUNCTION,	\
		t_s8:	S8##FUNCTION,	\
		t_s16:	S16##FUNCTION,	\
		t_s32:	S32##FUNCTION,	\
		t_s64:	S64##FUNCTION,	\
		t_s128:	S128##FUNCTION,	\
		t_sint:	SInt##FUNCTION,	\
		t_q16:	Q16##FUNCTION,	\
		t_q32:	Q32##FUNCTION,	\
		t_q64:	Q64##FUNCTION,	\
		t_q128:	Q128##FUNCTION,	\
		t_fixed:Fixed##FUNCTION,\
		t_f32:	F32##FUNCTION,	\
		t_f64:	F64##FUNCTION,	\
		t_f80:	F80##FUNCTION,	\
		t_f128:	F128##FUNCTION,	\
		t_float:Float##FUNCTION,\
	)(X, Y)



/*
** ************************************************************************** *|
**                          Integer Rounding Operations                       *|
** ************************************************************************** *|
*/

//! Returns the value of `x`, rounded to the nearest integer
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/round}
*/
#define						Math_Round(X)		DEFINE_GENERIC_MATH_FUNCTION(Round, X)
#define c_round				Math_Round
#define Math_FloatRound		Math_Round

//! Returns the value of `x`, rounded towards 0
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/trunc}
*/
#define						Math_Truncate(X)	DEFINE_GENERIC_MATH_FUNCTION(Truncate, X)
#define c_trunc				Math_Truncate
#define Math_FloatTrunc		Math_Truncate

//! Returns the value of `x`, rounded to the superior integer
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/floor}
*/
#define						Math_Floor(X)		DEFINE_GENERIC_MATH_FUNCTION(Floor, X)
#define c_floor				Math_Floor
#define Math_FloatFloor		Math_Floor

//! Returns the value of `x`, rounded to the inferior integer
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/ceil}
*/
#define						Math_Ceiling(X)		DEFINE_GENERIC_MATH_FUNCTION(Ceiling, X)
#define c_ceil				Math_Ceiling
#define Math_FloatCeiling	Math_Ceiling



/*
** ************************************************************************** *|
**                            Arithmetic Operations                           *|
** ************************************************************************** *|
*/

//! Returns the absolute value of `x` (makes `x` positive)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/abs}
**	@isostd{https://en.cppreference.com/w/c/numeric/math/fabs}
**
**	- Math: @f$ {|x|} @f$
*/
#define						Math_Abs(X)		DEFINE_GENERIC_MATH_FUNCTION(Abs, X)
#define c_abs				Math_Abs
#define Math_AbsoluteValue	Math_Abs



//! Returns the addition result of `x` and `y`
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/fmod}
*/
#define						Math_Add(X, Y)	DEFINE_GENERIC_MATH_OPERATOR(Add, X, Y)
#define c_add				Math_Add

//! Returns the subtraction result of `x` by `y`
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/fmod}
*/
#define						Math_Sub(X, Y)	DEFINE_GENERIC_MATH_OPERATOR(Sub, X, Y)
#define c_sub				Math_Sub
#define Math_Subtract		Math_Sub

//! Returns the multiplication result of `x` and `y`
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/fmod}
*/
#define						Math_Mul(X, Y)	DEFINE_GENERIC_MATH_OPERATOR(Mul, X, Y)
#define c_mul				Math_Mul
#define Math_Multiply		Math_Mul

//! Returns the quotient of euclidian division of `x` by `y`
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/fmod}
*/
#define						Math_Div(X, Y)	DEFINE_GENERIC_MATH_OPERATOR(Div, X, Y)
#define c_div				Math_Div
#define Math_Divide			Math_Div

//! Returns the remainder of euclidian division of `x` by `y`
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/fmod}
*/
#define						Math_Mod(X, Y)	DEFINE_GENERIC_MATH_OPERATOR(Mod, X, Y)
#define c_mod				Math_Mod
#define Math_Modulo			Math_Mod

//! Returns the value of `x` to the power of `y`
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/pow}
**
**	- Math: @f$ {x^y} @f$
*/
#define						Math_Pow(X, Y)	DEFINE_GENERIC_MATH_OPERATOR(Pow, X, Y)
#define c_pow				Math_Pow
#define Math_Power			Math_Pow

//! Returns the value of `x` to the power of `n` (integer)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/pow}
**
**	- Math: @f$ {x^n} @f$
*/
#define						Math_IntPow(X, N);	DEFINE_GENERIC_MATH_OPERATOR(IntPow, X, N)
#define c_intpow			Math_IntPow
#define Math_IntPower		Math_IntPow



//! Returns the square root of `x` (inverse of power of 2)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/sqrt}
**
**	- Math: @f$ {\sqrt {x}} @f$
*/
#define						Math_Root2(X)	DEFINE_GENERIC_MATH_FUNCTION(Root2, X)
#define c_sqrt				Math_Root2
#define Math_SqRt			Math_Root2
#define Math_SquareRoot		Math_Root2

//! Returns the cubic root of `x` (inverse of power of 3)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/cbrt}
**
**	- Math: @f$ {\sqrt[3] {x}} @f$
*/
#define						Math_Root3(X)	DEFINE_GENERIC_MATH_FUNCTION(Root3, X)
#define c_cbrt				Math_Root3
#define Math_CbRt			Math_Root3
#define Math_CubeRoot		Math_Root3

//! Returns the power-n-ic root of `x` (inverse of power of `n`)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/nrt}
**
**	- Math: @f$ {\sqrt[n] {x}} @f$
*/
#define						Math_RootN(X, Y)	DEFINE_GENERIC_MATH_OPERATOR(RootN, X, Y)
#define c_nrt				Math_RootN
#define Math_NRt			Math_RootN
#define Math_NRoot			Math_RootN

// TODO hypot: {\sqrt{x^2+y^2}}



/*
** ************************************************************************** *|
**                           Exponentiation Operations                        *|
** ************************************************************************** *|
*/

//! Returns the exponential function's value for `x`
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/exp}
**
**	- Math: @f$ {e^x} @f$
*/
#define							Math_Exp(X)		DEFINE_GENERIC_MATH_FUNCTION(Exp, X)
#define c_exp					Math_Exp
#define Math_Exponential		Math_Exp

//! Returns the natural logarithm of `x`
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/log}
**
**	- Math: @f$ {\ln {x}} @f$
*/
#define							Math_Ln(X)		DEFINE_GENERIC_MATH_FUNCTION(Ln, X)
#define c_ln					Math_Ln
#define Math_NaturalLogarithm	Math_Ln

//! Returns the binary (base-2) logarithm of `x`
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/log2}
**
**	- Math: @f$ {\log_{2} {x}} @f$
*/
#define							Math_Log2(X)	DEFINE_GENERIC_MATH_FUNCTION(Log2, X)
#define c_lg					Math_Log2
#define c_log2					Math_Log2
#define Math_Logarithm_Base2	Math_Log2

//! Returns the decimal (base-10) logarithm of `x`
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/log10}
**
**	- Math: @f$ {\log_{10} {x}} @f$
*/
#define							Math_Log10(X)	DEFINE_GENERIC_MATH_FUNCTION(Log10, X)
#define c_log					Math_Log10
#define c_log10					Math_Log10
#define Math_Logarithm_Base10	Math_Log10

//! Returns the base-`n` logarithm of `x`
/*!
**	@nonstd
**
**	- Math: @f$ {\log_{n} {x}} @f$
*/
#define							Math_LogN(X, Y)	DEFINE_GENERIC_MATH_OPERATOR(LogN, X, Y)
#define c_logn					Math_LogN
#define Math_Logarithm_BaseN	Math_LogN



/*
** ************************************************************************** *|
**                           Trigonometric Operations                         *|
** ************************************************************************** *|
*/

//! Returns the cosine of `x` (horizontal trigonometry coordinate)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/cos}
**
**	- Math: @f$ {\cos {x}} @f$
*/
#define						Math_Cos(X)		DEFINE_GENERIC_MATH_FUNCTION(Cos, X)
#define c_cos				Math_Cos
#define Math_Cosine			Math_Cos

//! Returns the sine of `x` (vertical trigonometry coordinate)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/sin}
**
**	- Math: @f$ {\sin {x}} @f$
*/
#define						Math_Sin(X)		DEFINE_GENERIC_MATH_FUNCTION(Sin, X)
#define c_sin				Math_Sin
#define Math_Sine			Math_Sin

//! Returns the tangent of `x` (trigonometry tangent line)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/tan}
**
**	- Math: @f$ {\tan {x}} @f$
*/
#define						Math_Tan(X)		DEFINE_GENERIC_MATH_FUNCTION(Tan, X)
#define c_tan				Math_Tan
#define Math_Tangent		Math_Tan

//! Returns the arc-cosine of `x` (inverse of the cos function)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/acos}
**
**	- Math: @f$ {\cos^{-1} {x}} @f$
*/
#define						Math_ArcCos(X)		DEFINE_GENERIC_MATH_FUNCTION(ArcCos, X)
#define c_acos				Math_ArcCos
#define Math_Cos_1			Math_ArcCos
#define Math_InvCosine		Math_ArcCos

//! Returns the arc-sine of `x` (inverse of the sin function)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/asin}
**
**	- Math: @f$ {\sin^{-1} {x}} @f$
*/
#define						Math_ArcSin(X)		DEFINE_GENERIC_MATH_FUNCTION(ArcSin, X)
#define c_asin				Math_ArcSin
#define Math_Sin_1			Math_ArcSin
#define Math_InvSine		Math_ArcSin

//! Returns the arc-tangent of `x` (inverse of the tan function)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/atan}
**
**	- Math: @f$ {\tan^{-1} {x}} @f$
*/
#define						Math_ArcTan(X)		DEFINE_GENERIC_MATH_FUNCTION(ArcTan, X)
#define c_atan				Math_ArcTan
#define Math_Tan_1			Math_ArcTan
#define Math_InvTangent		Math_ArcTan



//! Returns the arc-tangent of (`y` / `x`), used to find an angle from coordinates
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/atan2}
**
**	- Math: @f$ {\arctan(\frac{y}{x})} @f$, in the range @f$ {[-\pi;+\pi]} @f$
*/
#define							Math_ArcTan2(Y, X)	DEFINE_GENERIC_MATH_FUNCTION(ArcTan2, Y, X)
#define c_atan2					Math_ArcTan2
#define Math_ArcTan_YoverX		Math_ArcTan2
#define Math_ArcTangent2		Math_ArcTan2
#define Math_ArcTangent_YoverX	Math_ArcTan2



//! Returns the hyperbolic cosine of `x`
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/cosh}
**
**	- Math: @f$ {\cosh {x}} @f$
*/
#define								Math_CosH(X)	DEFINE_GENERIC_MATH_FUNCTION(CosH, X)
#define c_cosh						Math_CosH
#define Math_Cos_H					Math_CosH
#define Math_Cosine_Hyperbolic		Math_CosH

//! Returns the hyperbolic sine of `x`
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/sinh}
**
**	- Math: @f$ {\sinh {x}} @f$
*/
#define								Math_SinH(X)	DEFINE_GENERIC_MATH_FUNCTION(SinH, X)
#define c_sinh						Math_SinH
#define Math_Sin_H					Math_SinH
#define Math_Sine_Hyperbolic		Math_SinH

//! Returns the hyperbolic tangent of `x`
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/tanh}
**
**	- Math: @f$ {\tanh {x}} @f$
*/
#define								Math_TanH(X)	DEFINE_GENERIC_MATH_FUNCTION(TanH, X)
#define c_tanh						Math_TanH
#define Math_Tan_H					Math_TanH
#define Math_Tangent_Hyperbolic		Math_TanH

//! Returns the hyperbolic arc-cosine of `x` (inverse of the cosh function)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/acosh}
**
**	- Math: @f$ {\cosh^{-1} {x}} @f$
*/
#define								Math_InvCosH(X)	DEFINE_GENERIC_MATH_FUNCTION(InvCosH, X)
#define c_acosh						Math_InvCosH
#define Math_Cos_1_H				Math_InvCosH
#define Math_InvCosine_Hyperbolic	Math_InvCosH

//! Returns the hyperbolic arc-sine of `x` (inverse of the sinh function)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/asinh}
**
**	- Math: @f$ {\sinh^{-1} {x}} @f$
*/
#define								Math_InvSinH(X)	DEFINE_GENERIC_MATH_FUNCTION(InvSinH, X)
#define c_asinh						Math_InvSinH
#define Math_Sin_1_H				Math_InvSinH
#define Math_InvSine_Hyperbolic		Math_InvSinH

//! Returns the hyperbolic arc-tangent of `x` (inverse of the tanh function)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/atanh}
**
**	- Math: @f$ {\tanh^{-1} {x}} @f$
*/
#define								Math_InvTanH(X)	DEFINE_GENERIC_MATH_FUNCTION(InvTanH, X)
#define c_atanh						Math_InvTanH
#define Math_Tan_1_H				Math_InvTanH
#define Math_InvTangent_Hyperbolic	Math_InvTanH



// TODO hypot ?
// TODO lgamma ?
// TODO tgamma ?



/*! @} */
HEADER_END
#endif
