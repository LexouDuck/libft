
#include "libccc_char.h"
#include "libccc_convert.h"



t_size	Convert_String_To_Size(char const* str)
{
	t_size	result;
	t_size	i;

#if LIBCCCCONFIG_HANDLE_NULLPOINTERS
	if (str == NULL)
		return (0);
#endif
	i = 0;
	while (!(Char_IsDigit(str[i]) || str[i] == '+'))
	{
		if (!str[i] || !Char_IsSpace(str[i]))
			return (0);
		++i;
	}
	if (str[i] == '+')
		++i;
	result = 0;
	while (str[i] && Char_IsDigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		++i;
	}
	return (result);
}
