/*============================================================================*/
/*                                            ______________________________  */
/*  libccc/float.h                           |    __    __  ___      _____  | */
/*                                           |   / /\  / /\/ . |\   /  __|\ | */
/*  https://github.com/LexouDuck/libccc.git  |  / /_/ / / / . <_/  |  /___| | */
/*                                           | /___/\/_/ /___-'\   \____/\  | */
/* Comprehensive|Cross-platform|Customizable | \____/\__/\____-'    \____/  | */
/* This source code follows the MIT License. |______________________________| */
/*                                                                            */
/*============================================================================*/

#ifndef __LIBCCC_FLOAT_H
#define __LIBCCC_FLOAT_H
/*!@group{libccc_float}
** @{
**	This header defines the floating-point number primitive types and functions.
**
**	@isostd{C,https://en.cppreference.com/w/c/language/arithmetic_types#Real_floating_types}
**	@isostd{C,https://en.cppreference.com/w/c/language/floating_constant}
**	@isostd{C,https://en.cppreference.com/w/c/numeric/math}
**	@isostd{C,https://en.cppreference.com/w/c/numeric/fenv}
**	- https://en.wikipedia.org/wiki/IEEE_754
**
**	@file
*/

// TODO subnormal float util functions/macros ?

/*
** ************************************************************************** *|
**                                   Includes                                 *|
** ************************************************************************** *|
*/

#include "libccc.h"

HEADER_CPP

#ifndef __LIBCCC_FLOAT_T
#define __LIBCCC_FLOAT_T

/*
** ************************************************************************** *|
**                                 Definitions                                *|
** ************************************************************************** *|
*/



//! Primitive type: 32-bit 'single precision' IEEE-754 floating-point numbers
/*!
**	@isostd{C,https://en.cppreference.com/w/c/language/arithmetic_types#Real_floating_types}
**
**	- https://en.wikipedia.org/wiki/Single-precision_floating-point_format
*/
typedef float		t_f32;
TYPEDEF_ALIAS(		t_f32,	FLOAT_32,	PRIMITIVE)

//! Primitive type: 64-bit 'double precision' IEEE-754 floating-point numbers
/*!
**	@isostd{C,https://en.cppreference.com/w/c/language/arithmetic_types#Real_floating_types}
**
**	- https://en.wikipedia.org/wiki/Double-precision_floating-point_format
*/
typedef double		t_f64;
TYPEDEF_ALIAS(		t_f64,	FLOAT_64,	PRIMITIVE)

#ifdef	__float80
//! Primitive type: 80-bit 'extended precision' MacOS floating-point numbers (only certain platforms)
/*!
**	@isostd{C,https://gcc.gnu.org/onlinedocs/gcc/Floating-Types.html}
**
**	Consists of: 1 sign bit, 15 exponent bits, 1 integer bit, 63 fraction bits
*/
typedef _Float80	t_f80;
TYPEDEF_ALIAS(		t_f80,	FLOAT_80,	PRIMITIVE)
#elif (LIBCONFIG_BITS_FLOAT == 80)
	#error "Cannot set default float to 80-bit extended-precision, unavailable on this platform"
#endif

#ifdef	__float96
//! Primitive type: 96-bit 'extended precision' 68881 floating-point numbers (only certain platforms)
/*!
**	@isostd{C,https://gcc.gnu.org/onlinedocs/gcc/Floating-Types.html}
**
**	Consists of: 1 sign bit, 15 exponent bits, 16 pad bits, 1 integer bit, 63 fraction bits
*/
typedef _Float96	t_f96;
TYPEDEF_ALIAS(		t_f96,	FLOAT_96,	PRIMITIVE)
#elif (LIBCONFIG_BITS_FLOAT == 96)
	#error "Cannot set default float to 96-bit extended-precision, unavailable on this platform"
#endif

#ifdef	__float128
//! Primitive type: 32-bit 'quadruple precision' IEEE-754 floating-point numbers (only certain platforms)
/*!
**	@isostd{C,https://gcc.gnu.org/onlinedocs/gcc/Floating-Types.html}
**
**	- https://en.wikipedia.org/wiki/Quadruple-precision_floating-point_format
*/
typedef _Float128	t_f128;
TYPEDEF_ALIAS(		t_f128,	FLOAT_128,	PRIMITIVE)
#elif (LIBCONFIG_BITS_FLOAT == 128)
	#error "Cannot set default float to 128-bit quadruple-precision, unavailable on this platform"
#endif



//! The actual underlying type for the `t_float` configurable type
#define FLOAT_T		CONCAT(t_f,LIBCONFIG_BITS_FLOAT)
//! The actual underlying type for the `t_float` configurable type, in uppercase
#define FLOAT_TYPE	CONCAT(F,LIBCONFIG_BITS_FLOAT)

//! The configurable-size floating-point number primitive type.
/*!
**	@isostd{C,https://en.cppreference.com/w/c/language/arithmetic_types#Real_floating_types}
**
**	A floating-point number is a number which is encoded as 3 distinct parts:
**	- A sign bit (typically the rightmost bit)
**	- An exponent (typically a binary power, ie: 2^number)
**	- A mantissa/significand, which stores the most signficant part of this number
**	So, a floating point number is very much like a number in scientific notation.
**	Also, note that, the way floats are encoded with the IEEE-754 specification,
**	there are as many respresentable floating-point numbers between `0.` and `+1.`,
**	as there are between `+1.` and `+INFINITY`. The higher you go along the real numbers,
**	the more imprecise your floating-point calculations will become.
**	This type can express a number between negative #INFINITY and positive #INFINITY.
*/
typedef CONCAT(t_f,LIBCONFIG_BITS_FLOAT)	t_float;
TYPEDEF_ALIAS(t_float, FLOAT, PRIMITIVE)



#if(LIBCONFIG_BITS_FLOAT != 32 && \
	LIBCONFIG_BITS_FLOAT != 64 && \
	LIBCONFIG_BITS_FLOAT != 80 && \
	LIBCONFIG_BITS_FLOAT != 96 && \
	LIBCONFIG_BITS_FLOAT != 128)
	#error "LIBCONFIG_BITS_FLOAT must be equal to one of: 32, 64, 80, 96, 128"
