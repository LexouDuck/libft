
#include "libccc/memory.h"
#include "libccc/monad/array.h"

#include LIBCONFIG_ERROR_INCLUDE



_GENERIC()
s_array_T	CONCAT(Array_Duplicate,T_NAME)(s_array_T const* array)
{
	s_array_T	result = {0};

	HANDLE_ERROR(NULLPOINTER, (array == NULL), return (result);)
	HANDLE_ERROR(NULLPOINTER, (array->items == NULL), return (result);)
	result.items = (T*)Memory_Duplicate(array->items, sizeof(T) * array->length);
	if (result.items)
		result.length = array->length;
	return (result);
}
