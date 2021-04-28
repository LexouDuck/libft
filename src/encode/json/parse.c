
#include "libccc.h"
#include "libccc/char.h"
#include "libccc/string.h"
#include "libccc/memory.h"
#include "libccc/sys/io.h"
#include "libccc/encode/json.h"



typedef struct json_parse
{
	s_json*		result;		//!< the result JSON
	char const*	content;	//!< the string to parse
	t_size		length;		//!< the length of the string to parse
	t_bool		strict;		//!< if TRUE, strict parsing mode is on (rigourously follows the spec)
	t_size		offset;		//!< current parsing offset
	t_uint		depth;		//!< current section nesting level
	t_size		line;		//!< current line number
}			s_json_parse;



static t_bool JSON_Parse_Value(s_json* const item, s_json_parse* const p);
static t_bool JSON_Parse_Number(s_json* const item, s_json_parse* const p);
static t_bool JSON_Parse_String(s_json* const item, s_json_parse* const p);
static t_bool JSON_Parse_Array(s_json* const item, s_json_parse* const p);
static t_bool JSON_Parse_Object(s_json* const item, s_json_parse* const p);



#define PARSINGERROR_JSON	"Error while parsing JSON: "

//! Checks if the buffer can be accessed at the given index
#define CAN_PARSE(p, i) \
	(((p)->content != NULL) && (((p)->offset + i) <= (p)->length))



inline
t_bool	Char_IsSpace_JSON(t_utf32 c)
{
	return ((c == ' ') ||
			(c == '\t') ||
			(c == '\r') ||
			(c == '\n'));
}

//! Utility to jump whitespace and CR/LF
static
s_json_parse*	JSON_Parse_SkipWhiteSpace(s_json_parse* const p, t_bool skip_comments)
{
	t_size i;

	LIBCONFIG_HANDLE_NULLPOINTER(NULL, p)
	LIBCONFIG_HANDLE_NULLPOINTER(NULL, p->content)
	while (CAN_PARSE(p, 0))
	{
		if (skip_comments && p->content[p->offset] == '/')
		{
			if (p->content[p->offset] == '*') // multiple-line
			{
				for (i = 0; (p->offset + i < p->length); ++i)
				{
					if (p->content[p->offset + i] == '*')
					{
						++i;
						if (p->content[p->offset + i] == '/')
							break;
					}
				}
				if (p->offset + i == p->length)
					LIBCONFIG_HANDLE_PARSINGERROR(NULL, PARSINGERROR_JSON "input ends unexpectedly, expected end of comment block \"*/\"")
				p->offset += i;
			}
			else if (p->content[p->offset] == '/') // single-line
			{
				for (i = 0; (p->offset + i < p->length); ++i)
				{
					if (p->content[p->offset + i] == '\n')
						break;
				}
				p->offset += i;
			}
		}
		if (p->strict ?
			!Char_IsSpace_JSON(p->content[p->offset]) :
			!Char_IsSpace(p->content[p->offset]))
			break;
		p->offset++;
	}
	if (p->offset == p->length)
	{
		p->offset--;
	}
	return (p);
}



//! skip the UTF-8 BOM (byte order mark) if it is at the beginning of a buffer
static
s_json_parse*	JSON_Parse_SkipUTF8BOM(s_json_parse* const p)
{
	LIBCONFIG_HANDLE_NULLPOINTER(NULL, p)
	LIBCONFIG_HANDLE_NULLPOINTER(NULL, p->content)
	LIBCONFIG_HANDLE_INDEX2SMALL(NULL, p->offset, 0)
	if (CAN_PARSE(p, 4) && (String_Compare_N((t_char const*)&p->content[p->offset], UTF8_BOM, 3) == 0))
	{
		p->offset += 3;
	}
	return (p);
}



//! Parse the input text to generate a number, and populate the result into item.
static
t_bool		JSON_Parse_Number(s_json* const item, s_json_parse* const p)
{
	t_f64 number = 0;
	t_char number_c_string[64];
	t_size i = 0;

	for (i = 0; (i < (sizeof(number_c_string) - 1)) && CAN_PARSE(p, i); i++)
	{
		if (Char_IsDigit(p->content[p->offset + i]) ||
			p->content[p->offset] == '+' ||
			p->content[p->offset] == '-' ||
			p->content[p->offset] == 'e' ||
			p->content[p->offset] == 'E' ||
			p->content[p->offset] == '.')
			number_c_string[i] = p->content[p->offset + i];
		else break;
	}
	number_c_string[i] = '\0';

	number = F64_FromString((t_char const*)number_c_string);
	if (IS_NAN(number_c_string))
		return (FALSE); // JSON_Parse_Error
	item->value.number = number;
	item->type = DYNAMIC_TYPE_FLOAT;
	p->offset += i;
	return (TRUE);
}



