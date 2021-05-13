
#include "libccc/memory.h"
#include "libccc/string.h"



t_char*	String_Sub(t_char const* str, t_size index, t_size n)
{
	t_char*	result;
	t_size	length;
	t_size	i;

	LIBCONFIG_HANDLE_NULLPOINTER(NULL, str)
	length = 0;
	while (str[length])
		++length;
	LIBCONFIG_HANDLE_LENGTH2LARGE(NULL, index, length)
	LIBCONFIG_HANDLE_LENGTH2LARGE(NULL, index + n, length)
	if (!(result = (t_char*)Memory_Alloc(n + sizeof(""))))
		return (NULL);
	i = 0;
	while (i < n)
	{
		result[i] = str[index + i];
		++i;
	}
	result[i] = '\0';
	return (result);
}
