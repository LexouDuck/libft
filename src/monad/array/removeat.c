
#include "libccc/memory.h"
#include "libccc/monad/array.h"

#include LIBCONFIG_HANDLE_INCLUDE



_GENERIC()
void	CONCAT(Array_RemoveAt,T_NAME)(s_array_T* array, t_uint index)
{
	T*	result;

	HANDLE_ERROR(NULLPOINTER, (array == NULL), return;)
	HANDLE_ERROR(NULLPOINTER, (array->items == NULL), return;)
	if (array->length == 0 ||
		array->length <= index)
		return;
	result = (T*)Memory_Allocate(sizeof(T) * (array->length - 1));
	HANDLE_ERROR(ALLOCFAILURE, (result == NULL), return;)
	for (t_uint i = 0; i < array->length - 1; ++i)
	{
		if (i < index)
			result[i] = array->items[i];
		else
			result[i] = array->items[i + 1];
	}
	Memory_Free(array->items);
	array->items = result;
	array->length -= 1;
}


_GENERIC()
void	CONCAT(Array_RemoveAt_F,T_NAME)(s_array_T* array, t_uint index, void (*delete)(T))
{
	T*	result;

	HANDLE_ERROR(NULLPOINTER, (array == NULL), return;)
	HANDLE_ERROR(NULLPOINTER, (array->items == NULL), return;)
	if (array->length == 0 ||
		array->length <= index)
		return;
	result = (T*)Memory_Allocate(sizeof(T) * (array->length - 1));
	HANDLE_ERROR(ALLOCFAILURE, (result == NULL), return;)
	for (t_uint i = 0; i < array->length - 1; ++i)
	{
		if (i < index)
			result[i] = array->items[i];
		else if (i == index)
		{
			delete(array->items[i]);
			result[i] = array->items[i + 1];
		}
		else
			result[i] = array->items[i + 1];
	}
	Memory_Free(array->items);
	array->items = result;
	array->length -= 1;
}