#endif



//! The floating-point "not a number" value.
/*!
**	@isostd{C,https://en.cppreference.com/w/c/numeric/math/nan}
**	@isostd{C,https://en.cppreference.com/w/c/numeric/math/NAN}
*/
//!@{
#ifndef NAN
#define NAN			(0. / 0.)
#endif
#ifndef NOTNUMBER
#define NOTNUMBER	NAN
#endif
//!@}

//! Checks if the given 'x' has a "not a number" value.
/*!
**	@isostd{C,https://en.cppreference.com/w/c/numeric/math/isnan}
**
**	Also, define isnan() for ANSI C compatibility, if needed.
*/
//!@{
#ifndef isnan
#define isnan(X)	(X != X)
#endif
#ifndef IS_NAN
#define IS_NAN(X)		isnan(X)
#endif
#ifndef IS_NOTNUMBER
#define IS_NOTNUMBER(X)	isnan(X)
#endif
//!@}



//! The floating-point infinity value (use `-INF` for negative)
/*!
**	@isostd{C,https://en.cppreference.com/w/c/numeric/math/INFINITY}
*/
//!@{
#ifndef INF
#define INF			(1. / 0.)
#endif
#ifndef INFINITY
#define INFINITY	INF
#endif
//!@}

//! Checks if the given 'x' is either +INFINITY or -INFINITY
/*!
**	@isostd{C,https://en.cppreference.com/w/c/numeric/math/isinf}
**
**	Also, define isinf() for ANSI C compatibility, if needed.
*/
//!@{
#ifndef isinf
#define isinf(X)	(isnan((X) - (X)) && !isnan(X))
#endif
#ifndef IS_INF
#define IS_INF(X)		isinf(X)
#endif
#ifndef IS_INFINITY
#define IS_INFINITY(X)	isinf(X)
#endif
//!@}



// TODO add wrapper for isfinite(x)
// TODO add wrapper for isnormal(x)
// TODO add wrapper for isunordered(x, y)
// TODO add wrapper for fpclassify()



//! TODO add HUGE and TINY min/max value macros
/*!
**	@isostd{C,https://en.cppreference.com/w/c/numeric/math/HUGE_VAL}
**	@isostd{C,https://en.cppreference.com/w/c/types/limits#Limits_of_floating_point_types}
*/



//! TODO document this
#define SAMPLE_NB		(1024)



//! This macro sets the rounding behavior for floating-point number operations.
/*!
**	Rounding behavior for floating-point types. Possible values are:
**	`-1`	undetermined
**	 `0`	towards zero
**	 `1`	to nearest
**	 `2`	toward positive infinity
**	 `3`	toward negative infinity
*/
#ifndef FLT_ROUNDS
#define FLT_ROUNDS	1
#endif

//! This macro sets the floating-point expression evaluation method (ie: how floats are operated upon).
/*!
**	The floating-point evaluation method. Possible values are:
**	`-1`	indeterminate
**	 `0`	evaluate all operations and constants, whose semantic type has at most the range and precision of float, to the range and precision of float;
**	    	evaluate all other operations and constants to the range and precision of the semantic type.
**	 `1`	evaluate all operations and constants, whose semantic type has at most the range and precision of double, to the range and precision of double;
**	    	evaluate all other operations and constants to the range and precision of the semantic type.
**	 `2`	evaluate all operations and constants, whose semantic type has at most the range and precision of long double, to the range and precision of long double;
**	    	evaluate all other operations and constants to the range and precision of the semantic type.
**	 `N`	where `_FloatN`  is a supported interchange floating type:
**	    	evaluate all operations and constants, whose semantic type has at most the range and precision of the _FloatN type, to the range and precision of the _FloatN type;
**	    	evaluate all other operations and constants to the range and precision of the semantic type.
**	 `N + 1`, where `_FloatNx` is a supported extended floating type:
**	    	evaluate operations and constants, whose semantic type has at most the range and precision of the _FloatNx type, to the range and precision of the _FloatNx type;
**	    	evaluate all other operations and constants to the range and precision of the semantic type.
*/
#ifndef FLT_EVAL_METHOD
#define FLT_EVAL_METHOD	1
#endif



//! If `(value >= FLOAT_THRESHOLD_HUGE)`, Float_ToString() functions will write in scientific notation rather than decimal notation
#define FLOAT_THRESHOLD_HUGE	(1e+9)
//! If `(value <= FLOAT_THRESHOLD_TINY)`, Float_ToString() functions will write in scientific notation rather than decimal notation
#define FLOAT_THRESHOLD_TINY	(1e-9)

// TODO add IS_NORMAL() and IS_SUBNORMAL()

// TODO instead of using FLT_MIN/FLT_MAX/FLT_EPSILON, calculate them manually (in a cross-platform manner)

/*! @name 32-bit float bitwise constants
**	IEEE 754 32-bit floating point "single" precision bitwise macros
*/
//!@{
#define F32_SIGNED			(0x80000000)			//!< A 32-bit floating-point number's sign bit (bitmask)
#define F32_EXPONENT_BIAS	(127)					//!< A 32-bit floating-point number's exponent bias offset
#define F32_EXPONENT		(0x7F800000)			//!< A 32-bit floating-point number's exponent bit region (bitmask)
#define F32_EXPONENT_ZERO	(0x3F800000)			//!< A 32-bit floating-point number's 0-exponent value, accounting for bias (bitmask)
#define F32_EXPONENT_BITS	(8)						//!< A 32-bit floating-point number's amount of bits dedicated to the exponent
#define F32_MANTISSA		(0x007FFFFF)			//!< A 32-bit floating-point number's mantissa bit region (bitmask)
#define F32_MANTISSA_SIGNED	(0x807FFFFF)			//!< A 32-bit floating-point number's mantissa and sign bit regions (bitmask)
#define F32_MANTISSA_BITS	(23)					//!< A 32-bit floating-point number's amount of bits dedicated to the mantissa
#define F32_INIT_VALUE		(0x1.p-23)				//!< A 32-bit floating-point number's value if all bits are zero
#define F32_NEXT(X, TOWARD)	(nextafterf(X, TOWARD))	//!< Returns the nearest float value greater than the one given as `X`, going in the direction of `TOWARD`
#define F32_MIN				(FLT_MIN)				//!< A 32-bit floating-point's minimum representable positive normal value.
#define F32_MAX				(FLT_MAX)				//!< A 32-bit floating-point's maximum finite representable value.
#define F32_EPSILON			(FLT_EPSILON)			//!< Difference between 1 and the least value greater than 1 that is representable.
//!@}

