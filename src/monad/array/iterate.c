
#include "libccc/monad/array.h"


_GENERIC()
void	CONCAT(Array_Iterate,T_NAME)(s_array_T* array, void (*f)(T item))
{
	t_uint i;

#if LIBCONFIG_HANDLE_NULLPOINTERS
	if (array == NULL || array->items == NULL || f == NULL)
		return;
#endif
	for (i = 0; i < array->length; ++i)
	{
		f(array->items[i]);
	}
}


_GENERIC()
void	CONCAT(Array_Iterate_I,T_NAME)(s_array_T* array, void (*f)(T item, t_uint index))
{
	t_uint i;

#if LIBCONFIG_HANDLE_NULLPOINTERS
	if (array == NULL || array->items == NULL || f == NULL)
		return;
#endif
	for (i = 0; i < array->length; ++i)
	{
		f(array->items[i], i);
	}
}
