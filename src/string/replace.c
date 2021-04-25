
#include "libccc/memory.h"
#include "libccc/string.h"
#include "libccc/stringarray.h"



t_char*		String_Replace_Char(t_char const* str, t_char const old, t_char const new)
{
	t_char*	result;
	t_size	i;

#if LIBCONFIG_HANDLE_NULLPOINTERS
	if (old == new)
		return (String_Duplicate(str));
	if (str == NULL)
		return (NULL);
#endif
	if (old == '\0')
		return (NULL);
	i = 0;
	while (str[i])
		++i;
	if (!(result = (t_char*)Memory_Alloc(i + 1)))
		return (NULL);
	i = 0;
	while (str[i])
	{
		result[i] = (str[i] == old) ? new : str[i];
		++i;
	}
	result[i] = '\0';
	return (result);
}



t_char*		String_Replace_Charset(t_char const* str, t_char const* old, t_char const* new)
{
	t_char*	result;
	t_size	i;
	t_size	j;
	int		c_index;

#if LIBCONFIG_HANDLE_NULLPOINTERS
	if (old == new)
		return (String_Duplicate(str));
	if (str == NULL || old == NULL || new == NULL)
		return (NULL);
#endif
	if (String_Length(old) != String_Length(new))
		return (NULL);
	i = 0;
	while (old[i])
	{
		j = i;
		while (old[++j])
			if (old[i] == old[j])
				return (NULL);
		++i;
	}
	if (!(result = (t_char*)Memory_Alloc(i + 1)))
		return (NULL);
	i = 0;
	while (str[i])
	{
		if ((c_index = String_IndexOf_Char(old, str[i])) >= 0)
			result[i] = new[c_index];
		else
			result[i] = str[i];
		++i;
	}
	result[i] = '\0';
	return (result);
}



t_char*		String_Replace_String(t_char const* str, t_char const* old, t_char const* new)
{
	t_char*	result;
	t_char**	strarr;

#if LIBCONFIG_HANDLE_NULLPOINTERS
	if (old == new)
		return (String_Duplicate(str));
	if (str == NULL || old == NULL || new == NULL)
		return (NULL);
#endif
	strarr = String_Split_String(str, old);
	result = StringArray_Fold((t_char const**)strarr, new);
	StringArray_Delete(&strarr);
	return (result);
}



void		String_Replace_Char_InPlace(t_char* str, t_char const old, t_char const new)
{
	t_size	i;

#if LIBCONFIG_HANDLE_NULLPOINTERS
	if (str == NULL)
		return ;
#endif
	if (old == '\0')
		return ;
	i = 0;
	while (str[i])
	{
		if (str[i] == old)
			str[i] = new;
		++i;
	}
}



void		String_Replace_Charset_InPlace(t_char* str, t_char const* old, t_char const* new)
{
	t_size	i;
	t_size	j;
	int		c_index;

#if LIBCONFIG_HANDLE_NULLPOINTERS
	if (str == NULL || old == NULL || new == NULL)
		return ;
#endif
	if (String_Length(old) != String_Length(new))
		return ;
	i = 0;
	while (old[i])
	{
		j = i;
		while (old[++j])
			if (old[i] == old[j])
				return ;
		++i;
	}
	i = 0;
	while (str[i])
	{
		if ((c_index = String_IndexOf_Char(old, str[i])) >= 0)
			str[i] = new[c_index];
		++i;
	}
}



void	String_Replace_String_InPlace(t_char* *a_str, t_char const* old, t_char const* new)
{
	t_char*	tmp;
	tmp = String_Replace_String(*a_str, old, new);
	String_Delete(a_str);
	*a_str = tmp;
}
