
#include "libft_complex.h"
#include "libft_math.h"



inline t_float	Complex_Abs(s_complex const* z)
{
	return (Math_SqRt((z->re * z->re) + (z->im * z->im)));
}