// Parse the input text into an unescaped cinput, and populate item.
static t_bool JSON_Parse_String(s_json* const item, s_json_parse* const p)
{
	t_char const* input_pointer = &p->content[p->offset] + 1;
	t_char const* input_end = &p->content[p->offset] + 1;
	t_char* output_pointer = NULL;
	t_char* output = NULL;

	// not a string
	if (p->content[p->offset] != '\"')
		goto failure;

	{
		// calculate approximate size of the output (overestimate)
		t_size allocation_length = 0;
		t_size skipped_bytes = 0;
		while (((t_size)(input_end - p->content) < p->length) && (*input_end != '\"'))
		{
			// is escape sequence
			if (input_end[0] == '\\')
			{
				if ((t_size)(input_end + 1 - p->content) >= p->length)
				{
					// prevent buffer overflow when last input character is a backslash
					goto failure;
				}
				skipped_bytes++;
				input_end++;
			}
			input_end++;
		}
		if (((t_size)(input_end - p->content) >= p->length) || (*input_end != '\"'))
		{
			goto failure; // string ended unexpectedly
		}

		// This is at most how much we need for the output
		allocation_length = (t_size) (input_end - &p->content[p->offset]) - skipped_bytes;
		output = (t_char*)Memory_Alloc(allocation_length + sizeof(""));
		if (output == NULL)
		{
			goto failure; // allocation failure
		}
	}
	output_pointer = output;
	// loop through the string literal
	while (input_pointer < input_end)
	{
		if (*input_pointer == '\\') // escape sequence
		{
			t_char sequence_length = 2;
			if ((input_end - input_pointer) < 1)
				goto failure;
			switch (input_pointer[1])
			{
				case 'b':	*output_pointer++ = '\b';	break;
				case 'f':	*output_pointer++ = '\f';	break;
				case 'n':	*output_pointer++ = '\n';	break;
				case 'r':	*output_pointer++ = '\r';	break;
				case 't':	*output_pointer++ = '\t';	break;
				case '\"':
				case '\\':
				case '/':
					*output_pointer++ = input_pointer[1];
					break;
				case 'u': // UTF-16 literal
					t_utf32 c;
					sequence_length = Char_Parse_Unicode_N(&c, input_pointer, (input_end - input_pointer));
					if (sequence_length == 0)
						goto failure; // failed to convert UTF16-literal to UTF-8
					output_pointer += Char_ToUTF8(output_pointer, c);
					break;
				default:
					goto failure;
			}
			input_pointer += sequence_length;
		}
		else *output_pointer++ = *input_pointer++;
	}
	// zero terminate the output
	*output_pointer = '\0';
	item->type = DYNAMIC_TYPE_STRING;
	item->value.string = output;
	p->offset = (t_size)(input_end - p->content);
	p->offset++;
	return (TRUE);

failure:
	if (output != NULL)
	{
		Memory_Free(output);
	}
	if (input_pointer != NULL)
	{
		p->offset = (t_size)(input_pointer - p->content);
	}
	return (FALSE);
}



