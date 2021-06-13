
#include "libccc.h"
#include "libccc/encode/common.h"

#include LIBCONFIG_ERROR_INCLUDE



s_kvt*	KVT_Concat(s_kvt const* kvt1, s_kvt const* kvt2)
{
	s_kvt*	result = NULL;
	s_kvt*	concat = NULL;
	e_cccerror	error;
	t_bool	kvt1_isarray;
	t_bool	kvt2_isarray;
	t_bool	kvt1_isobject;
	t_bool	kvt2_isobject;

	kvt1_isarray  = KVT_IsArray (kvt1);
	kvt2_isarray  = KVT_IsArray (kvt2);
	kvt1_isobject = KVT_IsObject(kvt1);
	kvt2_isobject = KVT_IsObject(kvt2);
	if (!kvt1_isarray && !kvt1_isobject &&
		!kvt2_isarray && !kvt2_isobject)
	{
		result = KVT_CreateArray();
		HANDLE_ERROR(ALLOCFAILURE, (result == NULL), goto failure;)
		concat = KVT_Duplicate(kvt1, TRUE);
		HANDLE_ERROR(ALLOCFAILURE, (concat == NULL), goto failure;)
		error = KVT_AddToArray_Item(result, concat);
		HANDLE_ERROR(UNSPECIFIED, (error), goto failure;)
		concat = KVT_Duplicate(kvt2, TRUE);
		HANDLE_ERROR(ALLOCFAILURE, (concat == NULL), goto failure;)
		error = KVT_AddToArray_Item(result, concat);
		HANDLE_ERROR(UNSPECIFIED, (error), goto failure;)
	}
	else
	{
		result = KVT_Duplicate(kvt1, TRUE);
		HANDLE_ERROR(ALLOCFAILURE, (result == NULL), goto failure;)
		concat = KVT_Duplicate(kvt2, TRUE);
		HANDLE_ERROR(ALLOCFAILURE, (concat == NULL), goto failure;)

		if (kvt1_isarray && kvt2_isarray)
			KVT_Concat_Array(result, concat);
		else if (kvt1_isarray)
			KVT_AddToArray_Item(result, concat);
		else if (kvt2_isarray)
			KVT_AddToArray_Item(result, concat);

		if (kvt1_isobject && kvt2_isobject)
			KVT_Concat_Object(result, concat);
		else if (kvt1_isobject)
			KVT_Insert_InArray(concat, 0, result);
		else if (kvt2_isobject)
			KVT_Insert_InArray(concat, 0, result);
	}
	HANDLE_ERROR(UNSPECIFIED, (result == NULL), goto failure;)
	return (result);

failure:
	KVT_Delete(result);
	KVT_Delete(concat);
	return (NULL);
}



s_kvt*	KVT_Concat_Array(s_kvt const* kvt1, s_kvt const* kvt2)
{
	s_kvt*	result;
	s_kvt*	concat;

	HANDLE_ERROR(WRONGTYPE, (!KVT_IsArray(kvt1)), return (NULL);)
	HANDLE_ERROR(WRONGTYPE, (!KVT_IsArray(kvt2)), return (NULL);)
	result = KVT_Duplicate(kvt1, TRUE);
	concat = KVT_Duplicate(kvt2, TRUE);
	result->prev->next = concat; // use `prev` to access last element without looping
	result->prev = concat->prev; // update the new first-elem `prev` to point to end of array
	concat->prev = result;
	return (result);
}



s_kvt*	KVT_Concat_Object(s_kvt const* kvt1, s_kvt const* kvt2)
{
	s_kvt*	result;
	s_kvt*	concat;

	HANDLE_ERROR(WRONGTYPE, (!KVT_IsObject(kvt1)), return (NULL);)
	HANDLE_ERROR(WRONGTYPE, (!KVT_IsObject(kvt2)), return (NULL);)
	result = KVT_Duplicate(kvt1, TRUE);
	concat = KVT_Duplicate(kvt2, TRUE);
	result->prev->next = concat; // use `prev` to access last element without looping
	result->prev = concat->prev; // update the new first-elem `prev` to point to end of array
	concat->prev = result;
	return (result);
}
