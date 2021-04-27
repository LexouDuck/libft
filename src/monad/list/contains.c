
#include "libccc/monad/list.h"


_GENERIC()
t_bool	CONCAT(List_Contains,T_NAME)(s_list_T const* list, T item)
{
	LIBCONFIG_HANDLE_NULLPOINTER(NULL, list)
	while (list)
	{
		if (T_EQUALS(list->item, item))
			return (TRUE);
		list = list->next;
	}
	return (FALSE);
}
