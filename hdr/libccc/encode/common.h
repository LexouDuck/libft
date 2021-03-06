/*============================================================================*/
/*                                            ______________________________  */
/*  libccc/encode/common.h                   |    __    __  ___      _____  | */
/*                                           |   / /\  / /\/ . |\   /  __|\ | */
/*  https://github.com/LexouDuck/libccc.git  |  / /_/ / / / . <_/  |  /___| | */
/*                                           | /___/\/_/ /___-'\   \____/\  | */
/* Comprehensive|Cross-platform|Customizable | \____/\__/\____-'    \____/  | */
/* This source code follows the MIT License. |______________________________| */
/*                                                                            */
/*============================================================================*/

#ifndef __LIBCCC_ENCODE_COMMON_H
#define __LIBCCC_ENCODE_COMMON_H
/*!@group{libccc_encode_common}
** @{
**	This header defines a dynamic runtime "object" type, which is called "KVT",
**	as in "Key Value Tree" - it stores key/value pairs in a tree-like structure.
**
**	In particular, much of the code in this header was inspired by cJSON:
**	- https://github.com/DaveGamble/cJSON
**
**	@file
*/

/*
** ************************************************************************** *|
**                                   Includes                                 *|
** ************************************************************************** *|
*/

#include "libccc/char.h"
#include "libccc/bool.h"
#include "libccc/int.h"
#include "libccc/float.h"
#include "libccc/pointer.h"
#include "libccc/text/unicode.h"

HEADER_CPP


/*
** ************************************************************************** *|
**                                 Definitions                                *|
** ************************************************************************** *|
*/

//! An integer type which works as a bitflag enum, used to express a dynamiclly-typed value's type at runtime
typedef t_sint		t_dynamic;

//! These constants are the enum items/bitflags for the `t_dynamic` dynamic-type enum
//!@{
#define DYNAMICTYPE_INVALID	((t_dynamic)(0))		//!< value stored is invalid
#define DYNAMICTYPE_NULL	((t_dynamic)(1 << 0))	//!< no value stored
#define DYNAMICTYPE_BOOLEAN	((t_dynamic)(1 << 1))	//!< value stored as boolean (`t_bool`)
#define DYNAMICTYPE_INTEGER	((t_dynamic)(1 << 2))	//!< value stored as integer (`t_s64`)
#define DYNAMICTYPE_FLOAT	((t_dynamic)(1 << 3))	//!< value stored as floating-point number: (`t_f64`)
#define DYNAMICTYPE_STRING	((t_dynamic)(1 << 4))	//!< value stored as string (`t_char*`)
#define DYNAMICTYPE_ARRAY	((t_dynamic)(1 << 5))	//!< value stored as array of values (no keys)
#define DYNAMICTYPE_OBJECT	((t_dynamic)(1 << 6))	//!< value stored as dict of values (with keys)
#define DYNAMICTYPE_RAW		((t_dynamic)(1 << 7))	//!< value stored as raw string (language-specific syntax)

#define DYNAMICTYPE_ISREFERENCE	((t_dynamic)(1 << 8))	//!< If this bit is set, the `value` is not to be freed
//!@}

//! The bitmask expressing the 'type enum' portion of a `t_dynamic` type specifier
#define DYNAMICTYPE_MASK	((t_dynamic)(0xFF))



//! A general key-value tree struct, used to model data for several file formats: JSON, INI/TOML, YAML, XML, etc
/*!
**	This struct can be used to store any kind of data, much like a general
**	extensible data file format: think JSON, INI/TOML, YAML, XML, etc.
**	The abbreviation "KVT" is used throughout the code to avoid writing `key_value_tree`, which is rather long.
**	In general, you should not use this struct directly, unless you wish to implement a new file format spec.
*/
typedef struct kvt
{
	struct kvt*	next;	//!< linked-list pointer to neighboring items (is `NULL` for the last item in the list)
	struct kvt*	prev;	//!< linked-list pointer to neighboring items (for the first item, `prev` will point to the end of the list)

//	t_utf8*		comment;//!< A comment line associated with this item // TODO
	t_utf8*		key;	//!< The item's key string, if this item is the child of, or is in the list of subitems of, an object.
	t_dynamic	type;	//!< The type of the item: uses the `TOML_TYPE_*` macros defined above.
	union dynamic
	{
		t_bool		boolean;	//!< #DYNAMICTYPE_BOOLEAN
		t_s64		integer;	//!< #DYNAMICTYPE_INTEGER
		t_f64		number;		//!< #DYNAMICTYPE_FLOAT
		t_utf8*		string;		//!< #DYNAMICTYPE_STRING or #DYNAMICTYPE_RAW
		struct kvt*	child;		//!< #DYNAMICTYPE_ARRAY or #DYNAMICTYPE_OBJECT
	}			value;	//!< The item's stored value (can be of any type)
}		s_kvt;