static
t_bool	JSON_Parse_Array(s_json* const item, s_json_parse* const p)
{
	s_json* head = NULL; // head of the linked list
	s_json* current_item = NULL;

	if (p->depth >= KVT_NESTING_LIMIT)
		return (FALSE); // too deeply nested
	p->depth++;

	if (p->content[p->offset] != '[')
		goto failure; // not an array
	p->offset++;
	JSON_Parse_SkipWhiteSpace(p, p->strict);
	if (CAN_PARSE(p, 0) && (p->content[p->offset] == ']'))
		goto success; // empty array
	// check if we skipped to the end of the buffer
	if (!CAN_PARSE(p, 0))
	{
		p->offset--;
		goto failure;
	}
	// step back to character in front of the first element
	p->offset--;
	// loop through the comma separated array elements
	do
	{
		// allocate next item
		s_json* new_item = JSON_Item();
		if (new_item == NULL)
			goto failure; // allocation failure
		// attach next item to list
		if (head == NULL)
		{	// start the linked list
			current_item = head = new_item;
		}
		else
		{
			// add to the end and advance
			current_item->next = new_item;
			new_item->prev = current_item;
			current_item = new_item;
		}
		// p next value
		p->offset++;
		JSON_Parse_SkipWhiteSpace(p, p->strict);
		if (!JSON_Parse_Value(current_item, p))
			goto failure; // failed to p value
		JSON_Parse_SkipWhiteSpace(p, p->strict);
	}
	while (CAN_PARSE(p, 0) && (p->content[p->offset] == ','));

	if (!CAN_PARSE(p, 0) || p->content[p->offset] != ']')
		goto failure; // expected end of array

success:
	p->depth--;

	if (head != NULL)
	{
		head->prev = current_item;
	}

	item->type = DYNAMIC_TYPE_ARRAY;
	item->value.child = head;

	p->offset++;
	return (TRUE);

failure:
	if (head != NULL)
	{
		JSON_Delete(head);
	}
	return (FALSE);
}



static
t_bool	JSON_Parse_Object(s_json* const item, s_json_parse* const p)
{
	s_json* head = NULL; // linked list head
	s_json* current_item = NULL;

	if (p->depth >= KVT_NESTING_LIMIT)
		return (FALSE); // reached nesting depth limit
	p->depth++;
	if (!CAN_PARSE(p, 0) || (p->content[p->offset] != '{'))
		goto failure; // not an object
	p->offset++;
	JSON_Parse_SkipWhiteSpace(p, p->strict);
	if (CAN_PARSE(p, 0) && (p->content[p->offset] == '}'))
		goto success; // empty object
	// check if we skipped to the end of the buffer
	if (!CAN_PARSE(p, 0))
	{
		p->offset--;
		goto failure;
	}
	// step back to character in front of the first element
	p->offset--;
	// loop through the comma separated array elements
	do
	{
		// allocate next item
		s_json* new_item = JSON_Item();
		if (new_item == NULL)
			goto failure; // allocation failure
		// attach next item to list
		if (head == NULL)
		{	// start the linked list
			current_item = head = new_item;
		}
		else
		{	// add to the end and advance
			current_item->next = new_item;
			new_item->prev = current_item;
			current_item = new_item;
		}
		// p the name of the child
		p->offset++;
		JSON_Parse_SkipWhiteSpace(p, p->strict);
		if (!JSON_Parse_String(current_item, p))
			goto failure; // failed to p name
		JSON_Parse_SkipWhiteSpace(p, p->strict);
		// swap value.string and string, because we parsed the name
		current_item->key = current_item->value.string;
		current_item->value.string = NULL;
		if (!CAN_PARSE(p, 0) || (p->content[p->offset] != ':'))
			goto failure; // invalid object
		// p the value
		p->offset++;
		JSON_Parse_SkipWhiteSpace(p, p->strict);
		if (!JSON_Parse_Value(current_item, p))
			goto failure; // failed to p value
		JSON_Parse_SkipWhiteSpace(p, p->strict);
	}
	while (CAN_PARSE(p, 0) && (p->content[p->offset] == ','));

	if (!CAN_PARSE(p, 0) || (p->content[p->offset] != '}'))
		goto failure; // expected end of object

failure:
	if (head != NULL)
	{
		JSON_Delete(head);
	}
	return (FALSE);

success:
	p->depth--;

	if (head != NULL)
	{
		head->prev = current_item;
	}

	item->type = DYNAMIC_TYPE_OBJECT;
	item->value.child = head;

	p->offset++;
	return (TRUE);
}



