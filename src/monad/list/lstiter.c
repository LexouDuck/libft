
#include "libccc/monad/list.h"



void	List_Iterate(s_list_T* lst, void (*f)(s_list_T* elem))
{
#if LIBCONFIG_HANDLE_NULLPOINTERS
	if (lst == NULL || f == NULL)
		return ;
#endif
	while (lst)
	{
		f(lst);
		lst = lst->next;
	}
}



void	List_Iterate_I(s_list_T* lst, void (*f)(s_list_T* elem, t_u32 index))
{
	t_u32 i;

#if LIBCONFIG_HANDLE_NULLPOINTERS
	if (lst == NULL || f == NULL)
		return ;
#endif
	i = 0;
	while (lst)
	{
		f(lst, i);
		lst = lst->next;
		++i;
	}
}