//! This union type can express a value of any major data type, see #t_dynamic
typedef union dynamic	u_dynamic;



#define foreach_s_kvt(_TYPE_, _VAR_, _KVT_)		foreach (_TYPE_, _VAR_, s_kvt, _KVT_)

#define foreach_s_kvt_init(		_TYPE_, _VAR_, _KVT_)	s_kvt const* _VAR_##_i = (_KVT_ != NULL ? (_KVT_)->value.child : NULL);
#define foreach_s_kvt_exit(		_TYPE_, _VAR_, _KVT_)	if ((_KVT_ != NULL) && (KVT_IsArray(_KVT_) || KVT_IsObject(_KVT_)))
#define foreach_s_kvt_loop_init(_TYPE_, _VAR_, _KVT_)	_TYPE_ _VAR_ = (_TYPE_)_VAR_##_i
#define foreach_s_kvt_loop_exit(_TYPE_, _VAR_, _KVT_)	(_VAR_##_i != NULL)
#define foreach_s_kvt_loop_incr(_TYPE_, _VAR_, _KVT_)	_VAR_##_i = _VAR_##_i->next
#define foreach_s_kvt_loop_setv(_TYPE_, _VAR_, _KVT_)	_VAR_ = (_VAR_##_i == NULL ? _VAR_ : (_TYPE_)_VAR_##_i)



//! The maximum depth of nested arrays/objects within a key-value-tree
/*!
**	Limits how deeply nested arrays/objects can be before KVT rejects to parse them.
**	The existence of this macro serves to prevent stack overflows in several cases.
*/
#ifndef KVT_NESTING_LIMIT
#define KVT_NESTING_LIMIT 1000
#endif



//! This struct is used to parse a data file string (JSON, TOML, YAML, XML, etc)
typedef struct kvt_parse
{
	t_utf8 const*	content;//!< the string to parse
	s_kvt*		result;		//!< the result JSON
	t_size		offset;		//!< current parsing offset
	t_size		length;		//!< the length of the string to parse
	t_bool		strict;		//!< if TRUE, strict parsing mode is on (rigourously follows the spec)
	t_uint		depth;		//!< current section nesting level
	t_size		line;		//!< current line number
	t_char*		error;		//!< current error message (or NULL if no error has been thrown yet)
}		s_kvt_parse;



//! This struct is used to print a data file string (JSON, TOML, YAML, XML, etc)
typedef struct kvt_print
{
	s_kvt const*	item;	//!< the object to print
	t_utf8*		result;		//!< the result string which is written to
	t_size		offset;		//!< current writing offset of the string to print
	t_size		length;		//!< the (current maximum) length of the string to print
	t_bool		noalloc;	//!< if `TRUE`, then it means `buffer` is pre-allocated by the caller
	t_bool		format;		//!< is this print a formatted print
	t_size		depth;		//!< the current nesting depth (for formatted printing)
	t_utf8*		keypath;	//!< the current path of keys, separated by periods (think TOML section tables)
}		s_kvt_print;

#define KVT_NUMBER_BUFFERSIZE	64

//! This function is used by the `*_Parse()` functions, to update the #s_kvt_print struct
/*!
**	Calculates the new length of the string in `p->result` and updates the `p->offset`.
*/
void	KVT_Print_UpdateOffset(s_kvt_print* p);

//! This function is used by the `*_Print()` functions, to ensure the print buffer has `needed` bytes more
/*!
**	If the `p->length` is too short, will reallocate the `p->result` string.
**	If `p->noalloc == FALSE`, this will simply return `NULL` if `needed` is too large.
*/
t_utf8*	KVT_Print_EnsureBuffer(s_kvt_print* p, t_size needed);



/*
** ************************************************************************** *|
**                             Basic KVT Operations                           *|
** ************************************************************************** *|
*/

//! Allocates one single KVT struct
s_kvt*	KVT_Item(void);



//! Duplicates a KVT object.
/*!
**	Duplicate will create a new, identical `s_kvt` item to the one you pass, in new memory that will
**	need to be released. With `recurse != FALSE`, it will duplicate any children connected to the item.
**	The `item`'s `->next` and `->prev` pointers are always zero on return from Duplicate.
*/
s_kvt*	KVT_Duplicate(s_kvt const* item, t_bool recurse);



