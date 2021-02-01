
#include "libccc/math/complex.h"
#include "libccc/memory.h"



s_complex*	Complex_New(t_float re, t_float im)
{
	s_complex* result;

	if (!(result = (s_complex*)Memory_Alloc(sizeof(s_complex))))
		return (NULL);
	result->re = re;
	result->im = im;
	return (result);
}