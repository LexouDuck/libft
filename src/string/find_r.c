
#include "libccc/string.h"

#if LIBCONFIG_USE_STD_FUNCTIONS_ALWAYS
#include <string.h>
#endif

#include LIBCONFIG_ERROR_INCLUDE



#if LIBCONFIG_USE_STD_FUNCTIONS_ALWAYS
inline
t_char*	String_Find_R_Char(t_char const* str, t_char c)
{ return (strchr(str, c)); }
#else
t_char*	String_Find_R_Char(t_char const* str, t_char c)
{
	t_size	i;

	HANDLE_ERROR(NULLPOINTER, (str == NULL), return (NULL);)
	i = 0;
	while (str[i])
		++i;
	if (c == '\0')
		return ((t_char*)(str + i));
	else if (i == 0)
		return (NULL);
	while (i--)
	{
		if (str[i] == c)
			return ((t_char*)str + i);
	}
	HANDLE_ERROR_SF(NOTFOUND, (TRUE), return (NULL);,
		", no char '%c' in string \"%s\"", c, str)
}
#endif

inline
t_sintmax	String_IndexOf_R_Char(t_char const* str, t_char c)
{
	t_char* result = String_Find_R_Char(str, c);
	HANDLE_ERROR(NOTFOUND, (result == NULL), return (ERROR);)
	return (result - str);
}



t_char*	String_Find_R_Charset(t_char const* str, t_char const* charset)
{
	t_size	i;

	HANDLE_ERROR(NULLPOINTER, (str == NULL), return (NULL);)
	HANDLE_ERROR(NULLPOINTER, (charset == NULL), return (NULL);)
	i = 0;
	while (str[i])
		++i;
	if (charset[0] == '\0')
		return ((t_char*)(str + i));
	else if (i == 0)
		return (NULL);
	while (i--)
	{
		for (t_size j = 0; charset[j]; ++j)
		{
			if (str[i] == charset[j])
				return ((t_char*)str + i);
		}
	}
	HANDLE_ERROR_SF(NOTFOUND, (TRUE), return (NULL);,
		", no char from charset \"%s\" in string \"%s\"", charset, str)
}

inline
t_sintmax	String_IndexOf_R_Charset(t_char const* str, t_char const* charset)
{
	t_char* result = String_Find_R_Charset(str, charset);
	HANDLE_ERROR(NOTFOUND, (result == NULL), return (ERROR);)
	return (result - str);
}



t_char*	String_Find_R_String(t_char const* str, t_char const* query)
{
	t_size	length;
	t_size	match;
	t_size	i;

	HANDLE_ERROR(NULLPOINTER, (str == NULL), return (NULL);)
	HANDLE_ERROR(NULLPOINTER, (query == NULL), return (NULL);)
	length = 0;
	while (query[length])
		++length;
	if (length == 0)
		return (NULL);
	i = String_Length(str);
	while (i--)
	{
		match = 0;
		while (match < length)
		{
			if (str[i + match] == query[match])
				++match;
			else
				break;
		}
		if (match == length)
			return ((t_char*)str + i);
	}
	HANDLE_ERROR_SF(NOTFOUND, (TRUE), return (NULL);,
		", no string \"%s\" in string \"%s\"", query, str)
}

inline
t_sintmax	String_IndexOf_R_String(t_char const* str, t_char const* query)
{
	t_char* result = String_Find_R_String(str, query);
	HANDLE_ERROR(NOTFOUND, (result == NULL), return (ERROR);)
	return (result - str);
}
