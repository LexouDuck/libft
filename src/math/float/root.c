
#include "libccc/math/math.h"

#include LIBCONFIG_ERROR_INCLUDE



#if (LIBCONFIG_FLOAT_BITS == 80 || LIBCONFIG_FLOAT_BITS == 128)
	#define POWERS_LENGTH	16
#else
	#define POWERS_LENGTH	12
#endif

/*!
**	Some SQRT(2)^n lookup tables for quick newton method initial guess
*/
static
t_float	Float_Root2_2powN(t_s32 n)
{
	static const t_float powers_pos[POWERS_LENGTH] =
	{
		SQRT_2,
		(t_float)0x1.0p+1,
		(t_float)0x1.0p+2,
		(t_float)0x1.0p+4,
		(t_float)0x1.0p+8,
		(t_float)0x1.0p+16,
		(t_float)0x1.0p+32,
		(t_float)0x1.0p+64,
		(t_float)0x1.0p+128,
		(t_float)0x1.0p+256,
		(t_float)0x1.0p+512,
#if (LIBCONFIG_FLOAT_BITS == 80 || LIBCONFIG_FLOAT_BITS == 128)
		(t_float)0x1.0p+1024,
		(t_float)0x1.0p+2048,
		(t_float)0x1.0p+4096,
		(t_float)0x1.0p+8192,
#endif
		INFINITY,
	};
	static const t_float powers_neg[POWERS_LENGTH] =
	{
		INV_SQRT_2,
		(t_float)0x1.0p-1,
		(t_float)0x1.0p-2,
		(t_float)0x1.0p-4,
		(t_float)0x1.0p-8,
		(t_float)0x1.0p-16,
		(t_float)0x1.0p-32,
		(t_float)0x1.0p-64,
		(t_float)0x1.0p-128,
		(t_float)0x1.0p-256,
		(t_float)0x1.0p-512,
#if (LIBCONFIG_FLOAT_BITS == 80 || LIBCONFIG_FLOAT_BITS == 128)
		(t_float)0x1.0p-1024,
		(t_float)0x1.0p-2048,
		(t_float)0x1.0p-4096,
		(t_float)0x1.0p+8192,
#endif
		0.,
	};
	if (n > 0 && (n >> 11))
		return (INFINITY);
	const t_float* powers = powers_pos;
	if (n == 0)
		return (1.);
	else if (n < 0)
	{
		n = -n;
		powers = powers_neg;
	}
	t_float result = 1.;
	if (n & 0x0001) { result *= powers[0x0]; }
	if (n & 0x0002) { result *= powers[0x1]; }
	if (n & 0x0004) { result *= powers[0x2]; }
	if (n & 0x0008) { result *= powers[0x3]; }
	if (n & 0x0010) { result *= powers[0x4]; }
	if (n & 0x0020) { result *= powers[0x5]; }
	if (n & 0x0040) { result *= powers[0x6]; }
	if (n & 0x0080) { result *= powers[0x7]; }
#if (LIBCONFIG_FLOAT_BITS == 64 || LIBCONFIG_FLOAT_BITS == 80 || LIBCONFIG_FLOAT_BITS == 128)
	if (n & 0x0100) { result *= powers[0x8]; }
	if (n & 0x0200) { result *= powers[0x9]; }
	if (n & 0x0400) { result *= powers[0xA]; }
#endif
#if (LIBCONFIG_FLOAT_BITS == 80 || LIBCONFIG_FLOAT_BITS == 128)
	if (n & 0x0800) { result *= powers[0xB]; }
	if (n & 0x1000) { result *= powers[0xC]; }
	if (n & 0x2000) { result *= powers[0xD]; }
	if (n & 0x4000) { result *= powers[0xE]; }
#endif
	return (result);
}