/*! @name 64-bit float bitwise constants
**	IEEE 754 64-bit floating point double-precision bitwise macros
*/
//!@{
#define F64_SIGNED			(0x8000000000000000)	//!< A 64-bit floating-point number's sign bit (bitmask)
#define F64_EXPONENT_BIAS	(1023)					//!< A 64-bit floating-point number's exponent bias offset
#define F64_EXPONENT		(0x7FF0000000000000)	//!< A 64-bit floating-point number's exponent bit region (bitmask)
#define F64_EXPONENT_ZERO	(0x3FF0000000000000)	//!< A 64-bit floating-point number's 0-exponent value, accounting for bias (bitmask)
#define F64_EXPONENT_BITS	(11)					//!< A 64-bit floating-point number's amount of bits dedicated to the exponent
#define F64_MANTISSA		(0x000FFFFFFFFFFFFF)	//!< A 64-bit floating-point number's mantissa bit region (bitmask)
#define F64_MANTISSA_SIGNED	(0x800FFFFFFFFFFFFF)	//!< A 64-bit floating-point number's mantissa and sign bit regions (bitmask)
#define F64_MANTISSA_BITS	(52)					//!< A 64-bit floating-point number's amount of bits dedicated to the mantissa
#define F64_INIT_VALUE		(0x1.p-52)				//!< A 64-bit floating-point number's value if all bits are zero
#define F64_NEXT(X, TOWARD)	(nextafterd(X, TOWARD))	//!< Returns the nearest float value greater than the one given as `X`, going in the direction of `TOWARD`
#define F64_MIN				(DBL_MIN)				//!< A 64-bit floating-point's minimum representable positive normal value.
#define F64_MAX				(DBL_MAX)				//!< A 64-bit floating-point's maximum finite representable value.
#define F64_EPSILON			(DBL_EPSILON)			//!< Difference between 1 and the least value greater than 1 that is representable.
//!@}

/*! @name 80-bit float bitwise constants
**	x86 80-bit floating point extended precision bitwise macros
*/
//!@{
#define F80_SIGNED			(0x80000000000000000000l)	//!< A 80-bit floating-point number's sign bit (bitmask)
#define F80_EXPONENT_BIAS	(16383)						//!< A 80-bit floating-point number's exponent bias offset
#define F80_EXPONENT		(0x7FFF0000000000000000l)	//!< A 80-bit floating-point number's exponent bit region (bitmask)
#define F80_EXPONENT_ZERO	(0x3FFF0000000000000000l)	//!< A 80-bit floating-point number's 0-exponent value, accounting for bias (bitmask)
#define F80_EXPONENT_BITS	(15)						//!< A 80-bit floating-point number's amount of bits dedicated to the exponent
#define F80_MANTISSA		(0x0000FFFFFFFFFFFFFFFFl)	//!< A 80-bit floating-point number's mantissa bit region (bitmask)
#define F80_MANTISSA_SIGNED	(0x8000FFFFFFFFFFFFFFFFl)	//!< A 80-bit floating-point number's mantissa and sign bit regions (bitmask)
#define F80_MANTISSA_BITS	(64)						//!< A 80-bit floating-point number's amount of bits dedicated to the mantissa
#define F80_INIT_VALUE		(0x1.p-64)					//!< A 80-bit floating-point number's value if all bits are zero
#define F80_NEXT(X, TOWARD)	(nextafterld(X, TOWARD))	//!< Returns the nearest float value greater than the one given as `X`, going in the direction of `TOWARD`
#define F80_MIN				(LDBL_MIN)					//!< A 80-bit floating-point's minimum representable positive normal value.
#define F80_MAX				(LDBL_MAX)					//!< A 80-bit floating-point's maximum finite representable value.
#define F80_EPSILON			(LDBL_EPSILON)				//!< Difference between 1 and the least value greater than 1 that is representable.
//!@}

/*! @name 128-bit float bitwise constants
**	IEEE 754 128-bit floating point quadruple-precision bitwise macros
*/
//!@{
#define F128_SIGNED				(0x80000000000000000000000000000000l)	//!< A 128-bit floating-point number's sign bit (bitmask)
#define F128_EXPONENT_BIAS		(16383)									//!< A 128-bit floating-point number's exponent bias offset
#define F128_EXPONENT			(0x7FFF0000000000000000000000000000l)	//!< A 128-bit floating-point number's exponent bit region (bitmask)
#define F128_EXPONENT_ZERO		(0x3FFF0000000000000000000000000000l)	//!< A 128-bit floating-point number's 0-exponent value, accounting for bias (bitmask)
#define F128_EXPONENT_BITS		(15)									//!< A 128-bit floating-point number's amount of bits dedicated to the exponent
#define F128_MANTISSA			(0x0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFl)	//!< A 128-bit floating-point number's mantissa bit region (bitmask)
#define F128_MANTISSA_SIGNED	(0x8000FFFFFFFFFFFFFFFFFFFFFFFFFFFFl)	//!< A 128-bit floating-point number's mantissa and sign bit regions (bitmask)
#define F128_MANTISSA_BITS		(112)									//!< A 128-bit floating-point number's amount of bits dedicated to the mantissa
#define F128_INIT_VALUE			(0x1.p-112)								//!< A 128-bit floating-point number's value if all bits are zero
#define F128_NEXT(X, TOWARD)	(nextafterq(X, TOWARD))					//!< Returns the nearest float value greater than the one given as `X`, going in the direction of `TOWARD`
#define F128_MIN				(LDBL_MIN)								//!< A 128-bit floating-point's minimum representable positive normal value.
#define F128_MAX				(LDBL_MAX)								//!< A 128-bit floating-point's maximum finite representable value.
#define F128_EPSILON			(LDBL_EPSILON)							//!< Difference between 1 and the least value greater than 1 that is representable.
//!@}



