
#include "libccc.h"
#include "libccc/char.h"
#include "libccc/memory.h"
#include "libccc/encode/common.h"

#include LIBCONFIG_HANDLE_INCLUDE



e_stderror	KVT_Delete(s_kvt* item)
{
	HANDLE_ERROR(NULLPOINTER, (item == NULL), return (ERROR_NULLPOINTER);)
	s_kvt* next = NULL;
	while (item != NULL)
	{
		next = item->next;
		if (!(item->type & DYNAMICTYPE_ISREFERENCE))
		{
			if ((item->type & DYNAMICTYPE_ARRAY) ||
				(item->type & DYNAMICTYPE_OBJECT))
			{
				if (item->value.child != NULL)
				{
					e_stderror error = KVT_Delete(item->value.child);
					if (error)	return (error);
				}
			}
			else if ((item->type & DYNAMICTYPE_STRING))
			{
				if (item->value.string != NULL)
					Memory_Free(item->value.string);
			}
		}
		if (item->key != NULL)
		{
			Memory_Free(item->key);
		}
		Memory_Free(item);
		item = next;
	}
	return (OK);
}

e_stderror	KVT_Delete_FromArray(s_kvt* array, t_uint index)
{
	s_kvt*	tmp;

	HANDLE_ERROR(NULLPOINTER, (array == NULL), return (ERROR_NULLPOINTER);)
	tmp = KVT_Detach_FromArray(array, index);
	HANDLE_ERROR(INDEX2LARGE, (tmp == NULL), return (ERROR_INDEX2LARGE);)
	return (KVT_Delete(tmp));
}

e_stderror	KVT_Delete_FromObject_IgnoreCase(s_kvt* object, t_char const* key)
{
	s_kvt*	tmp;

	HANDLE_ERROR(NULLPOINTER, (object == NULL), return (ERROR_NULLPOINTER);)
	tmp = KVT_Detach_FromObject_IgnoreCase(object, key);
	HANDLE_ERROR(KEYNOTFOUND, (tmp == NULL), return (ERROR_KEYNOTFOUND);)
	return (KVT_Delete(tmp));
}

e_stderror	KVT_Delete_FromObject_CaseSensitive(s_kvt* object, t_char const* key)
{
	s_kvt*	tmp;

	HANDLE_ERROR(NULLPOINTER, (object == NULL), return (ERROR_NULLPOINTER);)
	tmp = KVT_Detach_FromObject_CaseSensitive(object, key);
	HANDLE_ERROR(KEYNOTFOUND, (tmp == NULL), return (ERROR_KEYNOTFOUND);)
	return (KVT_Delete(tmp));
}