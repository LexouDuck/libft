
#include "libft_memory.h"



void	*ft_memchr(void const *ptr, t_u8 byte, t_size n)
{
	t_u8	*result;
	t_size	i;

#if LIBFTCONFIG_HANDLE_NULLPOINTERS
	if (ptr == NULL)
		return (NULL);
#endif
	byte &= 0xFF;
	result = (t_u8 *)ptr;
	i = 0;
	while (i < n)
	{
		if (result[i] == byte)
			return (result + i);
		++i;
	}
	return (NULL);
}
/*
**	if (ptr == NULL)
**		return (NULL);
*/