/*! @name float bitwise constants
**	Depending on the 't_float' type (_FLOAT_32_ or _FLOAT_64_, etc) chosen,
**	the appropriate bitwise macros will be used by the math functions.
**	It is often better to only use one type of floating-point precision
**	for a given program, so the best way to do that is by using the 'FLOAT_'
**	macros defined below, rather than the 'F32_' or 'F64_' macros above.
*/
//!@{
#define FLOAT_SIGNED			CONCAT(FLOAT_TYPE,_SIGNED)
#define FLOAT_EXPONENT_BIAS		CONCAT(FLOAT_TYPE,_EXPONENT_BIAS)
#define FLOAT_EXPONENT			CONCAT(FLOAT_TYPE,_EXPONENT)
#define FLOAT_EXPONENT_ZERO		CONCAT(FLOAT_TYPE,_EXPONENT_ZERO)
#define FLOAT_EXPONENT_BITS		CONCAT(FLOAT_TYPE,_EXPONENT_BITS)
#define FLOAT_MANTISSA			CONCAT(FLOAT_TYPE,_MANTISSA)
#define FLOAT_MANTISSA_SIGNED	CONCAT(FLOAT_TYPE,_MANTISSA_SIGNED)
#define FLOAT_MANTISSA_BITS		CONCAT(FLOAT_TYPE,_MANTISSA_BITS)
#define FLOAT_INIT_VALUE		CONCAT(FLOAT_TYPE,_INIT_VALUE)
#define FLOAT_NEXT				CONCAT(FLOAT_TYPE,_NEXT)
#define FLOAT_MIN				CONCAT(FLOAT_TYPE,_MIN)
#define FLOAT_MAX				CONCAT(FLOAT_TYPE,_MAX)
#define FLOAT_EPSILON			CONCAT(FLOAT_TYPE,_EPSILON)
//!@}



//! Union type to allow direct bitwise manipulation of floating-point values
/*!
**	This union type is used in several math function implementations, to
**	manipulate float bits directly, by using bitwise operators with int types.
*/
typedef union float_cast
{
	t_float	value_float;
#if (LIBCONFIG_BITS_FLOAT == 32)
	s32		value_int;
#elif (LIBCONFIG_BITS_FLOAT == 64)
	s64		value_int;
#else
	s64[2]	value_int;
#endif
}				u_float_cast;



#endif
#ifndef __LIBCCC_FLOAT_F
#define __LIBCCC_FLOAT_F

/*
** ************************************************************************** *|
**                        Floating-point number functions                     *|
** ************************************************************************** *|
*/

//! A smart constructor: calls the appropriate `Float_From*()` function from the given argument type
//!@{
#define DEFINEFUNC_Float(X, FUNCTYPE) \
	_Generic((X),	\
		t_s16:	 FUNCTYPE##_FromInt,	\
		t_s32:	 FUNCTYPE##_FromInt,	\
		t_s64:	 FUNCTYPE##_FromInt,	\
		t_s128:	 FUNCTYPE##_FromInt,	\
		t_sint:  FUNCTYPE##_FromInt,	\
		t_q16:	 FUNCTYPE##_FromFixed,	\
		t_q32:	 FUNCTYPE##_FromFixed,	\
		t_q64:	 FUNCTYPE##_FromFixed,	\
		t_q128:	 FUNCTYPE##_FromFixed,	\
		t_fixed: FUNCTYPE##_FromFixed,	\
		t_f32:	 FUNCTYPE##_FromFloat,	\
		t_f64:	 FUNCTYPE##_FromFloat,	\
		t_f80:	 FUNCTYPE##_FromFloat,	\
		t_f128:	 FUNCTYPE##_FromFloat,	\
		t_float: FUNCTYPE##_FromFloat,	\
	)(X)
#define Float(X)	DEFINEFUNC_Float(X, Fixed)
#define F32(X)		DEFINEFUNC_Float(X, F32)
#define F64(X)		DEFINEFUNC_Float(X, F64)
#define F80(X)		DEFINEFUNC_Float(X, F80)
#define F128(X)		DEFINEFUNC_Float(X, F128)
//!@}

//! Returns the nearest floating-point value to the given integer `number`
/*!
**	TODO document
*/
//!@{
#define					Float_FromInt	CONCAT(FIXED_TYPE,_FromInt)
t_f32					F32_FromInt(t_sint number);
t_f64					F64_FromInt(t_sint number);
#ifdef __float80
t_f16					F80_FromInt(t_sint number);
#endif
#ifdef __float128
t_f128					F128_FromInt(t_sint number);
#endif
#define c_itofloat		Float_FromInt 	//!< @alias{Float_FromInt}
#define c_itof16		F16_FromInt 	//!< @alias{F16_FromInt}
#define c_itof32		F32_FromInt 	//!< @alias{F32_FromInt}
#define c_itof64		F64_FromInt 	//!< @alias{F64_FromInt}
#define c_itof128		F128_FromInt 	//!< @alias{F128_FromInt}
//!@}

//! Returns the nearest floating-point value to the given fixed-point `number`
/*!
**	TODO document
*/
//!@{
#define					Float_FromFixed	CONCAT(FIXED_TYPE,_FromFixed)
t_f32					F32_FromFixed(t_fixed number);
t_f64					F64_FromFixed(t_fixed number);
#ifdef __float80
t_f16					F80_FromFixed(t_fixed number);
#endif
#ifdef __float128
t_f128					F128_FromFixed(t_fixed number);
#endif
#define c_qtofloat		Float_FromFixed	//!< @alias{Float_FromFixed}
#define c_qtof16		F16_FromFixed	//!< @alias{F16_FromFixed}
#define c_qtof32		F32_FromFixed	//!< @alias{F32_FromFixed}
#define c_qtof64		F64_FromFixed	//!< @alias{F64_FromFixed}
#define c_qtof128		F128_FromFixed	//!< @alias{F128_FromFixed}
//!@}

