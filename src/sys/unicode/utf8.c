
#include "libccc/sys/unicode.h"
#include "libccc/pointer.h"

#include LIBCONFIG_HANDLE_INCLUDE



#define MASK	((1 << 6) - 1)



t_size		UTF32_ToUTF8(t_utf8* dest, t_utf32 c)
{
	t_u8 mask;

	HANDLE_ERROR(NULLPOINTER, (dest == NULL), return (0);)
	if (UTF32_IsValid(c))
	{
		if (c < UTF8_1BYTE)
		{
			dest[0] = (t_u8)c;
			return (1);
		}
		else if (c < UTF8_2BYTE)
		{
			mask = ((1 << 5) - 1);
			dest[0] = (mask & (c >> (6 * 1))) | 0xC0;
			dest[1] = (MASK & (c >> (6 * 0))) | 0x80;
			return (2);
		}
		else if (c < UTF8_3BYTE)
		{
			mask = ((1 << 4) - 1);
			dest[0] = (mask & (c >> (6 * 2))) | 0xE0;
			dest[1] = (MASK & (c >> (6 * 1))) | 0x80;
			dest[2] = (MASK & (c >> (6 * 0))) | 0x80;
			return (3);
		}
		else if (c <= UTF8_4BYTE)
		{
			mask = ((1 << 3) - 1);
			dest[0] = (mask & (c >> (6 * 3))) | 0xF0;
			dest[1] = (MASK & (c >> (6 * 2))) | 0x80;
			dest[2] = (MASK & (c >> (6 * 1))) | 0x80;
			dest[3] = (MASK & (c >> (6 * 0))) | 0x80;
			return (4);
		}
	}
	return (0); // INVALID UTF-8
}



t_utf32		UTF32_FromUTF8(t_utf8 const* str)
{
	t_u8 c;

	HANDLE_ERROR(NULLPOINTER, (str == NULL), return (ERROR);)
	c = str[0];
	if (c & (1 << 7)) // multi-byte character
	{
		t_utf32 result = 0;
		t_u8 mask;
		if (c & (1 << 6)) // 2-byte character
		{
			if (c & (1 << 5)) // 3-byte character
			{
				if (c & (1 << 4)) // 4-byte character
				{
					if (c & (1 << 3))
						return (ERROR); // INVALID UTF-8
					mask = ((1 << 3) - 1);
					result |= (c & mask) << (6 * 3);	c = str[1];
					result |= (c & MASK) << (6 * 2);	c = str[2];
					result |= (c & MASK) << (6 * 1);	c = str[3];
					result |= (c & MASK);
					return (result);
				}
				else
				{
					mask = ((1 << 4) - 1);
					result |= (c & mask) << (6 * 2);	c = str[1];
					result |= (c & MASK) << (6 * 1);	c = str[2];
					result |= (c & MASK);
					return (result);
				}
			}
			else
			{
				mask = ((1 << 5) - 1);
				result |= (c & mask) << (6 * 1);	c = str[1];
				result |= (c & MASK);
				return (result);
			}
		}
		else return (ERROR); // INVALID UTF-8
	}
	else return ((t_utf32)c);
}


/*
void	UTF32_Print_UTF8(t_utf8* dest, t_utf32 c)
{
	t_uint	utf8_length = 0;
	t_uint	utf8_position = 0;
	t_uint	sequence_length = 0;
	t_uint	first_byte_mark = 0;

	// encode as UTF-8 -> takes at maximum 4 bytes to encode:
	// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	if (codepoint < 0x80)
	{
		// normal ASCII, encoding 0xxxxxxx
		utf8_length = 1;
	}
	else if (codepoint < 0x800)
	{
		// two bytes, encoding 110xxxxx 10xxxxxx
		utf8_length = 2;
		first_byte_mark = (t_uint)0xC0; // 11000000
	}
	else if (codepoint < 0x10000)
	{
		// three bytes, encoding 1110xxxx 10xxxxxx 10xxxxxx
		utf8_length = 3;
		first_byte_mark = (t_uint)0xE0; // 11100000
	}
	else if (codepoint <= 0x10FFFF)
	{
		// four bytes, encoding 1110xxxx 10xxxxxx 10xxxxxx 10xxxxxx
		utf8_length = 4;
		first_byte_mark = (t_uint)0xF0; // 11110000
	}
	else
		goto failure; // invalid unicode codepoint

	// encode as utf8
	for (utf8_position = (t_uint)(utf8_length - 1); utf8_position > 0; utf8_position--)
	{
		// 10xxxxxx
		(*output_pointer)[(t_sint)utf8_position] = (t_uint)((codepoint | 0x80) & 0xBF);
		codepoint >>= 6;
	}
	// encode first byte
	if (utf8_length > 1)
	{
		(*output_pointer)[0] = (t_uint)((codepoint | first_byte_mark) & DYNAMICTYPE_MASK);
	}
	else
	{
		(*output_pointer)[0] = (t_uint)(codepoint & 0x7F);
	}
}
*/