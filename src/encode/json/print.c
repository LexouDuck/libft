
#include "libccc.h"
#include "libccc/char.h"
#include "libccc/string.h"
#include "libccc/memory.h"
#include "libccc/math/math.h"
#include "libccc/encode/json.h"

#include LIBCONFIG_ERROR_INCLUDE



typedef s_kvt_print	s_json_print;

#define ENSURE(NEEDED) \
	result = KVT_Print_EnsureBuffer(p, (NEEDED));	\
	if (result == NULL)								\
		return (ERROR);								\

#define JSON_Print_UpdateOffset \
		KVT_Print_UpdateOffset

#define JSON_NUMBER_BUFFERSIZE \
		KVT_NUMBER_BUFFERSIZE
	



static t_bool	JSON_Print_Number(s_json const* item, s_json_print* p, t_bool bigint);
static t_bool	JSON_Print_String(s_json const* item, s_json_print* p);
static t_bool	JSON_Print_Object(s_json const* item, s_json_print* p);
static t_bool	JSON_Print_Array (s_json const* item, s_json_print* p);
static t_bool	JSON_Print_Value (s_json const* item, s_json_print* p);



// Render the cstring provided to an escaped version that can be printed.
static
t_bool	JSON_Print_StringPtr(t_utf8 const* input, s_json_print* p)
{
	t_utf8 const* input_ptr = NULL;
	t_utf8* result = NULL;
	t_utf8* str;
	t_size result_length = 0;
	t_size escape_chars = 0; //!< amount of additional characters needed for escaping

	HANDLE_ERROR(NULLPOINTER, (p == NULL), return (ERROR);)
	// empty string
	if (input == NULL)
	{
		ENSURE(sizeof("\"\""))
		String_Copy(result, "\"\"");
		return (OK);
	}
	// set "flag" to 1 if something needs to be escaped
	input_ptr = input;
	while (*input_ptr != '\0')
	{
		t_utf32 c = UTF32_FromUTF8((t_utf8*)input_ptr);
		t_size length = 1;
			 if (c < UTF8_1BYTE)	length = 1;
		else if (c < UTF8_2BYTE)	length = 2;
		else if (c < UTF8_3BYTE)	length = 3;
		else if (c < UTF8_4BYTE)	length = 4;
		switch (*input_ptr)
		{
			case '\"':
			case '\\':
			case '\b':
			case '\f':
			case '\n':
			case '\r':
			case '\t':
			{	// one character escape sequence
				escape_chars++;
				break;
			}
			default:
			{
				if (!UTF32_IsPrintable(c))
				{
					escape_chars += 5;	// UTF-16 escape sequence \uXXXX
					if (c >= UTF8_3BYTE)
						escape_chars += 6;	// UTF-16 escape sequence \uXXXX\uXXXX
				}
				break;
			}
		}
		input_ptr += length;
	}
	result_length = (t_size)(input_ptr - input) + escape_chars;

	ENSURE(result_length + sizeof("\"\""))

	// no characters have to be escaped
	if (escape_chars == 0)
	{
		result[0] = '\"';
		Memory_Copy(result + 1, input, result_length);
		result[result_length + 1] = '\"';
		result[result_length + 2] = '\0';
		return (OK);
	}
	result[0] = '\"';
	str = result + 1;
	// copy the string
	input_ptr = input;
	while (*input_ptr != '\0')
	{
		t_utf32 c = UTF32_FromUTF8((t_utf8*)input_ptr);
		t_size length = 1;
			 if (c < UTF8_1BYTE)	length = 1;
		else if (c < UTF8_2BYTE)	length = 2;
		else if (c < UTF8_3BYTE)	length = 3;
		else if (c < UTF8_4BYTE)	length = 4;
		if (UTF32_IsPrintable(c) &&
			(c != '\"') &&
			(c != '\\'))
		{	// normal character, copy
			*str++ = *input_ptr;
			while (--length && *input_ptr != '\0')
			{
				input_ptr++;
				*str++ = *input_ptr;
			}
		}
		else // character needs to be escaped
		{
			*str++ = '\\';
			switch (c)
			{
				case '\b':	*str++ = 'b';	break;
				case '\t':	*str++ = 't';	break;
				case '\n':	*str++ = 'n';	break;
				case '\f':	*str++ = 'f';	break;
				case '\r':	*str++ = 'r';	break;
				case '\"':	*str++ = '\"';	break;
				case '\\':	*str++ = '\\';	break;
				default: // escape and print as unicode codepoint
				{
					t_utf16 u[2] = {0};
					length = UTF32_ToUTF16(u, c);
					if (length > 0)
					{
						String_Format_N(str, 6, "u%4.4X", u[0]);
						str += 5;
						if (u[1])
						{
							*str++ = '\\';
							String_Format_N(str, 6, "u%4.4X", u[1]);
							str += 5;
						}
					}
					break;
				}
			}
		}
		input_ptr++;
	}
	*str++ = '\"';
	*str++ = '\0';
	return (OK);
}

