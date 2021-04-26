/*============================================================================*/
/*                                            ______________________________  */
/*  libccc/encode/json.h                     |    __    __  ___      _____  | */
/*                                           |   / /\  / /\/ . |\   /  __|\ | */
/*  https://github.com/LexouDuck/libccc.git  |  / /_/ / / / . <_/  |  /___| | */
/*                                           | /___/\/_/ /___-'\   \____/\  | */
/* Comprehensive|Cross-platform|Customizable | \____/\__/\____-'    \____/  | */
/* This source code follows the MIT License. |______________________________| */
/*                                                                            */
/*============================================================================*/

#ifndef __LIBCCC_ENCODE_JSON_H
#define __LIBCCC_ENCODE_JSON_H
/*!@group{libccc_encode_json}
** @{
**	This header defines a dynamic runtime object type, similar to objects in JS.
**	- JSON spec: https://www.json.org/json-en.html
**
**	In particular, most of the code exposed from this header comes from cJSON:
**	https://github.com/DaveGamble/cJSON
**
**	@file
*/

/*
** ************************************************************************** *|
**                                   Includes                                 *|
** ************************************************************************** *|
*/

#include "libccc.h"
#include "libccc/char.h"
#include "libccc/encode/common.h"

HEADER_CPP

/*
** ************************************************************************** *|
**                                 Definitions                                *|
** ************************************************************************** *|
*/

typedef s_kvt	s_json;



#define foreach_s_json(_TYPE_, _VAR_, _JSON_)		foreach (_TYPE_, _VAR_, s_json, _JSON_)

#define foreach_s_json_init			foreach_s_kvt_init
#define foreach_s_json_exit			foreach_s_kvt_exit
#define foreach_s_json_loop_init	foreach_s_kvt_loop_init
#define foreach_s_json_loop_exit	foreach_s_kvt_loop_exit
#define foreach_s_json_loop_incr	foreach_s_kvt_loop_incr
#define foreach_s_json_loop_setv	foreach_s_kvt_loop_setv



/*
** ************************************************************************** *|
**                             JSON String Operations                         *|
** ************************************************************************** *|
*/

//! Create a new `s_json` object, parsed from a (valid) JSON string
/*!
**	Memory Management: the caller is always responsible to free the results,
**	from all variants of JSON_Parse() (by using JSON_Delete()) and JSON_Print()
**	(with stdlib free(), JSON_Hooks.free_fn(), or JSON_free() as appropriate).
**	The exception is JSON_PrintPreallocated(), where the caller has full responsibility of the buffer.
**	Supply a block of JSON, and this returns a `s_json` object you can interrogate.
*/
s_json*	JSON_FromString(t_char const* value);
#define JSON_Parse	JSON_FromString
#define JSON_Decode	JSON_FromString

//! Create a new `s_json` object, parsed from a (valid) JSON string, (only the first `buffer_length` chars are parsed)
s_json*	JSON_FromString_N(t_char const* value, t_size buffer_length);
#define JSON_Parse_N	JSON_FromString_N
#define JSON_Decode_N	JSON_FromString_N

//! Create a new `s_json` object, parsed from a (valid) JSON string
/*!
**	JSON_ParseStrict() allows you to require (and check) that the JSON is null terminated,
**	and to retrieve the pointer to the final byte parsed.
**	If you supply a ptr in return_parse_end and parsing fails, then `return_parse_end`
**	will contain a pointer to the error, such that it will match the return of JSON_GetErrorPtr().
*/
s_json*	JSON_FromString_Strict(t_char const* value, t_char const* *return_parse_end, t_bool require_null_terminated);
#define JSON_Parse_Strict	JSON_FromString_Strict
#define JSON_Decode_Strict	JSON_FromString_Strict

//! Create a new `s_json` object, parsed from a (valid) JSON string, (only the first `buffer_length` chars are parsed)
s_json*	JSON_FromString_Strict_N(t_char const* value, t_size buffer_length, t_char const* *return_parse_end, t_bool require_null_terminated);
#define JSON_Parse_Strict_N 	JSON_FromString_Strict_N
#define JSON_Decode_Strict_N	JSON_FromString_Strict_N



#define JSON_ToString	JSON_ToString_Pretty
#define JSON_Print		JSON_ToString
#define JSON_Encode		JSON_ToString

