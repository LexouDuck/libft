
#include "libccc/memory.h"

#include LIBCONFIG_HANDLE_INCLUDE



int			Memory_Swap(void* ptr1, void* ptr2, t_size size)
{
	t_u8*	p1;
	t_u8*	p2;
	t_size	i;

	HANDLE_ERROR(NULLPOINTER, (ptr1 == NULL), return (ERROR);)
	HANDLE_ERROR(NULLPOINTER, (ptr2 == NULL), return (ERROR);)
	if (ptr1 == ptr2)
		return (OK);
	else if (size == 1 && *(t_u8*)(ptr1) == *(t_u8*)(ptr2))
		return (OK);
	else if (size == 2 && *(t_u16*)(ptr1) == *(t_u16*)(ptr2))
		return (OK);
	else if (size == 4 && *(t_u32*)(ptr1) == *(t_u32*)(ptr2))
		return (OK);
	else if (size == 8 && *(t_u64*)(ptr1) == *(t_u64*)(ptr2))
		return (OK);
	else if (ptr1 && ptr2)
	{
		p1 = ptr1;
		p2 = ptr2;
		i = 0;
		while (i < size)
		{
			p1[i] = (p1[i] ^ p2[i]);
			p2[i] = (p2[i] ^ p1[i]);
			p1[i] = (p1[i] ^ p2[i]);
			++i;
		}
		return (OK);
	}
	return (ERROR);
}