// Invoke JSON_Print_StringPtr (which is useful) on an item.
static
t_bool	JSON_Print_String(s_json const* item, s_json_print* p)
{
	return (JSON_Print_StringPtr((t_utf8*)item->value.string, p));
}

// Render the number nicely from the given item into a string.
static
t_bool	JSON_Print_Number(s_json const* item, s_json_print* p, t_bool bigint)
{
	t_utf8*	result = NULL;
	t_size	length = 0;
	t_size	i = 0;
	t_f64	test = 0.0;
	t_utf8	number_buffer[JSON_NUMBER_BUFFERSIZE] = {0}; // temporary buffer to print the number into

	HANDLE_ERROR(NULLPOINTER, (p == NULL), return (ERROR);)
	if (bigint) // TODO handle variable-length integers
	{
		t_s64	d = item->value.integer;
		length = String_Format_N(number_buffer, JSON_NUMBER_BUFFERSIZE, SF_S64"n", d);
	}
	else
	{
		t_f64	d = item->value.number;
		if (IS_NAN(d))
		{
			length = String_Format_N(number_buffer, JSON_NUMBER_BUFFERSIZE, "nan");
		}
		else if (IS_INF(d))
		{
			if (d > 0)	length = String_Format_N(number_buffer, JSON_NUMBER_BUFFERSIZE, "+inf");
			if (d < 0)	length = String_Format_N(number_buffer, JSON_NUMBER_BUFFERSIZE, "-inf");
		}
		else
		{
			// Try 15 decimal places of precision to avoid nonsignificant nonzero digits
			length = String_Format_N(number_buffer, JSON_NUMBER_BUFFERSIZE, "%1.15g", d);
			// Check whether the original t_f64 can be recovered
			test = F64_FromString(number_buffer);
			if (test != d)
			{
				// If not, print with 17 decimal places of precision
				length = String_Format_N(number_buffer, JSON_NUMBER_BUFFERSIZE, "%1.17g", d);
			}
		}
	}
	HANDLE_ERROR_SF(PRINT, (length == 0), return (ERROR);,
		": Could not print number value for item with key \"%s\"", item->key)
	HANDLE_ERROR_SF(PRINT, (length > (sizeof(number_buffer) - 1)), return (ERROR);,
		": Could not print number value for item with key \"%s\" -> buffer overrun occurred", item->key)
	// reserve appropriate space in the output
	ENSURE(length + sizeof(""))
	// copy the printed number to the output
	for (i = 0; i < length; ++i)
	{
		result[i] = number_buffer[i];
	}
	// TODO ? replace any locale-dependent decimal point with '.' (inspect whether printf/snprintf may output with other decimal point chars ?)
	result[i] = '\0';
	p->offset += length;
	return (OK);
}



// Render an array to text
static
t_bool	JSON_Print_Array(s_json const* item, s_json_print* p)
{
	t_utf8*	result = NULL;
	s_json*	current_item = item->value.child;
	t_size	length = 0;
	t_bool	multiline = p->format;

	HANDLE_ERROR(NULLPOINTER, (p == NULL), return (ERROR);)
	// Compose the output array.
	if (!(current_item && (current_item->next || current_item->prev != current_item)))
		multiline = FALSE;
	if (multiline &&
		p->buffer[p->offset - 1] == ' ' &&
		p->buffer[p->offset - 2] == ':')
	{
		ENSURE(p->depth + 1)
		*result++ = '\n';
		for (t_size i = 0; i < p->depth; i++)
		{
			*result++ = '\t';
		}
		p->offset += p->depth + 1;
	}
	// opening square bracket
	length = 1;//(t_size)(1 + (p->format && !multiline ? 1 : 0));
	ENSURE(length)
	*result++ = '[';
//	if (p->format && !multiline)
//		*result++ = ' ';
	p->offset += length;
	p->depth++;
	while (current_item != NULL)
	{
		if (multiline)
		{
			ENSURE(p->depth + 1)
			*result++ = '\n';
			for (t_size i = 0; i < p->depth; i++)
			{
				*result++ = '\t';
			}
			p->offset += p->depth + 1;
		}
		if (JSON_Print_Value(current_item, p))
			return (ERROR);
		JSON_Print_UpdateOffset(p);
		length = (t_size)(current_item->next ? 1 : 0);
		ENSURE(length + 1)
		if (current_item->next)
		{
			*result++ = ',';
		}
		*result = '\0';
		p->offset += length;
		current_item = current_item->next;
	}

	p->depth--;
	if (multiline)
	{
		ENSURE(p->depth + 1)
		*result++ = '\n';
		for (t_size i = 0; i < p->depth; ++i)
		{
			*result++ = '\t';
		}
		p->offset += p->depth + 1;
	}
	length = 2;//(t_size)(2 + (p->format && !multiline ? 1 : 0));
	ENSURE(length)
//	if (p->format && !multiline)
//		*result++ = ' ';
	*result++ = ']';
	*result = '\0';
	return (OK);
}