static
t_bool	JSON_Parse_Value(s_json* const item, s_json_parse* const p)
{
	if ((p == NULL) || (p->content == NULL))
		return (FALSE); // no input

	// p the different types of values
	if (CAN_PARSE(p, 4) && (String_Equals_N((t_char const*)&p->content[p->offset], "null", 4) ||
		p->strict ? 0 :		String_Equals_N((t_char const*)&p->content[p->offset], "NULL", 4)))
	{	// null
		item->type = DYNAMIC_TYPE_NULL;
		p->offset += 4;
		return (TRUE);
	}
	if (CAN_PARSE(p, 5) && (String_Equals_N((t_char const*)&p->content[p->offset], "false", 5) &&
		p->strict ? 0 :		String_Equals_N((t_char const*)&p->content[p->offset], "FALSE", 5)))
	{	// FALSE
		item->type = DYNAMIC_TYPE_BOOLEAN;
		item->value.boolean = FALSE;
		p->offset += 5;
		return (TRUE);
	}
	if (CAN_PARSE(p, 4) && (String_Equals_N((t_char const*)&p->content[p->offset], "true", 4) &&
		p->strict ? 0 :		String_Equals_N((t_char const*)&p->content[p->offset], "TRUE", 4)))
	{	// TRUE
		item->type = DYNAMIC_TYPE_BOOLEAN;
		item->value.boolean = TRUE;
		p->offset += 4;
		return (TRUE);
	}
	if (CAN_PARSE(p, 0) && (p->content[p->offset] == '\"'))
	{	// string
		return (JSON_Parse_String(item, p));
	}
	if (CAN_PARSE(p, 0) && (Char_IsDigit(p->content[p->offset]) ||
							(p->content[p->offset] == '-') ||
		(p->strict ? 0 :	(p->content[p->offset] == '+'))))
	{	// number
		return (JSON_Parse_Number(item, p));
	}
	if (CAN_PARSE(p, 0) && (p->content[p->offset] == '['))
	{	// array
		return (JSON_Parse_Array(item, p));
	}
	if (CAN_PARSE(p, 0) && (p->content[p->offset] == '{'))
	{	// object
		return (JSON_Parse_Object(item, p));
	}
	return (FALSE);
}



static
s_json*	JSON_Parse_(t_char const* json, t_size buffer_length, t_bool strict, t_char const** return_parse_end)
{
	s_json_parse p = { 0 };
	s_json* result = NULL;

	LIBCONFIG_HANDLE_LENGTH2SMALL(NULL, buffer_length, 1)
	p.content = (t_char const*)json;
	p.length = buffer_length; 
	p.offset = 0;
	p.strict = strict;
	result = JSON_Item();
	if (result == NULL)
		goto failure; // memory failure
	if (!JSON_Parse_Value(result, JSON_Parse_SkipWhiteSpace(JSON_Parse_SkipUTF8BOM(&p), !p.strict)))
		goto failure; // p failure. ep is set.
	if (p.strict)
	{	// if we require null-terminated JSON without appended garbage, skip and then check for a null terminator
		JSON_Parse_SkipWhiteSpace(&p, !p.strict);
		if ((p.offset >= p.length) || (&p.content[p.offset])[0] != '\0')
			goto failure;
	}
	if (return_parse_end)
	{
		*return_parse_end = (t_char const*)(&p.content[p.offset]);
	}
	return (result);

failure:
	if (result != NULL)
	{
		JSON_Delete(result);
	}
	if (json != NULL)
	{
		t_size	position = 0;

		if (p.offset < p.length)
			position = p.offset;
		else if (p.length > 0)
			position = p.length - 1;

		if (return_parse_end != NULL)
		{
			*return_parse_end = (t_char const*)(json + position);
		}
	}
	return (NULL);
}



s_json*	JSON_Parse(t_char const* json)
{
	LIBCONFIG_HANDLE_NULLPOINTER(NULL, json)
	return (JSON_Parse_(json, String_Length(json), FALSE, NULL));
}

s_json*	JSON_Parse_N(t_char const* json, t_size maxlength)
{
	LIBCONFIG_HANDLE_NULLPOINTER(NULL, json)
	return (JSON_Parse_(json, maxlength, FALSE, NULL));
}

s_json*	JSON_ParseStrict(t_char const* json, t_char const** return_parse_end)
{
	LIBCONFIG_HANDLE_NULLPOINTER(NULL, json)
	return (JSON_Parse_(json, String_Length(json), TRUE, return_parse_end));
}

s_json*	JSON_ParseStrict_N(t_char const* json, t_size maxlength, t_char const** return_parse_end)
{
	LIBCONFIG_HANDLE_NULLPOINTER(NULL, json)
	return (JSON_Parse_(json, maxlength, TRUE, return_parse_end));
}