//! Render a s_json entity to text for transfer/storage (with 'pretty' formatting).
t_char*	JSON_ToString_Pretty(s_json const* item);
#define JSON_Print_Pretty	JSON_ToString_Pretty
#define JSON_Encode_Pretty	JSON_ToString_Pretty

//! Render a s_json entity to text for transfer/storage, without any formatting/whitespace
t_char*	JSON_ToString_Minify(s_json const* item);
#define JSON_Print_Minify	JSON_ToString_Minify
#define JSON_Decode_Minify	JSON_ToString_Minify

//! Render a s_json entity to text using a buffered strategy.
/*!
**	prebuffer is a guess at the final size. guessing well reduces reallocation. fmt=0 gives unformatted, =1 gives formatted.
*/
t_char*	JSON_ToString_Buffered(s_json const* item, t_sint prebuffer, t_bool fmt);
#define JSON_Print_Buffered 	JSON_ToString_Buffered
#define JSON_Decode_Buffered 	JSON_ToString_Buffered

//! Render a `s_json` entity to text using a buffer already allocated in memory with given length.
/*!
**	@returns 1(TRUE) on success and 0(FALSE) on failure.
**	NOTE: s_json is not always 100% accurate in estimating how much memory it will use, so to be safe allocate 5 bytes more than you actually need.
*/
t_bool	JSON_ToString_Preallocated(s_json* item, t_char* buffer, t_sint const length, t_bool const format);
#define JSON_Print_Preallocated 	JSON_ToString_Preallocated
#define JSON_Encode_Preallocated 	JSON_ToString_Preallocated



//! Minify a JSON string, to make it more lightweight: removes all whitespace characters
/*!
**	Minify a JSON, removing blank characters (such as ' ', '\t', '\r', '\n') from strings.
**	The input pointer json cannot point to a read-only address area, such as a string constant, 
**	but should point to a readable and writable address area.
*/
void	JSON_Minify(t_char* json); //!< TODO rename to JSON_Minify_InPlace(), and add JSON_Minify(), which would allocate



/*
** ************************************************************************** *|
**                             Basic JSON Operations                           *|
** ************************************************************************** *|
*/

#define JSON_Item		KVT_Item		//!< @alias{KVT_Item}

#define JSON_Duplicate	KVT_Duplicate	//!< @alias{KVT_Duplicate}

#define JSON_Equals		KVT_Equals		//!< @alias{KVT_Equals}

#define JSON_Concat		KVT_Concat		//!< @alias{KVT_Concat}



#define JSON_GetError			KVT_GetError		//!< @alias{KVT_GetError}

#define JSON_GetErrorMessage	KVT_GetErrorMessage	//!< @alias{KVT_GetErrorMessage}



/*
** ************************************************************************** *|
**                            JSON "create" Operations                         *|
** ************************************************************************** *|
*/

#define JSON_CreateNull		KVT_CreateNull		//!< @alias{KVT_CreateNull}
#define JSON_CreateBoolean	KVT_CreateBoolean	//!< @alias{KVT_CreateBoolean}
#define JSON_CreateInteger	KVT_CreateInteger	//!< @alias{KVT_CreateInteger}
#define JSON_CreateFloat	KVT_CreateFloat		//!< @alias{KVT_CreateFloat}
#define JSON_CreateString	KVT_CreateString	//!< @alias{KVT_CreateString}
#define JSON_CreateArray	KVT_CreateArray		//!< @alias{KVT_CreateArray}
#define JSON_CreateObject	KVT_CreateObject	//!< @alias{KVT_CreateObject}
#define JSON_CreateRaw		KVT_CreateRaw		//!< @alias{KVT_CreateRaw}



#define JSON_CreateArrayReference	KVT_CreateArrayReference	//!< @alias{KVT_CreateArrayReference}
#define JSON_CreateObjectReference	KVT_CreateObjectReference	//!< @alias{KVT_CreateObjectReference}
#define JSON_CreateStringReference	KVT_CreateStringReference	//!< @alias{KVT_CreateStringReference}