//! Returns the nearest floating-point value to the given floating-point `number`
/*!
**	TODO document
*/
//!@{
#define	 				Float_FromFloat	CONCAT(FIXED_TYPE,_FromFloat)
t_f32	 				F32_FromFloat(t_float number);
t_f64	 				F64_FromFloat(t_float number);
#ifdef __float80
t_f16	 				F80_FromFloat(t_float number);
#endif
#ifdef __float128
t_f128	 				F128_FromFloat(t_float number);
#endif
#define c_ftofloat		Float_FromFloat	//!< @alias{Float_FromFloat}
#define c_ftof16		F16_FromFloat	//!< @alias{F16_FromFloat}
#define c_ftof32		F32_FromFloat	//!< @alias{F32_FromFloat}
#define c_ftof64		F64_FromFloat	//!< @alias{F64_FromFloat}
#define c_ftof128		F128_FromFloat	//!< @alias{F128_FromFloat}
//!@}



//! Creates a float value from its individual component parts.
/*!
**	@nonstd
**
**	@param	integer		The significant digits of this floating-point number
**	@param	exponent	The (base 2) exponent of this floating-point number,
**						this is applied like so: `integer * pow(2, exponent)`
**	@returns
**	The floating-point number value resulting from putting together
**	the given `integer` and `exponent` parts.
*/
//!@{
#define					Float_From	CONCAT(FLOAT_TYPE,_From)
t_f32					F32_From(t_sint integer, t_sint exponent);
t_f64					F64_From(t_sint integer, t_sint exponent);
#ifdef	__float80
t_f80					F80_From(t_sint integer, t_sint exponent);
#endif
#ifdef	__float128
t_f128					F128_From(t_sint integer, t_sint exponent);
#endif
#define c_tofloat		Float_From	//!< @alias{Float_From}
#define c_tof32			F32_From	//!< @alias{F32_From}
#define c_tof64			F64_From	//!< @alias{F64_From}
#define c_tof80			F80_From	//!< @alias{F80_From}
#define c_tof128		F128_From	//!< @alias{F128_From}
//!@}



//! Returns the base-2 exponent of the given number `x`, as an integer
/*!
**	@isostd{C89,https://en.cppreference.com/w/c/numeric/math/ilogb}
*/
//!@{
#define						Float_GetExp2	CONCAT(FLOAT_TYPE,_GetExp2)
t_sint						F32_GetExp2(t_f32 number);
t_sint						F64_GetExp2(t_f64 number);
#ifdef	__float80
t_sint						F80_GetExp2(t_f80 number);
#endif
#ifdef	__float128
t_sint						F128_GetExp2(t_f128 number);
#endif
#define c_fgetexp2			Float_GetExp2	//!< @alias{Float_GetExp2}
#define c_f32getexp2		F32_GetExp2		//!< @alias{F32_GetExp2}
#define c_f64getexp2		F64_GetExp2		//!< @alias{F64_GetExp2}
#define c_f80getexp2		F80_GetExp2		//!< @alias{F80_GetExp2}
#define c_f128getexp2		F128_GetExp2	//!< @alias{F128_GetExp2}

#define Float_GetExponent2	Float_GetExp2	//!< @alias{Float_GetExp2}
#define c_filogb			Float_GetExp2	//!< @alias{Float_GetExp2}
#define c_filog2			Float_GetExp2	//!< @alias{Float_GetExp2}
//!@}

//! Returns the base-10 exponent of the given number `x`, as an integer
/*!
**	@nonstd
*/
//!@{
#define						Float_GetExp10	CONCAT(FLOAT_TYPE,_GetExp10)
t_sint						F32_GetExp10(t_f32 number);
t_sint						F64_GetExp10(t_f64 number);
#ifdef	__float80
t_sint						F80_GetExp10(t_f80 number);
#endif
#ifdef	__float128
t_sint						F128_GetExp10(t_f128 number);
#endif
#define c_fgetexp10			Float_GetExp10	//!< @alias{Float_GetExp10}
#define c_f32getexp10		F32_GetExp10	//!< @alias{F32_GetExp10}
#define c_f64getexp10		F64_GetExp10	//!< @alias{F64_GetExp10}
#define c_f80getexp10		F80_GetExp10	//!< @alias{F80_GetExp10}
#define c_f128getexp10		F128_GetExp10	//!< @alias{F128_GetExp10}

#define Float_GetExponent10	Float_GetExp10	//!< @alias{Float_GetExp10}
#define c_filogd			Float_GetExp10	//!< @alias{Float_GetExp10}
#define c_filog10			Float_GetExp10	//!< @alias{Float_GetExp10}
//!@}



/*
** ************************************************************************** *|
**                       Floating-point logic operators                       *|
** ************************************************************************** *|
*/

//! Returns `TRUE` if the 2 given floating-point values are exactly equal (operator: `==`)
/*!
**	@nonstd
**
**	@param	number1	The first value to check for (exact) equality
**	@param	number2	The second value to check for (exact) equality
**	@returns
**	`TRUE` if the 2 given floating-point values are exactly equal,
**	otherwise `FALSE`.
*/
//!@{
#define					Float_Equals	CONCAT(FLOAT_TYPE,_Equals)
t_bool					F32_Equals(t_f32 number1, t_f32 number2);
t_bool					F64_Equals(t_f64 number1, t_f64 number2);
#ifdef	__float80
t_bool					F80_Equals(t_f80 number1, t_f80 number2);
#endif
#ifdef	__float128
t_bool					F128_Equals(t_f128 number1, t_f128 number2);
#endif
#define c_fequ			Float_Equals//!< @alias{Float_Equals}
#define c_f32equ		F32_Equals	//!< @alias{F32_Equals}
#define c_f64equ		F64_Equals	//!< @alias{F64_Equals}
#define c_f80equ		F80_Equals	//!< @alias{F80_Equals}
#define c_f128equ		F128_Equals	//!< @alias{F128_Equals}
//!@}



