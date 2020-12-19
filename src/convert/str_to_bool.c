
#include "libft_char.h"
#include "libft_convert.h"



t_bool	Convert_String_To_Bool(char const* str)
{
	t_size	i;

#if LIBFTCONFIG_HANDLE_NULLPOINTERS
	if (str == NULL)
		return (FALSE);
#endif
	i = 0;
	while (str[i] && ft_isspace(str[i]))
		++i;
	if (!str[i])
		return (FALSE);
	if (ft_isdigit(str[i]) && ft_str_to_s64(str + i))
		return (TRUE);
	if ((str[i + 0] == 'T' || str[i + 0] == 't') &&
		(str[i + 1] == 'R' || str[i + 1] == 'r') &&
		(str[i + 2] == 'U' || str[i + 2] == 'u') &&
		(str[i + 3] == 'E' || str[i + 3] == 'e'))
		return (TRUE);
	return (FALSE);
}
