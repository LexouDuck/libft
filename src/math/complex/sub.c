
#include "libccc/math/complex.h"

#include LIBCONFIG_ERROR_INCLUDE



s_complex	Complex_Subtract(s_complex const* z1, s_complex const* z2)
{
	s_complex result;

	result.re = z1->re - z2->re;
	result.im = z1->im - z2->im;
	return (result);
}
