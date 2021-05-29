
#include "libccc/memory.h"

#include LIBCONFIG_HANDLE_INCLUDE



void*	Memory_Move(void* dest, void const* src, t_size n)
{
	t_u8*	result;
	t_u8*	source;
	t_size	i;

	HANDLE_ERROR(NULLPOINTER, (dest == NULL), return (NULL);)
	HANDLE_ERROR(NULLPOINTER, (src == NULL), return (dest);)
	if (dest && dest == src)
		return (dest);
	result = (t_u8*)dest;
	source = (t_u8*)src;
	if (source < result && source + n >= result)
	{
		i = n;
		while (i--)
			result[i] = source[i];
	}
	else
	{
		i = 0;
		while (i < n)
		{
			result[i] = source[i];
			++i;
		}
	}
	return (dest);
}
