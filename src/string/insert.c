
#include "libccc/memory.h"
#include "libccc/string.h"



t_char*	String_Insert(t_char const* dest, t_char const* src, t_size offset)
{
	t_char*	result;
	t_size	len_dst;
	t_size	len_src;
	t_size	i;

#if LIBCONFIG_HANDLE_NULLPOINTERS
	if (dest == NULL || src == NULL)
		return (NULL);
#endif
	len_dst = String_Length(dest);
	len_src = String_Length(src);
	if (!(result = (t_char* )Memory_Alloc(len_dst + len_src + 1)))
		return (NULL);
	i = (t_size)-1;
	while (++i < offset)
		result[i] = dest[i];
	--i;
	while (++i < len_src)
		result[i] = src[i - offset];
	--i;
	while (++i < len_dst)
		result[i] = dest[i - len_src];
	result[i] = '\0';
	return (result);
}



t_char*	String_Insert_InPlace(t_char* *a_dest, t_char const* src, t_u32 index)
{
	t_char*	tmp;

#if LIBCONFIG_HANDLE_NULLPOINTERS
	if (a_dest == NULL || src == NULL)
		return (NULL);
#endif
	tmp = String_Sub(*a_dest, 0, index);
	String_Append(&tmp, src);
	String_Append(&tmp, (*a_dest) + index);
	String_Delete(a_dest);
	*a_dest = tmp;
	return (*a_dest);
}