#if LIBCONFIG_USE_STD_MATH
MATH_DECL_REALFUNCTION(Root2, sqrt)
#else
t_float	Float_Root2(t_float x)
{
	HANDLE_ERROR(NANARGUMENT, IS_NAN(x), return (NAN);)
	HANDLE_ERROR(MATHDOMAIN, (x < 0.), return (NAN);)
//	Fast Inverse square root (from the Quake III source code)
	u_float_cast result = { x };
	t_float x_2;

	x_2 = result.value_float * 0.5;
#if (LIBCONFIG_FLOAT_BITS == 32) // magic voodoo constant
	result.value_int = 0x5F375A86 - (result.value_int >> 1);
#elif (LIBCONFIG_FLOAT_BITS == 64) // 64bit magic constant from https://cs.uwaterloo.ca/~m32rober/rsqrt.pdf
	result.value_int = 0x5FE6EB50C7B537A9 - (result.value_int >> 1);
#endif
	// TODO handle extended precision types
	result.value_float = result.value_float * (1.5 - (x_2 * result.value_float * result.value_float)); // 1st iteration
//	result.value_float = result.value_float * (1.5 - (x_2 * result.value_float * result.value_float)); // 2nd iteration, this can be removed
	return (result.value_float * x);
/*
//	Newton derivative approximation by iteration
	static const t_s32	i_max = 4;
	t_s32	i;
	t_float	result;
	t_float	previous;

	if (IS_NAN(x) || x < 0.)
		return (NAN);
	if (x == 0.)
		return (0.);
	if (x == 1.)
		return (1.);
	i = c_getexp(x);
	result = (i < 0 ? 0.75 : 1.25) * Float_Root2_2powN(i);
	previous = INFINITY;
	i = 0;
	while (c_fabs(result - previous) > FLOAT_APPROX)
	{
		previous = result;
		result -= (result * result - x) / (2 * result);
		if (++i == i_max)
			break;
	}
	return (result);
*/
}
#endif



#if LIBCONFIG_USE_STD_MATH
MATH_DECL_REALFUNCTION(Root3, cbrt)
#else
t_float	Float_Root3(t_float x)
{
	HANDLE_ERROR(NANARGUMENT, IS_NAN(x), return (NAN);)
//	Newton derivative approximation by iteration
	static const t_s32	i_max = 4;
	t_s32	i;
	t_float square;
	t_float	result;
	t_float	previous;

	if (x == 0)
		return (0);
	if (c_fabs(x) == 1.)
		return (SIGN(x));
	i = c_getexp(x);
	result = SIGN(x) * (i < 0 ? 0.75 : 1.25) * Float_Root2_2powN(i * 2 / 3);
	previous = INFINITY;
	i = 0;
	while (c_fabs(result - previous) > FLOAT_APPROX)
	{
		previous = result;
		square = result * result;
		result -= (result * square - x) / (3 * square);
		if (++i == i_max)
			break;
	}
	return (result);
}
#endif



t_float	Float_RootN(t_float x, t_u8 n)
{
	HANDLE_ERROR(NANARGUMENT, IS_NAN(x), return (NAN);)
	HANDLE_ERROR(MATHDOMAIN, (n % 2 == 0 && x < 0), return (NAN);)
//	Newton derivative approximation by iteration
	static const t_s32	i_max = 4;
	t_s32	i;
	t_float	result;
	t_float	previous;
	t_float	power;

	if (n == 0)
		return (NAN);
	if (n == 1)
		return (x);
	if (x == 0)
		return (0);
	if (c_fabs(x) == 1.)
		return (SIGN(x));
	i = Float_GetExp2(x);
	result = SIGN(x) * (i < 0 ? 1 : 1.25) * Float_Root2_2powN(i * 2 / (t_s32)n);
	previous = 0.;
	i = 0;
	n -= 1;
	while (c_fabs(result - previous) > FLOAT_APPROX)
	{
		previous = result;
		power = result;
		for (t_u32 j = 1; j < n; ++j)
		{
			power *= result;
		}
		result -= (power * result - x) / ((n + 1) * power);
		if (++i == i_max)
			break;
	}
	return (result);
}