// Render an object to text.
static
t_bool	JSON_Print_Object(s_json const* item, s_json_print* p)
{
	t_utf8*	result = NULL;
	s_json*	current_item = item->value.child;
	t_size	length = 0;
	t_bool	multiline = p->format;

	HANDLE_ERROR(NULLPOINTER, (p == NULL), return (ERROR);)
	if (!(current_item && (current_item->next || current_item->prev != current_item)))
		multiline = FALSE;
	if (multiline && p->offset >= 2 &&
		p->buffer[p->offset - 1] == ' ' &&
		p->buffer[p->offset - 2] == ':')
	{
		ENSURE(p->depth + 1)
		*result++ = '\n';
		for (t_size i = 0; i < p->depth; i++)
		{
			*result++ = '\t';
		}
		p->offset += p->depth + 1;
	}
	// opening curly brace
	length = 1;//(t_size)(1 + (p->format && !multiline ? 1 : 0));
	ENSURE(length)
	*result++ = '{';
//	if (p->format && !multiline)
//		*result++ = ' ';
	p->offset += length;
	p->depth++;
	while (current_item)
	{
		if (multiline)
		{
			ENSURE(p->depth + 1)
			*result++ = '\n';
			for (t_size i = 0; i < p->depth; i++)
			{
				*result++ = '\t';
			}
			p->offset += p->depth + 1;
		}
		// print key
		if (JSON_Print_StringPtr(current_item->key, p))
			return (ERROR);
		JSON_Print_UpdateOffset(p);

		length = (t_size)(p->format ? 2 : 1);
		ENSURE(length)
		*result++ = ':';
		if (p->format)
		{
			*result++ = ' ';
		}
		p->offset += length;

		// print value
		if (JSON_Print_Value(current_item, p))
			return (ERROR);
		JSON_Print_UpdateOffset(p);

		// print comma if not last
		length = (t_size)(current_item->next ? 1 : 0);
		ENSURE(length + 1)
		if (current_item->next)
		{
			*result++ = ',';
		}
		*result = '\0';
		p->offset += length;
		current_item = current_item->next;
	}

	p->depth--;
	if (multiline)
	{
		ENSURE(p->depth + 1)
		*result++ = '\n';
		for (t_size i = 0; i < p->depth; ++i)
		{
			*result++ = '\t';
		}
		p->offset += p->depth + 1;
	}
	length = 2;//(t_size)(2 + (p->format && !multiline ? 1 : 0));
	ENSURE(length)
//	if (p->format && !multiline)
//		*result++ = ' ';
	*result++ = '}';
	*result = '\0';
	return (OK);
}



static
t_bool	JSON_Print_Value(s_json const* item, s_json_print* p)
{
	t_utf8* result = NULL;
	t_utf8 const* str;

	HANDLE_ERROR(NULLPOINTER, (p == NULL), return (ERROR);)
	HANDLE_ERROR(NULLPOINTER, (item == NULL), return (ERROR);)
	switch ((item->type) & DYNAMICTYPE_MASK)
	{
		case DYNAMICTYPE_INTEGER: return (JSON_Print_Number(item, p, TRUE));
		case DYNAMICTYPE_FLOAT:   return (JSON_Print_Number(item, p, FALSE));
		case DYNAMICTYPE_STRING:  return (JSON_Print_String(item, p));
		case DYNAMICTYPE_ARRAY:   return (JSON_Print_Array (item, p));
		case DYNAMICTYPE_OBJECT:  return (JSON_Print_Object(item, p));
		case DYNAMICTYPE_NULL:
		{
			str = "null";
			ENSURE(String_Length(str) + 1)
			String_Copy(result, str);
			return (OK);
		}
		case DYNAMICTYPE_BOOLEAN:
		{
			str = (item->value.boolean ? "true" : "false");
			ENSURE(String_Length(str) + 1)
			String_Copy(result, str);
			return (OK);
		}
		case DYNAMICTYPE_RAW:
		{
			t_size raw_length = 0;
			HANDLE_ERROR_SF(PRINT, (item->value.string == NULL), return (ERROR);,
				"Item with key \"%s\" is of 'raw string' type, but its value is null", item->key)
			raw_length = String_Length(item->value.string) + sizeof("");
			ENSURE(raw_length)
			Memory_Copy(result, item->value.string, raw_length);
			return (OK);
		}
		default:
			HANDLE_ERROR_SF(PRINT, (TRUE), return (ERROR);,
				"Cannot print item with key \"%s\", has invalid type (%i)", item->key, item->type)
	}
}



