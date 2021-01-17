
#include "libccc_string.h"



void	c_strclr(char *str)
{
	t_size	i;

#if LIBCCCCONFIG_HANDLE_NULLPOINTERS
	if (str == NULL)
		return ;
#endif
	i = 0;
	while (str[i])
	{
		str[i] = '\0';
		++i;
	}
}
