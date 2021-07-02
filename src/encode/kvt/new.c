
#include "libccc/memory.h"
#include "libccc/encode/common.h"

#include LIBCONFIG_ERROR_INCLUDE



s_kvt*	KVT_Item(void)
{
	s_kvt*	result = (s_kvt*)Memory_New(sizeof(s_kvt));
	HANDLE_ERROR(ALLOCFAILURE, (result == NULL), return (NULL);)
	return (result);
}
