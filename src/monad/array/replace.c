
#include "libccc/memory.h"
#include "libccc/monad/array.h"


_GENERIC()
s_array_T	CONCAT(Array_Replace,T_NAME)(s_array_T const* array, T old, T new)
{
	s_array_T	result = {0};

	LIBCONFIG_HANDLE_NULLPOINTER(result, array)
	LIBCONFIG_HANDLE_NULLPOINTER(result, array->items)
	result.items = (T*)Memory_Duplicate(array->items, sizeof(T) * array->length);
	if (result.items == NULL)
		return (result);
	result.length = array->length;
	for (t_uint i = 0; i < result.length; ++i)
	{
		if (T_EQUALS(result.items[i], old))
			result.items[i] = new;
	}
	return (result);
}