//! Returns `TRUE` if the 2 given floating-point values are close to equal (operator: `~=`)
/*!
**	@nonstd
**
**	@param	number1	The first value to check for (approximate) equality
**	@param	number2	The second value to check for (approximate) equality
**	@returns
**	`TRUE` if the 2 given floating-point values are close enough to be considered equal
**	(using #FLOAT_APPROX as a comparison margin), otherwise `FALSE`.
*/
//!@{
#define					Float_EqualsApprox	CONCAT(FLOAT_TYPE,_EqualsApprox)
t_bool					F32_EqualsApprox(t_f32 number1, t_f32 number2);
t_bool					F64_EqualsApprox(t_f64 number1, t_f64 number2);
#ifdef	__float80
t_bool					F80_EqualsApprox(t_f80 number1, t_f80 number2);
#endif
#ifdef	__float128
t_bool					F128_EqualsApprox(t_f128 number1, t_f128 number2);
#endif
#define c_fequa			Float_EqualsApprox	//!< @alias{Float_EqualsApprox}
#define c_f32equa		F32_EqualsApprox	//!< @alias{F32_EqualsApprox}
#define c_f64equa		F64_EqualsApprox	//!< @alias{F64_EqualsApprox}
#define c_f80equa		F80_EqualsApprox	//!< @alias{F80_EqualsApprox}
#define c_f128equa		F128_EqualsApprox	//!< @alias{F128_EqualsApprox}
//!@}



// TODO Float_LessThan

// TODO Float_GreaterThan



/*
** ************************************************************************** *|
**                         Float-to-String Conversions                        *|
** ************************************************************************** *|
*/

//! Get the shortest string representation of the given floating-point number (dec or exp), with 'precision' fractional digits
/*!
**	@nonstd `ftoa()`
**
**	@param	number		The number to convert to a string
**	@param	precision	The amount of digits to print after the decimal point
**	@returns
**	A newly allocated string representation of the given `number`, with `precision` digits
*/
//!@{
#define					Float_ToString	CONCAT(FLOAT_TYPE,_ToString)
_MALLOC()	t_char*		F32_ToString(t_f32 number, t_u8 precision);
_MALLOC()	t_char*		F64_ToString(t_f64 number, t_u8 precision);
#ifdef	__float80
_MALLOC()	t_char*		F80_ToString(t_f80 number, t_u8 precision);
#endif
#ifdef	__float128
_MALLOC()	t_char*		F128_ToString(t_f128 number, t_u8 precision);
#endif
#define c_ftostr		Float_ToString	//!< @alias{Float_ToString}
#define c_f32tostr		F32_ToString	//!< @alias{F32_ToString}
#define c_f64tostr		F64_ToString	//!< @alias{F64_ToString}
#define c_f80tostr		F80_ToString	//!< @alias{F80_ToString}
#define c_f128tostr		F128_ToString	//!< @alias{F128_ToString}
//!@}



//! Get the string scientific notation of a floating-point number, with 'precision' fractional digits
/*!
**	@nonstd
**
**	@param	number		The number to convert to a string
**	@param	precision	The amount of digits to print after the decimal point
**	@returns
**	A newly allocated string representation of the given `number`, with `precision` digits
*/
//!@{
#define								Float_ToString_Exp	CONCAT(FLOAT_TYPE,_ToString_Exp)
_MALLOC()	t_char*					F32_ToString_Exp(t_f32 number, t_u8 precision);
_MALLOC()	t_char*					F64_ToString_Exp(t_f64 number, t_u8 precision);
#ifdef	__float80
_MALLOC()	t_char*					F80_ToString_Exp(t_f80 number, t_u8 precision);
#endif
#ifdef	__float128
_MALLOC()	t_char*					F128_ToString_Exp(t_f128 number, t_u8 precision);
#endif
#define c_ftostrexp					Float_ToString_Exp	//!< @alias{Float_ToString_Exp}
#define c_f32tostrexp				F32_ToString_Exp	//!< @alias{F32_ToString_Exp}
#define c_f64tostrexp				F64_ToString_Exp	//!< @alias{F64_ToString_Exp}
#define c_f80tostrexp				F80_ToString_Exp	//!< @alias{F80_ToString_Exp}
#define c_f128tostrexp				F128_ToString_Exp	//!< @alias{F128_ToString_Exp}

#define c_ftostrsci					Float_ToString_Exp	//!< @alias{Float_ToString_Exp}
#define c_f32tostrsci				F32_ToString_Exp	//!< @alias{F32_ToString_Exp}
#define c_f64tostrsci				F64_ToString_Exp	//!< @alias{F64_ToString_Exp}
#define c_f80tostrsci				F80_ToString_Exp	//!< @alias{F80_ToString_Exp}
#define c_f128tostrsci				F128_ToString_Exp	//!< @alias{F128_ToString_Exp}

#define Float_ToString_Exponential	Float_ToString_Exp	//!< @alias{Float_ToString_Exp}
#define Float_ToString_Scientific	Float_ToString_Exp	//!< @alias{Float_ToString_Exp}
#define Float_ToString_Sci			Float_ToString_Exp	//!< @alias{Float_ToString_Exp}
#define F32_ToString_Exponential	F32_ToString_Exp	//!< @alias{F32_ToString_Exp}
#define F32_ToString_Scientific		F32_ToString_Exp	//!< @alias{F32_ToString_Exp}
#define F32_ToString_Sci			F32_ToString_Exp	//!< @alias{F32_ToString_Exp}
#define F64_ToString_Exponential	F64_ToString_Exp	//!< @alias{F64_ToString_Exp}
#define F64_ToString_Scientific		F64_ToString_Exp	//!< @alias{F64_ToString_Exp}
#define F64_ToString_Sci			F64_ToString_Exp	//!< @alias{F64_ToString_Exp}
#define F80_ToString_Exponential	F80_ToString_Exp	//!< @alias{F80_ToString_Exp}
#define F80_ToString_Scientific		F80_ToString_Exp	//!< @alias{F80_ToString_Exp}
#define F80_ToString_Sci			F80_ToString_Exp	//!< @alias{F80_ToString_Exp}
#define F128_ToString_Exponential	F128_ToString_Exp	//!< @alias{F128_ToString_Exp}
#define F128_ToString_Scientific	F128_ToString_Exp	//!< @alias{F128_ToString_Exp}
#define F128_ToString_Sci			F128_ToString_Exp	//!< @alias{F128_ToString_Exp}
//!@}