//! Recursively compare two `s_kvt` items for equality.
/*!
**	If either `a` or `b` is `NULL` or invalid, they will be considered unequal.
**	@param kvt1				the first KVT to check
**	@param kvt2				the second KVT to check
**	@param case_sensitive	if `TRUE`, object keys are treated as case-sensitive
**	@returns
**	`TRUE` if `a` and `b` have equal contents
*/
t_bool	KVT_Equals(s_kvt const* kvt1, s_kvt const* kvt2, t_bool case_sensitive);



//! Creates a new KVT object by concatenating two existing ones
/*!
**	Concatenate two objects together while duplicating all of their contents.
*/
s_kvt*	KVT_Concat(s_kvt const* kvt1, s_kvt const* kvt2);
//! TODO document
s_kvt*	KVT_Concat_Array(s_kvt const* kvt1, s_kvt const* kvt2);
//! TODO document
s_kvt*	KVT_Concat_Object(s_kvt const* kvt1, s_kvt const* kvt2);



/*
** ************************************************************************** *|
**                            KVT "create" Operations                         *|
** ************************************************************************** *|
*/

//! These functions create an `s_kvt` item of the appropriate type.
//!@{
s_kvt*	KVT_CreateNull(void);
s_kvt*	KVT_CreateBoolean(t_bool value);
s_kvt*	KVT_CreateInteger(t_s64 value);
s_kvt*	KVT_CreateFloat(t_f64 value);
s_kvt*	KVT_CreateString(t_char const* value);
s_kvt*	KVT_CreateArray(void);
s_kvt*	KVT_CreateObject(void);
s_kvt*	KVT_CreateRaw(t_char const* raw);
//!@}



//! Create a string where `value_string` references a string, so it will not be freed by KVT_Delete()
s_kvt*	KVT_CreateStringReference(t_char const* string);
//! Create an array that only references it's elements, so they will not be freed by KVT_Delete()
s_kvt*	KVT_CreateArrayReference(s_kvt const* child);
//! Create an object that only references it's elements, so they will not be freed by KVT_Delete()
s_kvt*	KVT_CreateObjectReference(s_kvt const* child);

//! Create and fill a KVT array
/*!
**	These utilities create a KVT array of `count` items.
**	The given `count` cannot be greater than the number of elements in
**	the given `numbers` array, otherwise array access will be out of bounds.
*/
//!@{
s_kvt*	KVT_CreateArray_Boolean	(t_bool const*	booleans, t_uint count);

s_kvt*	KVT_CreateArray_UInt	(t_uint const*	numbers, t_uint count);
s_kvt*	KVT_CreateArray_U8		(t_u8 const*	numbers, t_uint count);
s_kvt*	KVT_CreateArray_U16		(t_u16 const*	numbers, t_uint count);
s_kvt*	KVT_CreateArray_U32		(t_u32 const*	numbers, t_uint count);
s_kvt*	KVT_CreateArray_U64		(t_u64 const*	numbers, t_uint count);
#ifdef __int128
s_kvt*	KVT_CreateArray_U128	(t_u128 const*	numbers, t_uint count);
#endif

s_kvt*	KVT_CreateArray_SInt	(t_sint const*	numbers, t_uint count);
s_kvt*	KVT_CreateArray_S8		(t_s8 const*	numbers, t_uint count);
s_kvt*	KVT_CreateArray_S16		(t_s16 const*	numbers, t_uint count);
s_kvt*	KVT_CreateArray_S32		(t_s32 const*	numbers, t_uint count);
s_kvt*	KVT_CreateArray_S64		(t_s64 const*	numbers, t_uint count);
#ifdef __int128
s_kvt*	KVT_CreateArray_S128	(t_s128 const*	numbers, t_uint count);
#endif

s_kvt*	KVT_CreateArray_Float	(t_float const*	numbers, t_uint count);
s_kvt*	KVT_CreateArray_F32		(t_f32 const*	numbers, t_uint count);
s_kvt*	KVT_CreateArray_F64		(t_f64 const*	numbers, t_uint count);
#ifdef __float80
s_kvt*	KVT_CreateArray_F80		(t_f80 const*	numbers, t_uint count);
#endif
#ifdef __float128
s_kvt*	KVT_CreateArray_F128	(t_f128 const*	numbers, t_uint count);
#endif

s_kvt*	KVT_CreateArray_String	(t_char const* const* strings, t_uint count);
//!@}



