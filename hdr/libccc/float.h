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
**	@isostd{https://en.cppreference.com/w/c/language/arithmetic_types#Real_floating_types}
**	@isostd{https://en.cppreference.com/w/c/language/floating_constant}
**	@isostd{https://en.cppreference.com/w/c/numeric/math}
**	@isostd{https://en.cppreference.com/w/c/numeric/fenv}
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

#include "libccc_config.h"
#include "libccc_define.h"

#include "libccc/bool.h"
#include "libccc/int.h"
#include "libccc/fixed.h"

HEADER_CPP

/*
** ************************************************************************** *|
**                                 Definitions                                *|
** ************************************************************************** *|
*/



//! Primitive type: 32-bit 'single precision' IEEE-754 floating-point numbers
/*!
**	@isostd{https://en.cppreference.com/w/c/language/arithmetic_types#Real_floating_types}
**
**	- https://en.wikipedia.org/wiki/Single-precision_floating-point_format
*/
typedef float		t_f32;
TYPEDEF_ALIAS(		t_f32,	FLOAT_32,	PRIMITIVE)

//! Primitive type: 64-bit 'double precision' IEEE-754 floating-point numbers
/*!
**	@isostd{https://en.cppreference.com/w/c/language/arithmetic_types#Real_floating_types}
**
**	- https://en.wikipedia.org/wiki/Double-precision_floating-point_format
*/
typedef double		t_f64;
TYPEDEF_ALIAS(		t_f64,	FLOAT_64,	PRIMITIVE)

#ifdef	__float80
//! Primitive type: 80-bit 'extended precision' MacOS floating-point numbers (only certain platforms)
/*!
**	@isostd{https://gcc.gnu.org/onlinedocs/gcc/Floating-Types.html}
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
**	@isostd{https://gcc.gnu.org/onlinedocs/gcc/Floating-Types.html}
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
**	@isostd{https://gcc.gnu.org/onlinedocs/gcc/Floating-Types.html}
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
**	@isostd{https://en.cppreference.com/w/c/language/arithmetic_types#Real_floating_types}
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



/*!
**	This very small float is typically used to compare two floating-point values.
**	Floating point equality checks aren't the most dependable kind of operation,
**	so it's often better to do `(ABS(x - y) <= FLOAT_APPROX)` to check for equality.
**	You can use the Float_EqualsApprox() functions for this purpose.
**	@see not to be confused with #FLOAT_EPSILON
*/
#define FLOAT_APPROX	LIBCONFIG_FLOAT_APPROX



//! The floating-point "not a number" value.
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/nan}
**	@isostd{https://en.cppreference.com/w/c/numeric/math/NAN}
*/
#ifndef NAN
#define NAN			(0. / 0.)
#endif

#ifndef NOTNUMBER
#define NOTNUMBER	NAN
#endif

//! Checks if the given 'x' has a "not a number" value.
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/isnan}
**
**	Also, define isnan() for ANSI C compatibility, if needed.
*/
#ifndef isnan
#define isnan(X)	(X != X)
#endif

#ifndef IS_NAN
#define IS_NAN(X)		isnan(X)
#endif

#ifndef IS_NOTNUMBER
#define IS_NOTNUMBER(X)	isnan(X)
#endif



//! The floating-point infinity value (use `-INF` for negative)
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/INFINITY}
*/
#ifndef INF
#define INF			(1. / 0.)
#endif
#ifndef INFINITY
#define INFINITY	INF
#endif

//! Checks if the given 'x' is either +INFINITY or -INFINITY
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/isinf}
**
**	Also, define isinf() for ANSI C compatibility, if needed.
*/
#ifndef isinf
#define isinf(X)	(isnan((X - X)) && !isnan(X))
#endif
#ifndef IS_INF
#define IS_INF(X)		isinf(X)
#endif
#ifndef IS_INFINITY
#define IS_INFINITY(X)	isinf(X)
#endif



// TODO add HUGE and TINY min/max value macros
/*!
**	@isostd{https://en.cppreference.com/w/c/numeric/math/HUGE_VAL}
**	@isostd{https://en.cppreference.com/w/c/types/limits#Limits_of_floating_point_types}
*/



//	TODO document this
#define SAMPLE_NB		(1024)



//! This macro sets the rounding behavior for floating-point number operations.
/*!
**	Rounding behavior for floating-point types. Possible values are:
**	-1 undetermined
**	 0 towards zero
**	 1 to nearest
**	 2 toward positive infinity
**	 3 toward negative infinity
*/
#ifndef FLT_ROUNDS
#define FLT_ROUNDS	1
#endif

//! This macro sets the floating-point expression evaluation method (ie: how floats are operated upon).
/*!
**	The floating-point evaluation method. Possible values are:
**	-1  indeterminate
**	 0  evaluate all operations and constants, whose semantic type has at most the range and precision of float, to the range and precision of float;
**	    evaluate all other operations and constants to the range and precision of the semantic type.
**	 1  evaluate all operations and constants, whose semantic type has at most the range and precision of double, to the range and precision of double;
**	    evaluate all other operations and constants to the range and precision of the semantic type.
**	 2  evaluate all operations and constants, whose semantic type has at most the range and precision of long double, to the range and precision of long double;
**	    evaluate all other operations and constants to the range and precision of the semantic type.
**	 N  where _FloatN  is a supported interchange floating type:
**	    evaluate all operations and constants, whose semantic type has at most the range and precision of the _FloatN type, to the range and precision of the _FloatN type;
**	    evaluate all other operations and constants to the range and precision of the semantic type.
**	 N + 1, where _FloatNx is a supported extended floating type:
**	    evaluate operations and constants, whose semantic type has at most the range and precision of the _FloatNx type, to the range and precision of the _FloatNx type;
**	    evaluate all other operations and constants to the range and precision of the semantic type.
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
	t_float		value_float;
#if (LIBCONFIG_BITS_FLOAT == 32)
	t_s32
#elif (LIBCONFIG_BITS_FLOAT == 64)
	t_s64
#else
	t_s64[2]
#endif
	value_int;
}				u_float_cast;



/*
** ************************************************************************** *|
**                        Floating-point number functions                     *|
** ************************************************************************** *|
*/



//! Creates a float value from its individual parts.
/*!
**	@param	mantissa	The significant digits of this floating-point number
**	@param	exponent	The (decimal) exponent of this floating-point number, is applied like: `number * pow(10, exponent)`
**	@returns the floating-point number value resulting from putting together
**		the given`mantissa` and `exponent` parts.
*/
//!@{
#define					Float	CONCAT(FLOAT_TYPE,)
#define c_float			Float 	//!< @alias{Float}

t_f32					F32(t_fixed mantissa, t_sint exponent);
#define c_f32			F32 	//!< @alias{F32}

t_f64					F64(t_fixed mantissa, t_sint exponent);
#define c_f64			F64 	//!< @alias{F64}

#ifdef	__float80
t_f80					F80(t_fixed mantissa, t_sint exponent);
#define c_f80			F80 	//!< @alias{F80}
#endif
#ifdef	__float128
t_f128					F128(t_fixed mantissa, t_sint exponent);
#define c_f128			F128 	//!< @alias{F128}
#endif
//!@}



/*
** ************************************************************************** *|
**                       Floating-point basic operators                       *|
** ************************************************************************** *|
*/



//! Performs an addition with the 2 given floating-point values (operator: `+`)
//!@{
#define					Float_Add	CONCAT(FLOAT_TYPE,_Add)
#define c_fadd			Float_Add	//!< @alias{Float_Add}

t_f32					F32_Add(t_f32 number1, t_f32 number2);
#define c_f32add		F32_Add 	//!< @alias{F32_Add}

t_f64					F64_Add(t_f64 number1, t_f64 number2);
#define c_f64add		F64_Add 	//!< @alias{F64_Add}

#ifdef	__float80
t_f80					F80_Add(t_f80 number1, t_f80 number2);
#define c_f80add		F80_Add 	//!< @alias{F80_Add}
#endif
#ifdef	__float128
t_f128					F128_Add(t_f128 number1, t_f128 number2);
#define c_f128add		F128_Add	//!< @alias{F128_Add}
#endif
//!@}



//! Performs a subtraction with the 2 given floating-point values (operator: `-`)
//!@{
#define					Float_Sub	CONCAT(FLOAT_TYPE,_Sub)
#define c_fsub			Float_Sub	//!< @alias{Float_Sub}
#define Float_Subtract	Float_Sub	//!< @alias{Float_Sub}

t_f32					F32_Sub(t_f32 number1, t_f32 number2);
#define c_f32sub		F32_Sub 	//!< @alias{F32_Sub}
#define F32_Subtract	F32_Sub 	//!< @alias{F32_Sub}

t_f64					F64_Sub(t_f64 number1, t_f64 number2);
#define c_f64sub		F64_Sub 	//!< @alias{F64_Sub}
#define F64_Subtract	F64_Sub 	//!< @alias{F64_Sub}

#ifdef	__float80
t_f80					F80_Sub(t_f80 number1, t_f80 number2);
#define c_f80sub		F80_Sub 	//!< @alias{F80_Sub}
#define F80_Subtract	F80_Sub 	//!< @alias{F80_Sub}
#endif
#ifdef	__float128
t_f128					F128_Sub(t_f128 number1, t_f128 number2);
#define c_f128sub		F128_Sub	//!< @alias{F128_Sub}
#define F128_Subtract	F128_Sub	//!< @alias{F128_Sub}
#endif
//!@}



//! Performs a multiplication with the 2 given floating-point values (operator: `*`)
//!@{
#define					Float_Mul	CONCAT(FLOAT_TYPE,_Mul)
#define c_fmul			Float_Mul	//!< @alias{Float_Mul}
#define Float_Multiply	Float_Mul	//!< @alias{Float_Mul}

t_f32					F32_Mul(t_f32 number1, t_f32 number2);
#define c_f32mul		F32_Mul 	//!< @alias{F32_Mul}
#define F32_Multiply	F32_Mul 	//!< @alias{F32_Mul}

t_f64					F64_Mul(t_f64 number1, t_f64 number2);
#define c_f64mul		F64_Mul 	//!< @alias{F64_Mul}
#define F64_Multiply	F64_Mul 	//!< @alias{F64_Mul}

#ifdef	__float80
t_f80					F80_Mul(t_f80 number1, t_f80 number2);
#define c_f80mul		F80_Mul 	//!< @alias{F80_Mul}
#define F80_Multiply	F80_Mul 	//!< @alias{F80_Mul}
#endif
#ifdef	__float128
t_f128					F128_Mul(t_f128 number1, t_f128 number2);
#define c_f128mul		F128_Mul	//!< @alias{F128_Mul}
#define F128_Multiply	F128_Mul	//!< @alias{F128_Mul}
#endif
//!@}



//! Performs a division with the 2 given floating-point values (operator: `/`)
//!@{
#define					Float_Div	CONCAT(FLOAT_TYPE,_Div)
#define c_fdiv			Float_Div	//!< @alias{Float_Div}
#define Float_Divide	Float_Div	//!< @alias{Float_Div}

t_f32					F32_Div(t_f32 number1, t_f32 number2);
#define c_f32div		F32_Div 	//!< @alias{F32_Div}
#define F32_Divide		F32_Div 	//!< @alias{F32_Div}

t_f64					F64_Div(t_f64 number1, t_f64 number2);
#define c_f64div		F64_Div 	//!< @alias{F64_Div}
#define F64_Divide		F64_Div 	//!< @alias{F64_Div}

#ifdef	__float80
t_f80					F80_Div(t_f80 number1, t_f80 number2);
#define c_f80div		F80_Div 	//!< @alias{F80_Div}
#define F80_Divide		F80_Div 	//!< @alias{F80_Div}
#endif
#ifdef	__float128
t_f128					F128_Div(t_f128 number1, t_f128 number2);
#define c_f128div		F128_Div	//!< @alias{F128_Div}
#define F128_Divide		F128_Div	//!< @alias{F128_Div}
#endif
//!@}



/*
** ************************************************************************** *|
**                       Floating-point logic operators                       *|
** ************************************************************************** *|
*/



//! Returns 1(TRUE) if the 2 given floating-point values are exactly equal (operator: `==`)
/*!
**	@param	number1	The first value to check for (exact) equality
**	@param	number2	The second value to check for (exact) equality
**	@returns 1(TRUE) if the 2 given floating-point values are exactly equal, 0(FALSE) otherwise.
*/
//!@{
#define					Float_Equals	CONCAT(FLOAT_TYPE,_Equals)
#define c_fequ			Float_Equals	//!< @alias{Float_Equals}

t_bool					F32_Equals(t_f32 number1, t_f32 number2);
#define c_f32equ		F32_Equals	//!< @alias{F32_Equals}

t_bool					F64_Equals(t_f64 number1, t_f64 number2);
#define c_f64equ		F64_Equals	//!< @alias{F64_Equals}

#ifdef	__float80
t_bool					F80_Equals(t_f80 number1, t_f80 number2);
#define c_f80equ		F80_Equals	//!< @alias{F80_Equals}
#endif
#ifdef	__float128
t_bool					F128_Equals(t_f128 number1, t_f128 number2);
#define c_f128equ		F128_Equals	//!< @alias{F128_Equals}
#endif
//!@}



//! Returns 1(TRUE) if the 2 given floating-point values are close enough to be considered equal (operator: `=~`, using FLOAT_APPROX)
/*!
**	@param	number1	The first value to check for (approximate) equality
**	@param	number2	The second value to check for (approximate) equality
**	@returns 1(TRUE) if the 2 given floating-point values are close enough to be considered equal
**		(using FLOAT_BIAS as a comparison margin), or 0(FALSE) otherwise.
*/
//!@{
#define					Float_EqualsApprox	CONCAT(FLOAT_TYPE,_EqualsApprox)
#define c_fequa			Float_EqualsApprox	//!< @alias{Float_EqualsApprox}

t_bool					F32_EqualsApprox(t_f32 number1, t_f32 number2);
#define c_f32equa		F32_EqualsApprox	//!< @alias{F32_EqualsApprox}

t_bool					F64_EqualsApprox(t_f64 number1, t_f64 number2);
#define c_f64equa		F64_EqualsApprox	//!< @alias{F64_EqualsApprox}

#ifdef	__float80
t_bool					F80_EqualsApprox(t_f80 number1, t_f80 number2);
#define c_f80equa		F80_EqualsApprox	//!< @alias{F80_EqualsApprox}
#endif
#ifdef	__float128
t_bool					F128_EqualsApprox(t_f128 number1, t_f128 number2);
#define c_f128equa		F128_EqualsApprox	//!< @alias{F128_EqualsApprox}
#endif
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
**	@param	number		The number to convert to a string
**	@param	precision	The amount of digits to print after the decimal point
**	@returns a newly llocated string representation of the given `number`, with `precision` digits
*/
//!@{
#define					Float_ToString	CONCAT(FLOAT_TYPE,_ToString)
#define c_ftostr		Float_ToString	//!< @alias{Float_ToString}

_MALLOC()	char*		F32_ToString(t_f32 number, t_u8 precision);
#define c_f32tostr		F32_ToString	//!< @alias{F32_ToString}

_MALLOC()	char*		F64_ToString(t_f64 number, t_u8 precision);
#define c_f64tostr		F64_ToString	//!< @alias{F64_ToString}

#ifdef	__float80
_MALLOC()	char*		F80_ToString(t_f80 number, t_u8 precision);
#define c_f80tostr		F80_ToString	//!< @alias{F80_ToString}
#endif
#ifdef	__float128
_MALLOC()	char*		F128_ToString(t_f128 number, t_u8 precision);
#define c_f128tostr		F128_ToString	//!< @alias{F128_ToString}
#endif
//!@}



//! Get the string scientific notation of a floating-point number, with 'precision' fractional digits
/*!
**	@param	number		The number to convert to a string
**	@param	precision	The amount of digits to print after the decimal point
**	@returns a newly llocated string representation of the given `number`, with `precision` digits
*/
//!@{
#define								Float_ToString_Exp	CONCAT(FLOAT_TYPE,_ToString_Exp)
#define c_ftostrexp					Float_ToString_Exp //!< @alias{Float_ToString_Exp}
#define c_ftostrsci					Float_ToString_Exp //!< @alias{Float_ToString_Exp}
#define Float_ToString_Exponential	Float_ToString_Exp //!< @alias{Float_ToString_Exp}
#define Float_ToString_Sci			Float_ToString_Exp //!< @alias{Float_ToString_Exp}
#define Float_ToString_Scientific	Float_ToString_Exp //!< @alias{Float_ToString_Exp}

_MALLOC()	char*					F32_ToString_Exp(t_f32 number, t_u8 precision);
#define c_f32tostrexp				F32_ToString_Exp //!< @alias{F32_ToString_Exp}
#define c_f32tostrsci				F32_ToString_Exp //!< @alias{F32_ToString_Exp}
#define F32_ToString_Exponential	F32_ToString_Exp //!< @alias{F32_ToString_Exp}
#define F32_ToString_Sci			F32_ToString_Exp //!< @alias{F32_ToString_Exp}
#define F32_ToString_Scientific		F32_ToString_Exp //!< @alias{F32_ToString_Exp}

_MALLOC()	char*					F64_ToString_Exp(t_f64 number, t_u8 precision);
#define c_f64tostrexp				F64_ToString_Exp //!< @alias{F64_ToString_Exp}
#define c_f64tostrsci				F64_ToString_Exp //!< @alias{F64_ToString_Exp}
#define F64_ToString_Exponential	F64_ToString_Exp //!< @alias{F64_ToString_Exp}
#define F64_ToString_Sci			F64_ToString_Exp //!< @alias{F64_ToString_Exp}
#define F64_ToString_Scientific		F64_ToString_Exp //!< @alias{F64_ToString_Exp}

#ifdef	__float80
_MALLOC()	char*					F80_ToString_Exp(t_f80 number, t_u8 precision);
#define c_f80tostrexp				F80_ToString_Exp //!< @alias{F80_ToString_Exp}
#define c_f80tostrsci				F80_ToString_Exp //!< @alias{F80_ToString_Exp}
#define F80_ToString_Exponential	F80_ToString_Exp //!< @alias{F80_ToString_Exp}
#define F80_ToString_Sci			F80_ToString_Exp //!< @alias{F80_ToString_Exp}
#define F80_ToString_Scientific		F80_ToString_Exp //!< @alias{F80_ToString_Exp}
#endif
#ifdef	__float128
_MALLOC()	char*					F128_ToString_Exp(t_f128 number, t_u8 precision);
#define c_f128tostrexp				F128_ToString_Exp //!< @alias{F128_ToString_Exp}
#define c_f128tostrsci				F128_ToString_Exp //!< @alias{F128_ToString_Exp}
#define F128_ToString_Exponential	F128_ToString_Exp //!< @alias{F128_ToString_Exp}
#define F128_ToString_Sci			F128_ToString_Exp //!< @alias{F128_ToString_Exp}
#define F128_ToString_Scientific	F128_ToString_Exp //!< @alias{F128_ToString_Exp}
#endif
//!@}



//! Get the string decimal representation of a floating-point number, with 'precision' fractional digits
/*!
**	@param	number		The number to convert to a string
**	@param	precision	The amount of digits to print after the decimal point
**	@returns a newly llocated string representation of the given `number`, with `precision` digits
*/
//!@{
#define								Float_ToString_Dec	CONCAT(FLOAT_TYPE,_ToString_Dec)
#define c_ftostrdec					Float_ToString_Dec //!< @alias{Float_ToString_Dec}
#define Float_ToString_Decimal		Float_ToString_Dec //!< @alias{Float_ToString_Dec}

_MALLOC()	char*					F32_ToString_Dec(t_f32 number, t_u8 precision);
#define c_f32tostrdec				F32_ToString_Dec //!< @alias{F32_ToString_Dec}
#define F32_ToString_Decimal		F32_ToString_Dec //!< @alias{F32_ToString_Dec}

_MALLOC()	char*					F64_ToString_Dec(t_f64 number, t_u8 precision);
#define c_f64tostrdec				F64_ToString_Dec //!< @alias{F64_ToString_Dec}
#define F64_ToString_Decimal		F64_ToString_Dec //!< @alias{F64_ToString_Dec}

#ifdef	__float80
_MALLOC()	char*					F80_ToString_Dec(t_f80 number, t_u8 precision);
#define c_f80tostrdec				F80_ToString_Dec //!< @alias{F80_ToString_Dec}
#define F80_ToString_Decimal		F80_ToString_Dec //!< @alias{F80_ToString_Dec}
#endif
#ifdef	__float128
_MALLOC()	char*					F128_ToString_Dec(t_f128 number, t_u8 precision);
#define c_f128tostrdec				F128_ToString_Dec //!< @alias{F128_ToString_Dec}
#define F128_ToString_Decimal		F128_ToString_Dec //!< @alias{F128_ToString_Dec}
#endif
//!@}



//! Get the string hexadecimal representation of a floating-point number, with 'precision' fractional digits (if 0, max precision)
/*!
**	@param	number		The number to convert to a string
**	@param	precision	The amount of digits to print after the decimal point
**	@returns a newly llocated string representation of the given `number`, with `precision` digits
*/
//!@{
#define								Float_ToString_Hex	CONCAT(FLOAT_TYPE,_ToString_Hex)
#define c_ftostrhex					Float_ToString_Hex //!< @alias{Float_ToString_Hex}
#define Float_ToString_Hexadecimal	Float_ToString_Hex //!< @alias{Float_ToString_Hex}

_MALLOC()	char*					F32_ToString_Hex(t_f32 number, t_u8 precision);
#define c_f32tostrhex				F32_ToString_Hex //!< @alias{F32_ToString_Hex}
#define F32_ToString_Hexadecimal	F32_ToString_Hex //!< @alias{F32_ToString_Hex}

_MALLOC()	char*					F64_ToString_Hex(t_f64 number, t_u8 precision);
#define c_f64tostrhex				F64_ToString_Hex //!< @alias{F64_ToString_Hex}
#define F64_ToString_Hexadecimal	F64_ToString_Hex //!< @alias{F64_ToString_Hex}

#ifdef	__float80
_MALLOC()	char*					F80_ToString_Hex(t_f80 number, t_u8 precision);
#define c_f80tostrhex				F80_ToString_Hex //!< @alias{F80_ToString_Hex}
#define F80_ToString_Hexadecimal	F80_ToString_Hex //!< @alias{F80_ToString_Hex}
#endif
#ifdef	__float128
_MALLOC()	char*					F128_ToString_Hex(t_f128 number, t_u8 precision);
#define c_f128tostrhex				F128_ToString_Hex //!< @alias{F128_ToString_Hex}
#define F128_ToString_Hexadecimal	F128_ToString_Hex //!< @alias{F128_ToString_Hex}
#endif
//!@}



//! Get the string binary representation of a floating-point number, with 'precision' fractional digits (if 0, max precision)
/*!
**	@param	number		The number to convert to a string
**	@param	precision	The amount of digits to print after the decimal point
**	@returns a newly llocated string representation of the given `number`, with `precision` digits
*/
//!@{
#define								Float_ToString_Bin	CONCAT(FLOAT_TYPE,_ToString_Bin)
#define c_ftostrbin					Float_ToString_Bin //!< @alias{Float_ToString_Bin}
#define Float_ToString_Binary		Float_ToString_Bin //!< @alias{Float_ToString_Bin}

_MALLOC()	char*					F32_ToString_Bin(t_f32 number, t_u8 precision);
#define c_f32tostrbin				F32_ToString_Bin //!< @alias{F32_ToString_Bin}
#define F32_ToString_Binary			F32_ToString_Bin //!< @alias{F32_ToString_Bin}

_MALLOC()	char*					F64_ToString_Bin(t_f64 number, t_u8 precision);
#define c_f64tostrbin				F64_ToString_Bin //!< @alias{F64_ToString_Bin}
#define F64_ToString_Binary			F64_ToString_Bin //!< @alias{F64_ToString_Bin}

#ifdef	__float80
_MALLOC()	char*					F80_ToString_Bin(t_f80 number, t_u8 precision);
#define c_f80tostrbin				F80_ToString_Bin //!< @alias{F80_ToString_Bin}
#define F80_ToString_Binary			F80_ToString_Bin //!< @alias{F80_ToString_Bin}
#endif
#ifdef	__float128
_MALLOC()	char*					F128_ToString_Bin(t_f128 number, t_u8 precision);
#define c_f128tostrbin				F128_ToString_Bin //!< @alias{F128_ToString_Bin}
#define F128_ToString_Binary		F128_ToString_Bin //!< @alias{F128_ToString_Bin}
#endif
//!@}



/*
** ************************************************************************** *|
**                         String-to-Float Conversions                        *|
** ************************************************************************** *|
*/



//! Parse a floating-point number from the given string (can be decimal/exponential/hexdecimal)
/*!
**	@param	str	The string to parse a floating-point value from.
**	@returns A converted floating-point value from the given `str`
*/
//!@{
#define					Float_FromString	CONCAT(FLOAT_TYPE,_FromString)
#define c_strtof		Float_FromString //!< @alias{Float_FromString}

t_f32					F32_FromString(char const* str);
#define c_strtof32		F32_FromString //!< @alias{F32_FromString}

t_f64					F64_FromString(char const* str);
#define c_strtof64		F64_FromString //!< @alias{F64_FromString}

#ifdef	__float80
t_f80					F80_FromString(char const* str);
#define c_strtof80		F80_FromString //!< @alias{F80_FromString}
#endif
#ifdef	__float128
t_f128					F128_FromString(char const* str);
#define c_strtof128		F128_FromString //!< @alias{F128_FromString}
#endif
//!@}



//! Parse the scientific notation of a floating-point number
/*!
**	@param	str	The string to parse a floating-point value from.
**	@returns A converted floating-point value from the given `str`
*/
//!@{
#define									Float_FromString_Exp	CONCAT(FLOAT_TYPE,_FromString_Exp)
#define c_strexptof						Float_FromString_Exp //!< @alias{Float_FromString_Exp}
#define c_strscitof						Float_FromString_Exp //!< @alias{Float_FromString_Exp}
#define Float_FromString_Exponential	Float_FromString_Exp //!< @alias{Float_FromString_Exp}
#define Float_FromString_Sci			Float_FromString_Exp //!< @alias{Float_FromString_Exp}
#define Float_FromString_Scientific		Float_FromString_Exp //!< @alias{Float_FromString_Exp}

t_f32									F32_FromString_Exp(char const* str);
#define c_strexptof32					F32_FromString_Exp //!< @alias{F32_FromString_Exp}
#define c_strscitof32					F32_FromString_Exp //!< @alias{F32_FromString_Exp}
#define F32_FromString_Exponential		F32_FromString_Exp //!< @alias{F32_FromString_Exp}
#define F32_FromString_Sci				F32_FromString_Exp //!< @alias{F32_FromString_Exp}
#define F32_FromString_Scientific		F32_FromString_Exp //!< @alias{F32_FromString_Exp}

t_f64									F64_FromString_Exp(char const* str);
#define c_strexptof64					F64_FromString_Exp //!< @alias{F64_FromString_Exp}
#define c_strscitof64					F64_FromString_Exp //!< @alias{F64_FromString_Exp}
#define F64_FromString_Exponential		F64_FromString_Exp //!< @alias{F64_FromString_Exp}
#define F64_FromString_Sci				F64_FromString_Exp //!< @alias{F64_FromString_Exp}
#define F64_FromString_Scientific		F64_FromString_Exp //!< @alias{F64_FromString_Exp}

#ifdef	__float80
t_f80									F80_FromString_Exp(char const* str);
#define c_strexptof80					F80_FromString_Exp //!< @alias{F80_FromString_Exp}
#define c_strscitof80					F80_FromString_Exp //!< @alias{F80_FromString_Exp}
#define F80_FromString_Exponential		F80_FromString_Exp //!< @alias{F80_FromString_Exp}
#define F80_FromString_Sci				F80_FromString_Exp //!< @alias{F80_FromString_Exp}
#define F80_FromString_Scientific		F80_FromString_Exp //!< @alias{F80_FromString_Exp}
#endif
#ifdef	__float128
t_f128									F128_FromString_Exp(char const* str);
#define c_strexptof128					F128_FromString_Exp //!< @alias{F128FromoString_Exp}
#define c_strscitof128					F128_FromString_Exp //!< @alias{F128FromoString_Exp}
#define F128_FromString_Exponential		F128_FromString_Exp //!< @alias{F128FromoString_Exp}
#define F128_FromString_Sci				F128_FromString_Exp //!< @alias{F128FromoString_Exp}
#define F128_FromString_Scientific		F128_FromString_Exp //!< @alias{F128FromoString_Exp}
#endif
//!@}



//! Parse the decimal representation of a floating-point number
/*!
**	@param	str	The string to parse a floating-point value from.
**	@returns A converted floating-point value from the given `str`
*/
//!@{
#define									Float_FromString_Dec	CONCAT(FLOAT_TYPE,_FromString_Dec)
#define c_strdectof						Float_FromString_Dec	//!< @alias{Float_FromString_Dec}
#define Float_FromString_Decimal		Float_FromString_Dec	//!< @alias{Float_FromString_Dec}

t_f32									F32_FromString_Dec(char const* str);
#define c_strdectof32					F32_FromString_Dec	//!< @alias{F32_FromString_Dec}
#define F32_FromString_Decimal			F32_FromString_Dec	//!< @alias{F32_FromString_Dec}

t_f64									F64_FromString_Dec(char const* str);
#define c_strdectof64					F64_FromString_Dec	//!< @alias{F64_FromString_Dec}
#define F64_FromString_Decimal			F64_FromString_Dec	//!< @alias{F64_FromString_Dec}

#ifdef	__float80
t_f80									F80_FromString_Dec(char const* str);
#define c_strdectof80					F80_FromString_Dec	//!< @alias{F80_FromString_Dec}
#define F80_FromString_Decimal			F80_FromString_Dec	//!< @alias{F80_FromString_Dec}
#endif
#ifdef	__float128
t_f128									F128_FromString_Dec(char const* str);
#define c_strdectof128					F128_FromString_Dec	//!< @alias{F128_FromString_Dec}
#define F128_FromString_Decimal			F128_FromString_Dec	//!< @alias{F128_FromString_Dec}
#endif
//!@}



//! Parse the hexadecimal representation of a floating-point number
/*!
**	@param	str	The string to parse a floating-point value from.
**	@returns A converted floating-point value from the given `str`
*/
//!@{
#define									Float_FromString_Hex	CONCAT(FLOAT_TYPE,_FromString_Hex)
#define c_strhextof						Float_FromString_Hex	//!< @alias{Float_FromString_Hex}
#define Float_FromString_Hexadecimal	Float_FromString_Hex	//!< @alias{Float_FromString_Hex}

t_f32									F32_FromString_Hex(char const* str);
#define c_strhextof32					F32_FromString_Hex	//!< @alias{F32_FromString_Hex}
#define F32_FromString_Hexadecimal		F32_FromString_Hex	//!< @alias{F32_FromString_Hex}

t_f64									F64_FromString_Hex(char const* str);
#define c_strhextof64					F64_FromString_Hex	//!< @alias{F64_FromString_Hex}
#define F64_FromString_Hexadecimal		F64_FromString_Hex	//!< @alias{F64_FromString_Hex}

#ifdef	__float80
t_f80									F80_FromString_Hex(char const* str);
#define c_strhextof80					F80_FromString_Hex	//!< @alias{F80_FromString_Hex}
#define F80_FromString_Hexadecimal		F80_FromString_Hex	//!< @alias{F80_FromString_Hex}
#endif
#ifdef	__float128
t_f128									F128_FromString_Hex(char const* str);
#define c_strhextof128					F128_FromString_Hex	//!< @alias{F128_FromString_Hex}
#define F128_FromString_Hexadecimal		F128_FromString_Hex	//!< @alias{F128_FromString_Hex}
#endif
//!@}



//! Parse the binary representation of a floating-point number
/*!
**	@param	str	The string to parse a floating-point value from.
**	@returns A converted floating-point value from the given `str`
*/
//!@{
#define								Float_FromString_Bin	CONCAT(TYPE_FLOAT,_FromString_Bin)
#define c_strbintof					Float_FromString_Bin	//!< @alias{Float_FromString_Bin}
#define Float_FromString_Binary		Float_FromString_Bin	//!< @alias{Float_FromString_Bin}

t_f32								F32_FromString_Bin(char const* str);
#define c_strbintof32				F32_FromString_Bin	//!< @alias{F32_FromString_Bin}
#define F32_FromString_Binary		F32_FromString_Bin	//!< @alias{F32_FromString_Bin}

t_f64								F64_FromString_Bin(char const* str);
#define c_strbintof64				F64_FromString_Bin	//!< @alias{F64_FromString_Bin}
#define F64_FromString_Binary		F64_FromString_Bin	//!< @alias{F64_FromString_Bin}

#ifdef	__float80
t_f80								F80_FromString_Bin(char const* str);
#define c_strbintof80				F80_FromString_Bin	//!< @alias{F80_FromString_Bin}
#define F80_FromString_Binary		F80_FromString_Bin	//!< @alias{F80_FromString_Bin}
#endif
#ifdef	__float128
t_f128								F128_FromString_Bin(char const* str);
#define c_strbintof128				F128_FromString_Bin	//!< @alias{F128_FromString_Bin}
#define F128_FromString_Binary		F128_FromString_Bin	//!< @alias{F128_FromString_Bin}
#endif
//!@}



/*
** ************************************************************************** *|
**                       Variable-size primitive types                        *|
** ************************************************************************** *|
*/

//! A union storing a floating-point number of any common size/precision
/*!
** These unions are used for certain difficult casting conditions.
** They are used in particular when casting an <stdarg.h> var_arg to the
** appropriate type in c_printf.
*/
typedef union varfloat
{
	t_f32			f;
	t_f64			d;
#ifdef	__float80
	t_f80			e;
#endif
#ifdef	__float128
	t_f128			q;
#endif
}					u_varfloat;



/*! @} */
HEADER_END
#endif
