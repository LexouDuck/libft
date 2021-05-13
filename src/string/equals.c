
#include "libccc/char.h"
#include "libccc/string.h"



t_bool	String_Equals(t_char const* str1, t_char const* str2)
{
	t_size i;

	if (str1 == str2)
		return (TRUE);
	LIBCONFIG_HANDLE_NULLPOINTER(FALSE, str1)
	LIBCONFIG_HANDLE_NULLPOINTER(FALSE, str2)
	i = 0;
	while (str1[i] && str2[i])
	{
		if (str1[i] != str2[i])
			return (FALSE);
		++i;
	}
	return (str1[i] == str2[i]);
}



t_bool	String_Equals_N(t_char const* str1, t_char const* str2, t_size n)
{
	t_size i;

	if (str1 == str2 || n == 0)
		return (TRUE);
	LIBCONFIG_HANDLE_NULLPOINTER(FALSE, str1)
	LIBCONFIG_HANDLE_NULLPOINTER(FALSE, str2)
	i = 0;
	while (str1[i] && str2[i])
	{
		if (str1[i] != str2[i])
			return (FALSE);
		++i;
		if (i == n)
			return (TRUE);
	}
	return (str1[i] == str2[i]);
}



t_bool	String_Equals_IgnoreCase(t_char const* str1, t_char const* str2)
{
	t_size i;

	if (str1 == str2)
		return (TRUE);
	LIBCONFIG_HANDLE_NULLPOINTER(FALSE, str1)
	LIBCONFIG_HANDLE_NULLPOINTER(FALSE, str2)
	i = 0;
	while (str1[i] && str2[i])
	{
		if (str1[i] != str2[i] &&
			!(Char_IsLower(str1[i]) && Char_ToUpper(str1[i]) == str2[i]) &&
			!(Char_IsUpper(str1[i]) && Char_ToLower(str1[i]) == str2[i]))
			return (FALSE);
		++i;
	}
	return (str1[i] == str2[i]);
}



t_bool	String_Equals_N_IgnoreCase(t_char const* str1, t_char const* str2, t_size n)
{
	t_size i;

	if (str1 == str2 || n == 0)
		return (TRUE);
	LIBCONFIG_HANDLE_NULLPOINTER(FALSE, str1)
	LIBCONFIG_HANDLE_NULLPOINTER(FALSE, str2)
	i = 0;
	while (str1[i] && str2[i])
	{
		if (str1[i] != str2[i] &&
			!(Char_IsLower(str1[i]) && Char_ToUpper(str1[i]) == str2[i]) &&
			!(Char_IsUpper(str1[i]) && Char_ToLower(str1[i]) == str2[i]))
			return (FALSE);
		++i;
		if (i == n)
			return (TRUE);
	}
	return (str1[i] == str2[i]);
}