/*
** ************************************************************************** *|
**                             KVT "get" Operations                           *|
** ************************************************************************** *|
*/

//! Returns the amount of items in the given `array` (or object).
t_sint	KVT_GetArrayLength(s_kvt const* array);

//! Retrieve item number `index` from the given `array`. Returns `NULL` if unsuccessful.
/*!
**	@param	array	The array (or object) from which to get an item
**	@param	index	The index of the item to get (NOTE: if negative, get starting from the last item)
*/
s_kvt*	KVT_GetArrayItem(s_kvt const* array, t_sint index);



//! Get the item with the given `key` from the given `object`.
#define KVT_GetObjectItem \
		KVT_GetObjectItem_CaseSensitive
s_kvt*	KVT_GetObjectItem_CaseSensitive	(s_kvt const* object, t_char const* key);	//!< (case-sensitive)
s_kvt*	KVT_GetObjectItem_IgnoreCase	(s_kvt const* object, t_char const* key);	//!< (case-insensitive)

//! Returns `TRUE` if the given `object` contains an item with the given `key`.
#define KVT_HasObjectItem \
		KVT_HasObjectItem_CaseSensitive
t_bool	KVT_HasObjectItem_CaseSensitive	(s_kvt const* object, t_char const* key);	//!< (case-sensitive)
t_bool	KVT_HasObjectItem_IgnoreCase	(s_kvt const* object, t_char const* key);	//!< (case-insensitive)



//! Access the contents of a KVT with a 'KVT path', ie: a format string of 'accessors' (ie: strings or numbers in brackets)
/*!
**	@param	object		The KVT object to get an item from
**	@param	format_path	The format string with an accessor pattern (example: `KVT_Get(json, "[\"subarray\"][3][\"name\"]")`)
**	@param	...			The variadic arguments list which goes along with `format_path` (works like printf)
**	@returns
**	The KVT object gotten from the given accessor path,
**	or `NULL` if the given path or KVT object was invalid.
*/
s_kvt*	KVT_Get(s_kvt const* object, t_char const* format_path, ...)
_FORMAT(printf, 2, 3);

//! Returns the boolean value contained within the given `item`, or `FALSE` if type is not #KVT_TYPE_BOOLEAN.
t_bool	KVT_GetValue_Boolean(s_kvt const* item);
//! Returns the integer value contained within the given `item`, or `0` if type is not #KVT_TYPE_INTEGER.
t_s64	KVT_GetValue_Integer(s_kvt const* item);
//! Returns the number value contained within the given `item`, or `NAN` if type is not #KVT_TYPE_FLOAT.
t_f64	KVT_GetValue_Float	(s_kvt const* item);
//! Returns the string value contained within the given `item`, or `NULL` if type is not #KVT_TYPE_STRING.
t_char*	KVT_GetValue_String	(s_kvt const* item);



/*
** ************************************************************************** *|
**                             KVT "set" Operations                           *|
** ************************************************************************** *|
*/

//! Change the `value` of a #KVT_TYPE_BOOLEAN object, only takes effect when `object->type == KVT_TYPE_BOOLEAN`.
e_cccerror	KVT_SetValue_Boolean(s_kvt* object, t_bool value);
//! Change the `value` of a #KVT_TYPE_INTEGER object, only takes effect when `object->type == KVT_TYPE_INTEGER`.
e_cccerror	KVT_SetValue_Integer(s_kvt* object, t_s64 value);
//! Change the `value` of a #KVT_TYPE_FLOAT object, only takes effect when `object->type == KVT_TYPE_FLOAT`.
e_cccerror	KVT_SetValue_Float	(s_kvt* object, t_f64 value);
//! Change the `value` of a #KVT_TYPE_STRING object, only takes effect when `object->type == KVT_TYPE_STRING`.
e_cccerror	KVT_SetValue_String	(s_kvt* object, t_char* value);



//! Appends the given `item` to the given `array`.
e_cccerror	KVT_AddToArray_Item(s_kvt* array, s_kvt* item);

//! Append a reference to `item` to the given `array`.
e_cccerror	KVT_AddToArray_ItemReference(s_kvt* array, s_kvt* item);



//! Appends the given `item` to the given `object`, with the given `key`.
e_cccerror	KVT_AddToObject_Item(s_kvt* object, t_char const* key, s_kvt* item);

//! Append reference to item to the given object.
e_cccerror	KVT_AddToObject_ItemReference(s_kvt* object, t_char const* key, s_kvt* item);