//! Get the string decimal representation of a floating-point number, with 'precision' fractional digits
/*!
**	@nonstd
**
**	@param	number		The number to convert to a string
**	@param	precision	The amount of digits to print after the decimal point
**	@returns
**	A newly allocated string representation of the given `number`, with `precision` digits
*/
//!@{
#define					Float_ToString_Dec	CONCAT(FLOAT_TYPE,_ToString_Dec)
_MALLOC()	t_char*		F32_ToString_Dec(t_f32 number, t_u8 precision);
_MALLOC()	t_char*		F64_ToString_Dec(t_f64 number, t_u8 precision);
#ifdef	__float80
_MALLOC()	t_char*		F80_ToString_Dec(t_f80 number, t_u8 precision);
#endif
#ifdef	__float128
_MALLOC()	t_char*		F128_ToString_Dec(t_f128 number, t_u8 precision);
#endif
#define c_ftostrdec		Float_ToString_Dec	//!< @alias{Float_ToString_Dec}
#define c_f32tostrdec	F32_ToString_Dec	//!< @alias{F32_ToString_Dec}
#define c_f64tostrdec	F64_ToString_Dec	//!< @alias{F64_ToString_Dec}
#define c_f80tostrdec	F80_ToString_Dec	//!< @alias{F80_ToString_Dec}
#define c_f128tostrdec	F128_ToString_Dec	//!< @alias{F128_ToString_Dec}
//!@}



//! Get the string hexadecimal representation of a floating-point number, with 'precision' fractional digits (if 0, max precision)
/*!
**	@nonstd
**
**	@param	number		The number to convert to a string
**	@param	precision	The amount of digits to print after the decimal point
**	@returns
**	A newly allocated string representation of the given `number`, with `precision` digits
*/
//!@{
#define					Float_ToString_Hex	CONCAT(FLOAT_TYPE,_ToString_Hex)
_MALLOC()	t_char*		F32_ToString_Hex(t_f32 number, t_u8 precision);
_MALLOC()	t_char*		F64_ToString_Hex(t_f64 number, t_u8 precision);
#ifdef	__float80
_MALLOC()	t_char*		F80_ToString_Hex(t_f80 number, t_u8 precision);
#endif
#ifdef	__float128
_MALLOC()	t_char*		F128_ToString_Hex(t_f128 number, t_u8 precision);
#endif
#define c_ftostrhex		Float_ToString_Hex	//!< @alias{Float_ToString_Hex}
#define c_f32tostrhex	F32_ToString_Hex	//!< @alias{F32_ToString_Hex}
#define c_f64tostrhex	F64_ToString_Hex	//!< @alias{F64_ToString_Hex}
#define c_f80tostrhex	F80_ToString_Hex	//!< @alias{F80_ToString_Hex}
#define c_f128tostrhex	F128_ToString_Hex	//!< @alias{F128_ToString_Hex}
//!@}



//! Get the string binary representation of a floating-point number, with 'precision' fractional digits (if 0, max precision)
/*!
**	@nonstd
**
**	@param	number		The number to convert to a string
**	@param	precision	The amount of digits to print after the decimal point
**	@returns
**	A newly allocated string representation of the given `number`, with `precision` digits
*/
//!@{
#define					Float_ToString_Bin	CONCAT(FLOAT_TYPE,_ToString_Bin)
_MALLOC()	t_char*		F32_ToString_Bin(t_f32 number, t_u8 precision);
_MALLOC()	t_char*		F64_ToString_Bin(t_f64 number, t_u8 precision);
#ifdef	__float80
_MALLOC()	t_char*		F80_ToString_Bin(t_f80 number, t_u8 precision);
#endif
#ifdef	__float128
_MALLOC()	t_char*		F128_ToString_Bin(t_f128 number, t_u8 precision);
#endif
#define c_ftostrbin		Float_ToString_Bin	//!< @alias{Float_ToString_Bin}
#define c_f32tostrbin	F32_ToString_Bin	//!< @alias{F32_ToString_Bin}
#define c_f64tostrbin	F64_ToString_Bin	//!< @alias{F64_ToString_Bin}
#define c_f80tostrbin	F80_ToString_Bin	//!< @alias{F80_ToString_Bin}
#define c_f128tostrbin	F128_ToString_Bin	//!< @alias{F128_ToString_Bin}
//!@}



// TODO Float_ToString_Base()



/*
** ************************************************************************** *|
**                         String-to-Float Conversions                        *|
** ************************************************************************** *|
*/

//! Parse a floating-point number from the given string (can be decimal/exponential/hexdecimal)
/*!
**	@isostd{C89,https://en.cppreference.com/w/c/string/byte/atof}
**
**	@param	str	The string to parse a floating-point value from.
**	@returns
**	A floating-point value parsed from the given `str`
*/
//!@{
#define					Float_FromString	CONCAT(FLOAT_TYPE,_FromString)
t_f32					F32_FromString(t_char const* str);
t_f64					F64_FromString(t_char const* str);
#ifdef	__float80
t_f80					F80_FromString(t_char const* str);
#endif
#ifdef	__float128
t_f128					F128_FromString(t_char const* str);
#endif
#define c_strtof		Float_FromString//!< @alias{Float_FromString}
#define c_strtof32		F32_FromString	//!< @alias{F32_FromString}
#define c_strtof64		F64_FromString	//!< @alias{F64_FromString}
#define c_strtof80		F80_FromString	//!< @alias{F80_FromString}
#define c_strtof128		F128_FromString	//!< @alias{F128_FromString}
//!@}



//! Parse the scientific notation of a floating-point number
/*!
**	@nonstd
**
**	@param	str	The string to parse a floating-point value from.
**	@returns
**	A floating-point value parsed from the given `str`
*/
//!@{
#define									Float_FromString_Exp	CONCAT(FLOAT_TYPE,_FromString_Exp)
t_f32									F32_FromString_Exp(t_char const* str);
t_f64									F64_FromString_Exp(t_char const* str);
#ifdef	__float80
t_f80									F80_FromString_Exp(t_char const* str);
#endif
#ifdef	__float128
t_f128									F128_FromString_Exp(t_char const* str);
#endif
#define c_strexptof						Float_FromString_Exp//!< @alias{Float_FromString_Exp}
#define c_strexptof32					F32_FromString_Exp	//!< @alias{F32_FromString_Exp}
#define c_strexptof64					F64_FromString_Exp	//!< @alias{F64_FromString_Exp}
#define c_strexptof80					F80_FromString_Exp	//!< @alias{F80_FromString_Exp}
#define c_strexptof128					F128_FromString_Exp	//!< @alias{F128FromoString_Exp}