#define JSON_CreateArray_Boolean	KVT_CreateArray_Boolean	//!< @alias{KVT_CreateArray_Boolean}
#define JSON_CreateArray_UInt		KVT_CreateArray_UInt	//!< @alias{KVT_CreateArray_UInt}
#define JSON_CreateArray_U8			KVT_CreateArray_U8		//!< @alias{KVT_CreateArray_U8}
#define JSON_CreateArray_U16		KVT_CreateArray_U16		//!< @alias{KVT_CreateArray_U16}
#define JSON_CreateArray_U32		KVT_CreateArray_U32		//!< @alias{KVT_CreateArray_U32}
#define JSON_CreateArray_U64		KVT_CreateArray_U64		//!< @alias{KVT_CreateArray_U64}
#define JSON_CreateArray_U128		KVT_CreateArray_U128	//!< @alias{KVT_CreateArray_U128}
#define JSON_CreateArray_SInt		KVT_CreateArray_SInt	//!< @alias{KVT_CreateArray_SInt}
#define JSON_CreateArray_S8			KVT_CreateArray_S8		//!< @alias{KVT_CreateArray_S8}
#define JSON_CreateArray_S16		KVT_CreateArray_S16		//!< @alias{KVT_CreateArray_S16}
#define JSON_CreateArray_S32		KVT_CreateArray_S32		//!< @alias{KVT_CreateArray_S32}
#define JSON_CreateArray_S64		KVT_CreateArray_S64		//!< @alias{KVT_CreateArray_S64}
#define JSON_CreateArray_S128		KVT_CreateArray_S128	//!< @alias{KVT_CreateArray_S128}
#define JSON_CreateArray_Float		KVT_CreateArray_Float	//!< @alias{KVT_CreateArray_Float}
#define JSON_CreateArray_F32		KVT_CreateArray_F32		//!< @alias{KVT_CreateArray_F32}
#define JSON_CreateArray_F64		KVT_CreateArray_F64		//!< @alias{KVT_CreateArray_F64}
#define JSON_CreateArray_F80		KVT_CreateArray_F80		//!< @alias{KVT_CreateArray_F80}
#define JSON_CreateArray_F128		KVT_CreateArray_F128	//!< @alias{KVT_CreateArray_F128}
#define JSON_CreateArray_String		KVT_CreateArray_String	//!< @alias{KVT_CreateArray_String}



/*
** ************************************************************************** *|
**                             JSON "get" Operations                           *|
** ************************************************************************** *|
*/

#define JSON_GetArrayLength		KVT_GetArrayLength	//!< @alias{KVT_GetArrayLength}

#define JSON_GetArrayItem		KVT_GetArrayItem	//!< @alias{KVT_GetArrayItem}



#define JSON_GetObjectItem \
		JSON_GetObjectItem_IgnoreCase
#define JSON_GetObjectItem_IgnoreCase		KVT_GetObjectItem_IgnoreCase	//!< @alias{KVT_GetObjectItem_IgnoreCase}
#define JSON_GetObjectItem_CaseSensitive	KVT_GetObjectItem_CaseSensitive	//!< @alias{KVT_GetObjectItem_CaseSensitive}



#define JSON_HasObjectItem		KVT_HasObjectItem	//!< @alias{KVT_HasObjectItem}



#define JSON_Get				KVT_Get	//!< @alias{KVT_Get}

#define JSON_GetValue_Boolean 	KVT_GetValue_Boolean	//!< @alias{KVT_GetValue_Boolean}
#define JSON_GetValue_Integer 	KVT_GetValue_Integer	//!< @alias{KVT_GetValue_Integer}
#define JSON_GetValue_Float 	KVT_GetValue_Float		//!< @alias{KVT_GetValue_Float}
#define JSON_GetValue_String 	KVT_GetValue_String		//!< @alias{KVT_GetValue_String}



/*
** ************************************************************************** *|
**                             JSON "set" Operations                           *|
** ************************************************************************** *|
*/

#define JSON_SetValue_Boolean 	KVT_SetValue_Boolean	//!< @alias{KVT_SetValue_Boolean}
#define JSON_SetValue_Integer 	KVT_SetValue_Integer	//!< @alias{KVT_SetValue_Integer}
#define JSON_SetValue_Float 	KVT_SetValue_Float		//!< @alias{KVT_SetValue_Float}
#define JSON_SetValue_String 	KVT_SetValue_String		//!< @alias{KVT_SetValue_String}



#define JSON_AddToArray_Item				KVT_AddToArray_Item				//!< @alias{KVT_AddToArray_Item}
#define JSON_AddToArray_ItemReference		KVT_AddToArray_ItemReference	//!< @alias{KVT_AddToArray_ItemReference}

#define JSON_AddToObject_Item				KVT_AddToObject_Item			//!< @alias{KVT_AddToObject_Item}
#define JSON_AddToObject_ItemReference		KVT_AddToObject_ItemReference	//!< @alias{KVT_AddToObject_ItemReference}



