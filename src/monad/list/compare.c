
#include "libccc/monad/list.h"


_GENERIC()
t_sint	CONCAT(List_Compare,T_NAME)(s_list_T const* list1, s_list_T const* list2, t_sint (*compare)(T item1, T item2))
{
	t_sint	result;

	LIBCONFIG_HANDLE_NULLPOINTER(compare, ERROR)
	if (list1 == list2)
		return (0);
	LIBCONFIG_HANDLE_NULLPOINTER(list1, ERROR)
	LIBCONFIG_HANDLE_NULLPOINTER(list2, ERROR)
	while (list1 && list2)
	{
		result = compare(list1->item, list2->item);
		if (result != 0)
			return (result);
		list1 = list1->next;
		list2 = list2->next;
	}
	return (0);
}


_GENERIC()
t_sint	CONCAT(List_Compare_N,T_NAME)(s_list_T const* list1, s_list_T const* list2, t_sint (*compare)(T item1, T item2), t_uint n)
{
	t_sint	result;

	LIBCONFIG_HANDLE_NULLPOINTER(compare, ERROR)
	if (list1 == list2)
		return (0);
	LIBCONFIG_HANDLE_NULLPOINTER(list1, ERROR)
	LIBCONFIG_HANDLE_NULLPOINTER(list2, ERROR)
	while (list1 && list2 && n--)
	{
		result = compare(list1->item, list2->item);
		if (result != 0)
			return (result);
		list1 = list1->next;
		list2 = list2->next;
	}
	return (0);
}
