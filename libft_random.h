/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_random.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: duquesne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2006/06/06 06:06:06 by duquesne          #+#    #+#             */
/*   Updated: 2006/06/06 06:06:06 by duquesne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_RANDOM_H
#define LIBFT_RANDOM_H

/*
** ************************************************************************** *|
**                                   Includes                                 *|
** ************************************************************************** *|
*/

#include "libft.h"

/*
** ************************************************************************** *|
**                                    Macros                                  *|
** ************************************************************************** *|
*/

#define FT_Random_SetSeed(seed)		ft_random_set_seed(seed)
#define FT_Random_RefreshSeed()		ft_random_renew_seed()

#define FT_Random()					ft_random()
#define FT_Random_Pow2n(n)			ft_random_0_to_pow2n(n)
#define FT_Random_n(n)				ft_random_0_to_n(n)
#define FT_RandomInt_a_b(a, b)		ft_random_int_a_to_b(a, b)
#define FT_RandomFloat_0_1()		ft_random_float_0_to_1()
#define FT_RandomFloat_a_b(a, b)	ft_random_float_a_to_b(a, b)



/*
** ************************************************************************** *|
**                                 Definitions                                *|
** ************************************************************************** *|
*/

/*
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

#define DEFAULT_SEED	0x93E21FD5

#define MODULUS		0x7FFFFFFF		
#define CEIL_SQRT_MOD	46341
#define OFFSET			2835



void		ft_random_set_seed(t_u32 seed);
void		ft_random_renew_seed(void);

t_u32		ft_random(void);
t_u32		ft_random_0_to_pow2n(t_u32 n);
t_u32		ft_random_0_to_n(t_u32 n);
t_int		ft_random_int_a_to_b(t_int a, t_int b);
t_float		ft_random_float_0_to_1(void);
t_float		ft_random_float_a_to_b(t_float a, t_float b);

#endif