#define c_strscitof						Float_FromString_Exp//!< @alias{Float_FromString_Exp}
#define c_strscitof32					F32_FromString_Exp	//!< @alias{F32_FromString_Exp}
#define c_strscitof64					F64_FromString_Exp	//!< @alias{F64_FromString_Exp}
#define c_strscitof80					F80_FromString_Exp	//!< @alias{F80_FromString_Exp}
#define c_strscitof128					F128_FromString_Exp	//!< @alias{F128FromoString_Exp}

#define Float_FromString_Exponential	Float_FromString_Exp//!< @alias{Float_FromString_Exp}
#define Float_FromString_Scientific		Float_FromString_Exp//!< @alias{Float_FromString_Exp}
#define Float_FromString_Sci			Float_FromString_Exp//!< @alias{Float_FromString_Exp}
#define F32_FromString_Exponential		F32_FromString_Exp	//!< @alias{F32_FromString_Exp}
#define F32_FromString_Scientific		F32_FromString_Exp	//!< @alias{F32_FromString_Exp}
#define F32_FromString_Sci				F32_FromString_Exp	//!< @alias{F32_FromString_Exp}
#define F64_FromString_Exponential		F64_FromString_Exp	//!< @alias{F64_FromString_Exp}
#define F64_FromString_Scientific		F64_FromString_Exp	//!< @alias{F64_FromString_Exp}
#define F64_FromString_Sci				F64_FromString_Exp	//!< @alias{F64_FromString_Exp}
#define F80_FromString_Exponential		F80_FromString_Exp	//!< @alias{F80_FromString_Exp}
#define F80_FromString_Scientific		F80_FromString_Exp	//!< @alias{F80_FromString_Exp}
#define F80_FromString_Sci				F80_FromString_Exp	//!< @alias{F80_FromString_Exp}
#define F128_FromString_Exponential		F128_FromString_Exp	//!< @alias{F128FromoString_Exp}
#define F128_FromString_Scientific		F128_FromString_Exp	//!< @alias{F128FromoString_Exp}
#define F128_FromString_Sci				F128_FromString_Exp	//!< @alias{F128FromoString_Exp}
//!@}



//! Parse the decimal representation of a floating-point number
/*!
**	@nonstd
**
**	@param	str	The string to parse a floating-point value from.
**	@returns
**	A floating-point value parsed from the given `str`
*/
//!@{
#define					Float_FromString_Dec	CONCAT(FLOAT_TYPE,_FromString_Dec)
t_f32					F32_FromString_Dec(t_char const* str);
t_f64					F64_FromString_Dec(t_char const* str);
#ifdef	__float80
t_f80					F80_FromString_Dec(t_char const* str);
#endif
#ifdef	__float128
t_f128					F128_FromString_Dec(t_char const* str);
#endif
#define c_strdectof		Float_FromString_Dec//!< @alias{Float_FromString_Dec}
#define c_strdectof32	F32_FromString_Dec	//!< @alias{F32_FromString_Dec}
#define c_strdectof64	F64_FromString_Dec	//!< @alias{F64_FromString_Dec}
#define c_strdectof80	F80_FromString_Dec	//!< @alias{F80_FromString_Dec}
#define c_strdectof128	F128_FromString_Dec	//!< @alias{F128_FromString_Dec}
//!@}



//! Parse the hexadecimal representation of a floating-point number
/*!
**	@nonstd
**
**	@param	str	The string to parse a floating-point value from.
**	@returns
**	A floating-point value parsed from the given `str`
*/
//!@{
#define					Float_FromString_Hex	CONCAT(FLOAT_TYPE,_FromString_Hex)
t_f32					F32_FromString_Hex(t_char const* str);
t_f64					F64_FromString_Hex(t_char const* str);
#ifdef	__float80
t_f80					F80_FromString_Hex(t_char const* str);
#endif
#ifdef	__float128
t_f128					F128_FromString_Hex(t_char const* str);
#endif
#define c_strhextof		Float_FromString_Hex//!< @alias{Float_FromString_Hex}
#define c_strhextof32	F32_FromString_Hex	//!< @alias{F32_FromString_Hex}
#define c_strhextof64	F64_FromString_Hex	//!< @alias{F64_FromString_Hex}
#define c_strhextof80	F80_FromString_Hex	//!< @alias{F80_FromString_Hex}
#define c_strhextof128	F128_FromString_Hex	//!< @alias{F128_FromString_Hex}
//!@}



//! Parse the binary representation of a floating-point number
/*!
**	@nonstd
**
**	@param	str	The string to parse a floating-point value from.
**	@returns
**	A floating-point value parsed from the given `str`
*/
//!@{
#define					Float_FromString_Bin	CONCAT(TYPE_FLOAT,_FromString_Bin)
t_f32					F32_FromString_Bin(t_char const* str);
t_f64					F64_FromString_Bin(t_char const* str);
#ifdef	__float80
t_f80					F80_FromString_Bin(t_char const* str);
#endif
#ifdef	__float128
t_f128					F128_FromString_Bin(t_char const* str);
#endif
#define c_strbintof		Float_FromString_Bin//!< @alias{Float_FromString_Bin}
#define c_strbintof32	F32_FromString_Bin	//!< @alias{F32_FromString_Bin}
#define c_strbintof64	F64_FromString_Bin	//!< @alias{F64_FromString_Bin}
#define c_strbintof80	F80_FromString_Bin	//!< @alias{F80_FromString_Bin}
#define c_strbintof128	F128_FromString_Bin	//!< @alias{F128_FromString_Bin}
//!@}



// TODO Float_FromString_Base()



#endif

/*! @} */
HEADER_END
#endif
