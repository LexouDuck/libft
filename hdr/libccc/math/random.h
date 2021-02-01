/*============================================================================*/
/*                                            ______________________________  */
/*  libccc/math/random.h                     |    __    __  ___      _____  | */
/*                                           |   / /\  / /\/ . |\   /  __|\ | */
/*  https://github.com/LexouDuck/libccc.git  |  / /_/ / / / . <_/  |  /___| | */
/*                                           | /___/\/_/ /___-'\   \____/\  | */
/* Comprehensive|Cross-platform|Customizable | \____/\__/\____-'    \____/  | */
/* This source code follows the MIT License. |______________________________| */
/*                                                                            */
/*============================================================================*/

#ifndef __LIBCCC_MATH_RANDOM_H
#define __LIBCCC_MATH_RANDOM_H
/*! @file libccc/math/random.h
**	This header defines some simple pseudo-random number generator functions.
**	@addtogroup libccc/math/random
**	@{
*/

/*
** ************************************************************************** *|
**                                   Includes                                 *|
** ************************************************************************** *|
*/

#include "libccc.h"

HEADER_CPP

/*
** ************************************************************************** *|
**                                 Definitions                                *|
** ************************************************************************** *|
*/

/*! @file libccc/math/random.h
** Random number generator. Modulus is 2^31
**
** - OFFSET and MODULUS are mutually prime.
** - CEIL_SQRT_MOD % 4 should be = 1 because MODULUS is a multiple of 4
** - For all P prime divisors of MODULUS, a % p = 1
** - OFFSET should be small compared to the two other parameters
** - The bitwise & is applied, which explains the choice of modulus to be
**		2^31 - 1 in implementation (could also be called RAND_MAX)
**
** Basic RNG formula is:
**		new_nb = (CEIL_SQRT_MOD  * old_nb + OFFSET) % MODULUS
*/

//! The default random seed for pseudo-random number generation
#define DEFAULT_SEED	(0x93E21FD5)

//! TODO document this
#define MODULUS			(0x7FFFFFFF)
//! TODO document this
#define CEIL_SQRT_MOD	(46341)
//! TODO document this
#define OFFSET			(2835)



/*
** ************************************************************************** *|
**                               Random Functions                             *|
** ************************************************************************** *|
*/

//! TODO document this
t_u32								Random_Shuffle(t_u32 n);
#define c_random_32bit_shuffle		Random_Shuffle


//! TODO document this
void								Random_SetSeed(t_u32 seed);
#define c_random_set_seed			Random_SetSeed

//! TODO document this
void								Random_RefreshSeed(void);
#define c_random_renew_seed		Random_RefreshSeed


//! TODO document this
t_u32								Random_U32(void);
#define c_random_u32				Random_U32

//! TODO document this
t_u32								Random_U32_Pow2N(t_u32 n);
#define c_random_u32_0_to_pow2n	Random_U32_Pow2N

//! TODO document this
t_u32								Random_U32_0toN(t_u32 n);
#define c_random_u32_0_to_n		Random_U32_0toN

//! TODO document this
t_int								Random_Int_AtoB(t_int a, t_int b);
#define c_random_int_a_to_b		Random_Int_AtoB

//! TODO document this
t_float								Random_Float_0to1(void);
#define c_random_float_0_to_1		Random_Float_0to1

//! TODO document this
t_float								Random_Float_AtoB(t_float a, t_float b);
#define c_random_float_a_to_b		Random_Float_AtoB




/*! @} */
HEADER_END
#endif