
#include <stdarg.h>

#include "libccc/memory.h"
#include "libccc/monad/list.h"



s_list_T*	List_New(t_uint n, ...)
{
	va_list		list;
	T			item;
	s_list_T*	result = NULL;
	s_list_T*	elem;
	s_list_T*	new;

	va_start(list, n);
	if (n == 0)
		return (NULL);
	for (t_uint i = 1; i <= n; ++i)
	{
		item = va_arg(list, T);
		if (!(new = (s_list_T*)Memory_Alloc(sizeof(s_list_T))))
			break;
		new->next = NULL;
		new->item = item;
		if (i == 1)
		{
			elem = new;
			result = new;
		}
		else
		{
			elem->next = new;
			elem = new;
		}
	}
	va_end(list);
	return (result);
}
