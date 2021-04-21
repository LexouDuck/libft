
#include "libccc/memory.h"
#include "libccc/monad/list.h"


_GENERIC()
void	CONCAT(List_Delete,T_NAME)(s_list_T* *a_list)
{
	s_list_T*	next;
	s_list_T*	i;

#if LIBCONFIG_HANDLE_NULLPOINTERS
	if (a_list == NULL)
		return ;
#endif
	i = *a_list;
	while (i)
	{
		next = i->next;
		Memory_Free(i);
		i = next;
	}
	*a_list = NULL;
}
