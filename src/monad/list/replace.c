
#include "libccc/monad/list.h"

#include LIBCONFIG_ERROR_INCLUDE



_GENERIC()
s_list_T*	CONCAT(List_Replace,T_NAME)(s_list_T const* list, T old, T new)
{
	s_list_T*	result = NULL;
	s_list_T*	elem;

	HANDLE_ERROR(NULLPOINTER, (list == NULL), return (NULL);)
	while (list)
	{
		elem = CONCAT(List_Item,T_NAME)(T_EQUALS(list->item, old) ? new : list->item);
		if (elem == NULL)
			break;
		CONCAT(List_Append,T_NAME)(result, elem);
		list = list->next;
	}
	return (result);
}