static
t_utf8*	JSON_Print_(s_json const* item, t_bool format)
{
	static const t_size default_buffer_size = 256;
	s_json_print p[1];
	t_utf8* printed = NULL;

	Memory_Clear(p, sizeof(p));
	// create buffer
	p->format = format;
	p->length = default_buffer_size;
	p->buffer = (t_utf8*)Memory_Allocate(default_buffer_size);
	HANDLE_ERROR(ALLOCFAILURE, (p->buffer == NULL), goto failure;)
	// print the value
	if (JSON_Print_Value(item, p))
		goto failure;
	JSON_Print_UpdateOffset(p);

#ifdef c_realloc // check if reallocate is available
	{
		printed = (t_utf8*)Memory_Reallocate(p->buffer, p->offset + 1);
		HANDLE_ERROR(ALLOCFAILURE, (printed == NULL), goto failure;)
		p->buffer = NULL;
	}
#else // otherwise copy the JSON over to a new buffer
	{
		printed = (t_utf8*)Memory_Allocate(p->offset + 1);
		HANDLE_ERROR(ALLOCFAILURE, (printed == NULL), goto failure;)
		Memory_Copy(printed, p->buffer, MIN(p->length, p->offset + 1));
		printed[p->offset] = '\0'; // just to be sure
		Memory_Free(p->buffer); // free the buffer
	}
#endif
	return (printed);

failure:
	if (p->buffer != NULL)
	{
		Memory_Free(p->buffer);
	}
	if (printed != NULL)
	{
		Memory_Free(printed);
	}
	return (NULL);
}



t_size	JSON_Print_Pretty(t_utf8* dest, s_json const* item, t_size n)
{
	s_json_print p = { 0 };

	HANDLE_ERROR(NULLPOINTER, (item == NULL), return (0);)
	if (n == 0)
		n = SIZE_MAX;
	p.buffer = (t_utf8*)dest;
	p.length = (t_size)n;
	p.offset = 0;
	p.noalloc = TRUE;
	p.format = TRUE;
	JSON_Print_Value(item, &p); // TODO error handling ?
	return (p.offset);
}

t_size	JSON_Print_Minify(t_utf8* dest, s_json const* item, t_size n)
{
	s_json_print p = { 0 };

	HANDLE_ERROR(NULLPOINTER, (item == NULL), return (0);)
	if (n == 0)
		n = SIZE_MAX;
	p.buffer = (t_utf8*)dest;
	p.length = (t_size)n;
	p.offset = 0;
	p.noalloc = TRUE;
	p.format = FALSE;
	JSON_Print_Value(item, &p); // TODO error handling ?
	return (p.offset);
}



t_utf8*	JSON_ToString_Pretty(s_json const* item)
{
	return (JSON_Print_(item, TRUE));
}

t_utf8*	JSON_ToString_Minify(s_json const* item)
{
	return (JSON_Print_(item, FALSE));
}



#if 0 // TODO
t_utf8*	JSON_Print_Buffered(s_json const* item, t_sint prebuffer, t_bool format)
{
	s_json_print p = { 0 };

	HANDLE_ERROR(LENGTH2SMALL, (prebuffer < 0), return (ERROR);)
	p.buffer = (t_utf8*)Memory_Allocate((t_size)prebuffer);
	HANDLE_ERROR(ALLOCFAILURE, (p.buffer == NULL), return (NULL);)
	p.length = (t_size)prebuffer;
	p.offset = 0;
	p.noalloc = FALSE;
	p.format = format;
	if (JSON_Print_Value(item, &p))
	{
		Memory_Free(p.buffer);
		return (NULL);
	}
	return ((t_utf8*)p.buffer);
}
#endif