#define JSON_AddToObject_Null		KVT_AddToObject_Null	//!< @alias{KVT_AddToObject_Null}
#define JSON_AddToObject_Boolean	KVT_AddToObject_Boolean	//!< @alias{KVT_AddToObject_Boolean}
#define JSON_AddToObject_Integer	KVT_AddToObject_Integer	//!< @alias{KVT_AddToObject_Integer}
#define JSON_AddToObject_Float		KVT_AddToObject_Float	//!< @alias{KVT_AddToObject_Float}
#define JSON_AddToObject_String		KVT_AddToObject_String	//!< @alias{KVT_AddToObject_String}
#define JSON_AddToObject_Object		KVT_AddToObject_Object	//!< @alias{KVT_AddToObject_Object}
#define JSON_AddToObject_Array		KVT_AddToObject_Array	//!< @alias{KVT_AddToObject_Array}
#define JSON_AddToObject_Raw		KVT_AddToObject_Raw		//!< @alias{KVT_AddToObject_Raw}



/*
** ************************************************************************** *|
**                             JSON Check Operations                          *|
** ************************************************************************** *|
*/

#define JSON_IsInvalid	KVT_IsInvalid	//!< @alias{KVT_IsInvalid}
#define JSON_IsNull		KVT_IsNull		//!< @alias{KVT_IsNull}
#define JSON_IsBoolean	KVT_IsBoolean	//!< @alias{KVT_IsBoolean}
#define JSON_IsInteger	KVT_IsInteger	//!< @alias{KVT_IsInteger}
#define JSON_IsFloat	KVT_IsFloat		//!< @alias{KVT_IsFloat}
#define JSON_IsString	KVT_IsString	//!< @alias{KVT_IsString}
#define JSON_IsArray	KVT_IsArray		//!< @alias{KVT_IsArray}
#define JSON_IsObject	KVT_IsObject	//!< @alias{KVT_IsObject}
#define JSON_IsRaw		KVT_IsRaw		//!< @alias{KVT_IsRaw}



/*
** ************************************************************************** *|
**                             JSON Other Operations                           *|
** ************************************************************************** *|
*/

#define JSON_Delete 	KVT_Delete	//!< @alias{KVT_Delete}

#define JSON_Detach 	KVT_Detach	//!< @alias{KVT_Detach}

#define JSON_Replace 	KVT_Replace	//!< @alias{KVT_Replace}



#define JSON_Delete_FromArray 	KVT_Delete_FromArray	//!< @alias{KVT_Delete_FromArray}

#define JSON_Detach_FromArray 	KVT_Detach_FromArray	//!< @alias{KVT_Detach_FromArray}

#define JSON_Replace_InArray 	KVT_Replace_InArray		//!< @alias{KVT_Replace_InArray}

#define JSON_Insert_InArray 	KVT_Insert_InArray		//!< @alias{KVT_Insert_InArray}



#define JSON_Delete_FromObject \
			JSON_Delete_FromObject_IgnoreCase
#define JSON_Delete_FromObject_IgnoreCase		KVT_Delete_FromObject_IgnoreCase	//! @alias{KVT_Delete_FromObject_IgnoreCase}
#define JSON_Delete_FromObject_CaseSensitive	KVT_Delete_FromObject_CaseSensitive	//! @alias{KVT_Delete_FromObject_CaseSensitive}

#define JSON_Detach_FromObject \
			JSON_Detach_FromObject_IgnoreCase
#define JSON_Detach_FromObject_IgnoreCase		KVT_Detach_FromObject_IgnoreCase	//! @alias{KVT_Detach_FromObject_IgnoreCase}
#define JSON_Detach_FromObject_CaseSensitive	KVT_Detach_FromObject_CaseSensitive	//! @alias{KVT_Detach_FromObject_CaseSensitive}

#define JSON_Replace_InObject \
			JSON_Replace_InObject_IgnoreCase
#define JSON_Replace_InObject_IgnoreCase		KVT_Replace_InObject_IgnoreCase		//! @alias{KVT_Replace_InObject_IgnoreCase}
#define JSON_Replace_InObject_CaseSensitive		KVT_Replace_InObject_CaseSensitive	//! @alias{KVT_Replace_InObject_CaseSensitive}



/*! @} */
HEADER_END
#endif
