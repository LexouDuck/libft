
#include "libccc/memory.h"



void*	Memory_Duplicate(const void* ptr, t_size n)
{
	t_u8*	result;
	t_u8*	source;
	t_size	i;

	LIBCONFIG_HANDLE_NULLPOINTER(NULL, ptr)
	if (!(result = (t_u8* )Memory_Alloc(n)))
		return (NULL);
	source = (t_u8* )ptr;
	i = 0;
	while (i < n)
	{
		result[i] = source[i];
		++i;
	}
	return ((void* )result);
}