/*!
**	Helper functions for creating and adding items to an object at the same time.
**	@returns
**	The newly added KVT item, or `NULL` on failure.
*/
//!@{
s_kvt*	KVT_AddToObject_Null	(s_kvt* object, t_char const* key);
s_kvt*	KVT_AddToObject_Boolean	(s_kvt* object, t_char const* key, t_bool value);
s_kvt*	KVT_AddToObject_Integer	(s_kvt* object, t_char const* key, t_s64 value);
s_kvt*	KVT_AddToObject_Float	(s_kvt* object, t_char const* key, t_f64 value);
s_kvt*	KVT_AddToObject_String	(s_kvt* object, t_char const* key, t_char const* value);
s_kvt*	KVT_AddToObject_Object	(s_kvt* object, t_char const* key);
s_kvt*	KVT_AddToObject_Array	(s_kvt* object, t_char const* key);
s_kvt*	KVT_AddToObject_Raw		(s_kvt* object, t_char const* key, t_char const* raw);
//!@}



/*
** ************************************************************************** *|
**                             KVT Check Operations                           *|
** ************************************************************************** *|
*/

//! These functions are used to check the type of an item.
//!@{
t_bool	KVT_IsInvalid	(s_kvt const* item);
t_bool	KVT_IsNull		(s_kvt const* item);
t_bool	KVT_IsBoolean	(s_kvt const* item);
t_bool	KVT_IsInteger	(s_kvt const* item);
t_bool	KVT_IsFloat		(s_kvt const* item);
t_bool	KVT_IsString	(s_kvt const* item);
t_bool	KVT_IsArray		(s_kvt const* item);
t_bool	KVT_IsObject	(s_kvt const* item);
t_bool	KVT_IsRaw		(s_kvt const* item);
//!@}



// TODO add "check for name conflicts" function



/*
** ************************************************************************** *|
**                             KVT Other Operations                           *|
** ************************************************************************** *|
*/

//! Removes (without deleting) the given `item` from the given `parent` object.
s_kvt*		KVT_Detach(s_kvt* parent, s_kvt* item);

//! Delete a `s_kvt` entity and all subentities.
e_cccerror	KVT_Delete(s_kvt* item);

//! Replaces the given `item` from the given `parent` object, with the given `newitem`.
e_cccerror	KVT_Replace(s_kvt* parent, s_kvt* item, s_kvt* newitem);



//! Deletes the item at the given `index` from the given `array`.
e_cccerror	KVT_Delete_FromArray(s_kvt* array, t_sint index);

//! Removes (without deleting) the given `item` from the given `array`.
s_kvt*		KVT_Detach_FromArray(s_kvt* array, t_sint index);

//! Replaces the given `item` from the given `array`, with the given `newitem`.
e_cccerror	KVT_Replace_InArray(s_kvt* array, t_sint index, s_kvt* newitem);

//! Inserts the given `newitem` in the givne `array`, shifting pre-existing items to the right.
e_cccerror	KVT_Insert_InArray(s_kvt* array, t_sint index, s_kvt* newitem);



//! Deletes the item with the given `key` from the given `object`.
//!@{
#define 	KVT_Delete_FromObject \
			KVT_Delete_FromObject_CaseSensitive
e_cccerror	KVT_Delete_FromObject_CaseSensitive	(s_kvt* object, t_char const* key); //!< (case-sensitive)
e_cccerror	KVT_Delete_FromObject_IgnoreCase	(s_kvt* object, t_char const* key); //!< (case-insensitive)
//!@}

//! Removes (without deleting) the given `item` from the given `object`.
//!@{
#define 	KVT_Detach_FromObject \
			KVT_Detach_FromObject_CaseSensitive
s_kvt*		KVT_Detach_FromObject_CaseSensitive	(s_kvt* object, t_char const* key); //!< (case-sensitive)
s_kvt*		KVT_Detach_FromObject_IgnoreCase	(s_kvt* object, t_char const* key); //!< (case-insensitive)
//!@}

//! Replaces the given `item` from the given `object`, with the given `newitem`
//!@{
#define 	KVT_Replace_InObject \
			KVT_Replace_InObject_CaseSensitive
e_cccerror	KVT_Replace_InObject_CaseSensitive	(s_kvt* object, t_char const* key, s_kvt* newitem); //!< (case-sensitive)
e_cccerror	KVT_Replace_InObject_IgnoreCase		(s_kvt* object, t_char const* key, s_kvt* newitem); //!< (case-insensitive)
//!@}



/*! @} */
HEADER_END
#endif
