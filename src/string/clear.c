
#include "libccc/string.h"

#include LIBCONFIG_HANDLE_INCLUDE



void	String_Clear(t_char* str)
{
	t_size	i;

	HANDLE_ERROR(NULLPOINTER, (str == NULL), return;)
	i = 0;
	while (str[i])
	{
		str[i] = '\0';
		++i;
	}
}
