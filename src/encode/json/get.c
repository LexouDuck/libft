
#include <stdarg.h>

#include "libccc.h"
#include "libccc/char.h"
#include "libccc/string.h"
#include "libccc/sys/io.h"
#include "libccc/encode/json.h"



#define PARSE_JSONPATH_WHITESPACE(EXPECTED, ERRORMESSAGE) \
	while (str[i] && Char_IsSpace(str[i]))	{ ++i; }				\
	if (str[i] == '\0')												\
	{																\
		IO_Output_Format(C_RED"JSON PATH PARSE ERROR:"C_RESET		\
			"Unexpected end of accessor string, "					\
			"expected %s "ERRORMESSAGE".\n",						\
			EXPECTED);												\
		return (result);											\
	}																\

#define PARSE_JSONPATH_MATCH_CHAR(CHAR, ERRORMESSAGE) \
	if (str[i] != CHAR)												\
	{																\
		IO_Output_Format(C_RED"JSON PATH PARSE ERROR:"C_RESET		\
			"Expected t_char '%c' "ERRORMESSAGE", "					\
			"but instead found: '%c'\n",							\
			CHAR, str[i]);											\
		return (result);											\
	}																\
	else ++i;														\

#define PARSE_JSONPATH_MATCH_STRING(STRING, ERRORMESSAGE) \
	if (String_Compare_N(str + i, STRING, String_Length(STRING)))	\
	{																\
		IO_Output_Format(C_RED"JSON PATH PARSE ERROR:"C_RESET		\
			"Expected \"%s\" "ERRORMESSAGE", "						\
			"but instead found: '%.16s'\n",							\
			STRING, str);											\
		return (result);											\
	}																\
	else ++i;														\

_FORMAT(printf, 2, 3)
s_json*	JSON_Get(s_json const* object, t_char const* format_path, ...)
{
	va_list args;
	t_char*	path;
	t_char*	str;
	s_json*	result = NULL;
	t_size	length;
	t_char*	accessor;

	if (object == NULL || format_path == NULL)
		return (NULL);
	va_start(args, format_path);
	path = String_Format_VA(format_path, args);
	va_end(args);
	if (path == NULL)
		return (NULL);
	result = (s_json*)object;
	str = path;
	for (t_size i = 0; str[i]; ++i)
	{
		PARSE_JSONPATH_WHITESPACE("'['", "to begin accessor")
		PARSE_JSONPATH_MATCH_CHAR( '[',  "to begin accessor")
		PARSE_JSONPATH_WHITESPACE("number or string value", "accessor")
		if (Char_IsDigit(str[i]))
		{	// number accessor
			length = 1;
			while (Char_IsDigit(str[i + length]))
			{
				++length;
			}
			accessor = String_Sub(str, i, length);
			t_u64 index = U64_FromString(accessor);
			result = JSON_GetArrayItem(result, index);
		}
		else if (str[i] == '\"')
		{	// string accessor
			PARSE_JSONPATH_MATCH_CHAR('"', "to begin string accessor")
			length = 1;
			while (str[i + length] != '\"')
			{
				if (str[i + length] == '\0')
				{
					IO_Output_Line("Unexpected end of accessor string, expected a closing double-quote '\"' t_char");
					return (result);
				}
				++length;
			}
			PARSE_JSONPATH_MATCH_CHAR('"', "to end string accessor")
			accessor = String_Sub(str, i, length);
			result = JSON_GetObjectItem(result, accessor);
		}
		else
		{
			IO_Output_Format(C_RED"JSON PATH PARSE ERROR"C_RESET": "
				"Expected number or double-quoted string within brackets, but instead found: '%s'\n", str);
			return (result);
		}
		String_Delete(&accessor);
		PARSE_JSONPATH_WHITESPACE("']'", "to end accessor")
		PARSE_JSONPATH_MATCH_CHAR( ']',  "to end accessor")
	}
	return (result);
}



t_f64	JSON_GetValue_Number(s_json const* const item) 
{
	if (!JSON_IsNumber(item)) 
		return ((t_f64)NAN);
	return (item->value_number);
}

t_char*	JSON_GetValue_String(s_json const* const item) 
{
	if (!JSON_IsString(item)) 
		return (NULL);
	return (item->value_string);
}



s_json*	JSON_GetArrayItem(s_json const* array, t_sint index)
{
	s_json* current_child;

	if (index < 0)
		return (NULL);
	if (array == NULL)
		return (NULL);
	current_child = array->child;
	while ((current_child != NULL) && (index > 0))
	{
		index--;
		current_child = current_child->next;
	}
	return (current_child);
}



static s_json* get_object_item(s_json const* const object, t_char const* const name, const t_bool case_sensitive)
{
	s_json* current_element = NULL;

	if ((object == NULL) || (name == NULL))
		return (NULL);

	current_element = object->child;
	if (case_sensitive)
	{
		while ((current_element != NULL) && (current_element->key != NULL) && (String_Compare(name, current_element->key) != 0))
		{
			current_element = current_element->next;
		}
	}
	else
	{
		while ((current_element != NULL) && (String_Compare_IgnoreCase(name, current_element->key) != 0))
		{
			current_element = current_element->next;
		}
	}
	if ((current_element == NULL) || (current_element->key == NULL))
		return (NULL);
	return (current_element);
}

s_json*	JSON_GetObjectItem(s_json const* const object, t_char const* const key)
{
	return (get_object_item(object, key, FALSE));
}

s_json*	JSON_GetObjectItem_CaseSensitive(s_json const* const object, t_char const* const key)
{
	return (get_object_item(object, key, TRUE));
}

t_bool	JSON_HasObjectItem(s_json const* object, t_char const* key)
{
	return (JSON_GetObjectItem(object, key) ? 1 : 0);
}
