/*============================================================================*/
/*                                            ______________________________  */
/*  libccc/char.h                            |    __    __  ___      _____  | */
/*                                           |   / /\  / /\/ . |\   /  __|\ | */
/*  https://github.com/LexouDuck/libccc.git  |  / /_/ / / / . <_/  |  /___| | */
/*                                           | /___/\/_/ /___-'\   \____/\  | */
/* Comprehensive|Cross-platform|Customizable | \____/\__/\____-'    \____/  | */
/* This source code follows the MIT License. |______________________________| */
/*                                                                            */
/*============================================================================*/

#ifndef __LIBCCC_CHAR_H
#define __LIBCCC_CHAR_H
/*!@group{libccc_char}
** @{
**	This header defines all the functions/sets related to single ASCII chars.
**
**	@isostd{https://en.cppreference.com/w/c/language/ascii}
**	- https://en.wikipedia.org/wiki/UTF-8
**	- https://en.wikipedia.org/wiki/UTF-16
**	- https://en.wikipedia.org/wiki/UTF-32
**
**	@file
*/

/*
** ************************************************************************** *|
**                                   Includes                                 *|
** ************************************************************************** *|
*/

#include "libccc_config.h"
#include "libccc_naming.h"
#include "libccc_define.h"

#include "libccc/bool.h"
#include "libccc/int.h"
#include "libccc/pointer.h"

HEADER_CPP

/*
** ************************************************************************** *|
**                                 Definitions                                *|
** ************************************************************************** *|
*/

//! Primitive type: `char`, ie: a text character (1 byte: ANSI/ASCII)
/*!
**	@isostd{https://en.cppreference.com/w/c/string/byte}
*/
typedef char	t_char;
TYPEDEF_ALIAS(	t_char, CHAR, PRIMITIVE)

//! Primitive type: Unicode UTF-8 character (variable length: 1 to 4 bytes)
/*!
**	@isostd{https://en.cppreference.com/w/c/string/multibyte}
*/
typedef char		t_utf8;
TYPEDEF_ALIAS(		t_utf8, UTF8, PRIMITIVE)

//! Primitive type: Unicode UTF-16 character (variable length: 2 or 4 bytes)
/*!
**	@isostd{https://en.cppreference.com/w/c/string/multibyte/char16_t}
**	@isostd{https://en.cppreference.com/w/c/string/multibyte}
*/
typedef uint_least16_t	t_utf16; // TODO add option to use C11 `char16_t`
TYPEDEF_ALIAS(			t_utf16, UTF16, PRIMITIVE)

//! Primitive type: Unicode UTF-32 character code point value (fixed length: 4 bytes)
/*!
**	@isostd{https://en.cppreference.com/w/c/string/multibyte/char32_t}
**	@isostd{https://en.cppreference.com/w/c/string/wide}
*/
typedef uint_least32_t	t_utf32; // TODO add option to use C11 `char32_t`
TYPEDEF_ALIAS(			t_utf32, UTF32, PRIMITIVE)

// TODO add option with wrapper functions for the ones in C11 <uchar.h>


/*
**	Define string literals for alphabet characters
*/
//! Set of characters for the lowercase latin alphabet
#define CHARSET_LOWERCASE	"abcdefghijklmnopqrstuvwxyz"
//! Set of characters for the uppercase latin alphabet
#define CHARSET_UPPERCASE	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//! Set of characters for the latin alphabet (both lowercase and uppercase)
#define CHARSET_ALPHABET	"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ"

/*
**	Define string literals for some of the commonly used number bases.
*/
#define CHARSET_BINARY		"01"
#define CHARSET_OCTAL		"01234567"
#define CHARSET_DECIMAL		"0123456789"
#define CHARSET_HEX_LOWER	"0123456789abcdef"
#define CHARSET_HEX_UPPER	"0123456789ABCDEF"
#define CHARSET_HEX			CHARSET_HEXADECIMAL
#define CHARSET_HEXADECIMAL	"0123456789aAbBcCdDeEfF"

//! A charset containing all of the different whitespace characters
#define CHARSET_WHITESPACE	" \t\n\r\v\f"

//! A charset containing all of the ASCII punctuation characters
#define CHARSET_PUNCTUATION	"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"



/*!
**	The Unicode byte order mark (https://en.wikipedia.org/wiki/Byte_order_mark) macros.
**	A byte order mark is used at the beginning of a string, to indicate that the string is
**	indeed a Unicode string, the type of Unicode encoding, and the byte endian-ness used.
*/
//!@{
//! The UTF-8 byte order mark: `EF BB BF`
#define UTF8_BOM	"\xEF\xBB\xBF"

//! The UTF-16 byte order mark: `FE FF` (Big-Endian)
#define UTF16_BOM_BE	"\xFE\xFF"
//! The UTF-16 byte order mark: `FF FE` (Little-Endian)
#define UTF16_BOM_LE	"\xFF\xFE"

//! The UTF-16 byte order mark: `00 00 FE FF` (Big-Endian)
#define UTF32_BOM_BE	"\x00\x00\xFE\xFF"
//! The UTF-16 byte order mark: `FF FE 00 00` (Little-Endian)
#define UTF32_BOM_LE	"\xFF\xFE\x00\x00"
//!@}



/*!
**	These macros are used to encode/decode UTF-16 strings.
*/
//!@{
#define UTF16_BIAS		0x10000
#define UTF16_SURROGATE_HI	0xD800
#define UTF16_SURROGATE_LO	0xDC00
#define UTF16_SURROGATE_END	0xE000
#define UTF16_SURROGATE_MASK	0x3FF
//!@}



/*
** ************************************************************************** *|
**                              Character Checks                              *|
** ************************************************************************** *|
*/

//! Check if the given char `c` is a letter (lowercase or uppercase)
/*!
**	Check if the given char `c` is a letter (lowercase or uppercase)
**
**	@param	c	The character to check (ASCII or Unicode)
**	@returns 1(TRUE) if the given char `c` is a letter (lowercase or uppercase),
**			and returns 0(FALSE) otherwise.
*/
t_bool					Char_IsLetter(t_utf32 c);
#define c_isalpha		Char_IsLetter
#define Char_IsAlpha	Char_IsLetter

//! Check if the given char `c` is a uppercase letter character
/*!
**	Check if the given char `c` is a uppercase letter character
**
**	@param	c	The character to check (ASCII or Unicode)
**	@returns 1(TRUE) if the given char `c` is a uppercase letter character,
**			and returns 0(FALSE) otherwise.
*/
t_bool					Char_IsUppercase(t_utf32 c);
#define c_isupper		Char_IsUppercase
#define Char_IsUpper	Char_IsUppercase

//! Check if the given char `c` is an lowercase letter character
/*!
**	Check if the given char `c` is an lowercase letter character
**
**	@param	c	The character to check (ASCII or Unicode)
**	@returns 1(TRUE) if the given char `c` is an lowercase letter character,
**			and returns 0(FALSE) otherwise.
*/
t_bool					Char_IsLowercase(t_utf32 c);
#define c_islower		Char_IsLowercase
#define Char_IsLower	Char_IsLowercase

//! Check if the given char `c` is alphanumeric (letter or digit) character
/*!
**	Check if the given char `c` is alphanumeric (letter or digit) character
**
**	@param	c	The character to check (ASCII or Unicode)
**	@returns 1(TRUE) if the given char `c` is alphanumeric (letter or digit),
**			and returns 0(FALSE) otherwise.
*/
t_bool					Char_IsAlphaNumeric(t_utf32 c);
#define c_isalnum		Char_IsAlphaNumeric
#define Char_IsAlNum	Char_IsAlphaNumeric

//! Check if the given char `c` is a decimal digit character
/*!
**	Check if the given char `c` is a decimal digit character
**
**	@param	c	The character to check (ASCII or Unicode)
**	@returns 1(TRUE) if the given char `c` is a numerical digit character,
**			and returns 0(FALSE) otherwise.
*/
t_bool							Char_IsDigit(t_utf32 c);
#define c_isdigit				Char_IsDigit
#define Char_IsDigit			Char_IsDigit
#define Char_IsDigit_Dec		Char_IsDigit
#define Char_IsDigit_Decimal	Char_IsDigit

//! Check if the given char `c` is a hexadecimal digit character (0-9 or A-F, lowercase or uppercase)
t_bool								Char_IsDigit_Hex(t_utf32 c);
#define c_isdigit_hex				Char_IsDigit_Hex
#define Char_IsDigit_Hexadecimal	Char_IsDigit_Hex

//! Check if the given char `c` is a octal-base digit character (ie: '0' to '7')
t_bool								Char_IsDigit_Oct(t_utf32 c);
#define c_isdigit_oct				Char_IsDigit_Oct
#define Char_IsDigit_Octal			Char_IsDigit_Oct

//! Check if the given char `c` is a binary-base digit character (ie: '0' or '1')
t_bool								Char_IsDigit_Bin(t_utf32 c);
#define c_isdigit_bin				Char_IsDigit_Bin
#define Char_IsDigit_Binary			Char_IsDigit_Bin

//! Check if the given char `c` is whitespace character (' ','\\t','\\r','\\n','\\f','\\v')
/*!
**	Check if the given char `c` is whitespace character (' ','\\t','\\r','\\n','\\f','\\v')
**
**	@param	c	The character to check (ASCII or Unicode)
**	@returns 1(TRUE) if the given char `c` is whitespace character: ' ','\\t','\\r','\\n','\\f','\\v'
**			(carriage return, newline, form feed, vertical tab, tab, or space), and returns 0(FALSE) otherwise.
*/
t_bool						Char_IsSpace(t_utf32 c);
#define c_isspace			Char_IsSpace
#define Char_IsWhiteSpace	Char_IsSpace

//! Check if the given char `c` is a punctuation character
/*!
**	Check if the given char `c` is a punctuation character
**
**	@param	c	The character to check (ASCII or Unicode)
**	@returns 1(TRUE) if the given char `c` is a punctuation character,
**			and returns 0(FALSE) otherwise.
*/
t_bool					Char_IsPunctuation(t_utf32 c);
#define c_ispunct		Char_IsPunctuation
#define Char_IsPunct	Char_IsPunctuation

//! Check if the given char `c` is a printable ASCII character
/*!
**	Check if the given char `c` is a printable ASCII character
**
**	@param	c	The character to check (ASCII or Unicode)
**	@returns 1(TRUE) if the given char `c` is a printable ASCII character,
**			(0x20 <= c && c <= 0x7E) and returns 0(FALSE) otherwise.
*/
t_bool					Char_IsPrintable(t_utf32 c);
#define c_isprint		Char_IsPrintable
#define Char_IsPrint	Char_IsPrintable

//! Check if the given char `c` is an ASCII-compatible char (non-Unicode)
/*!
**	Check if the given char `c` is an ASCII-compatible char (non-Unicode)
**
**	@param	c	The character to check (ASCII or Unicode)
**	@returns 1(TRUE) if the given char `c` has a value between 0 and 127,
**			(0x00 <= c && c <= 0x7F) and returns 0(FALSE) otherwise.
*/
t_bool				Char_IsASCII(t_utf32 c);
#define c_isascii	Char_IsASCII



//! Check if the given char `c` is contained in the given 'charset'
/*!
**	Check if the given char `c` is contained in the given 'charset'
**
**	@param	c		The character to check (ASCII or Unicode)
**	@param	charset	String, the set of characters to look through (the '\0' terminator ends the charset)
**	@returns 1(TRUE) if the given char `c` is contained within the given `charset` array
*/
//!@{
t_bool					Char_IsInCharset(t_utf32 c, t_char const* charset);
#define c_isincharset	Char_IsInCharset
/* TODO
t_bool						Char_IsInCharset_UTF8(t_utf32 c, t_utf8 const* charset);
#define c_isinutf8charset	Char_IsInCharset_UTF8
*/
/* TODO
t_bool						Char_IsInCharset_UTF16(t_utf32 c, t_utf16 const* charset);
#define c_isinutf16charset	Char_IsInCharset_UTF16
*/
//!@}



/*
** ************************************************************************** *|
**                           Character Substitutions                          *|
** ************************************************************************** *|
*/

//! Get the uppercase version of the given char `c`
/*!
**	Get the uppercase version of the given char `c`
**
**	@param	c	The character to uppercase-ify (ASCII only)
**	@returns the uppercase equivalent of the given character `c`
**			(nothing is done if `c` is not a letter character).
*/
t_char					Char_ToUppercase(t_char c); // TODO refactor `t_char` here to `t_utf32` (breaking change)
#define c_toupper		Char_ToUppercase
#define Char_ToUpper	Char_ToUppercase

//! Get the lowercase version of the given char `c`
/*!
**	Get the lowercase version of the given char `c`
**
**	@param	c	The character to lowercase-ify (ASCII only)
**	@returns the lowercase equivalent of the given character `c`
**			(nothing is done if `c` is not a letter character).
*/
t_char					Char_ToLowercase(t_char c); // TODO refactor `t_char` here to `t_utf32` (breaking change)
#define c_tolower		Char_ToLowercase
#define Char_ToLower	Char_ToLowercase

// TODO add Char_ToMixedCase



/*
** ************************************************************************** *|
**                             Encoding Conversions                           *|
** ************************************************************************** *|
*/

// TODO UTF8_Length(), to mirror the mblen() function



//! Converts the given Unicode code point value `c` to its UTF-8 equivalent representation
/*!
**	@isostd{https://en.cppreference.com/w/c/string/multibyte/wctomb}
**
**	Converts the given char `c` to UTF-8 representation, and writes it to 'dest'
**	@param	c		The character to convert (UTF-32 Unicode code point value)
**	@param	dest	The pointer at which to write the resulting UTF-8 char
**					(you must ensure that at least 4 bytes are writable to 'dest')
**	@returns The size (in bytes) of the character written (can be 0, 1, 2, 3, or 4)
*/
t_size					Char_ToUTF8(t_utf8* dest, t_utf32 c);
#define c_wctomb		Char_ToUTF8
#define c_ctoutf8		Char_ToUTF8

//! Converts the given Unicode code point value `c` to its UTF-16 equivalent representation
/*!
**	@isostd{https://en.cppreference.com/w/c/string/multibyte/wctomb}
**
**	Converts the given char `c` to UTF-16 representation, and writes it to 'dest'
**	@param	c		The character to convert (UTF-32 Unicode code point value)
**	@param	dest	The pointer at which to write the resulting UTF-16 char
**					(you must ensure that at least 4 bytes are writable to 'dest')
**	@returns The size (in bytes) of the character written (can be 0, 2, or 4)
*/
t_size					Char_ToUTF16(t_utf16* dest, t_utf32 c);
#define c_wctomb2		Char_ToUTF16
#define c_ctoutf16		Char_ToUTF16



//! Returns the Unicode code point value (UTF-32) for the UTF-8 character pointed to by 'str'
/*!
**	@isostd{https://en.cppreference.com/w/c/string/multibyte/mbtowc}
**
**	Reads up to 4 bytes in 'str', and converts these bytes to a Unicode code point value
**	@param	str		The string from which to read a UTF-8 character
**	@returns The Unicode code point value for the character pointed to by 'str'
*/
t_utf32					Char_FromUTF8(t_utf8 const* str);
#define c_mbtowc		Char_FromUTF8
#define c_utf8toc		Char_FromUTF8
#define UTF8_Get		Char_FromUTF8

//! Returns the Unicode code point value (UTF-32) for the UTF-16 character pointed to by 'str'
/*!
**	@isostd{https://en.cppreference.com/w/c/string/multibyte/mbtowc}
**
**	Reads 2 or 4 bytes in 'str', and converts these bytes to a Unicode code point value
**	@param	str		The string from which to read a UTF-16 character
**	@returns The Unicode code point value for the character pointed to by 'str'
*/
t_utf32					Char_FromUTF16(t_utf16 const* str);
#define c_mb2towc		Char_FromUTF16
#define c_utf16toc		Char_FromUTF16
#define UTF16_Get		Char_FromUTF16



//! Parses a `\U????????`, or `\u????` or `\u????\u????` UTF-8 string escape sequence
/*!
**	@param	str	The string from which to parse an escape sequence
**	@returns the UTF-32 code point for the parsed unicode character,
**		or #ERROR (`-1`) if there was a parsing error.
*/
t_utf32		Char_Parse_Unicode(t_char const* str);

//! Parses a `\U????????`, or `\u????` or `\u????\u????` UTF-8 string escape sequence, reading at most `n` chars from `str`
/*!
**	@param	dest	The pointer to the 32-bit integer in which the result will be written.
**	@param	str		The string from which to parse an escape sequence.
**	@param	n		The maximum amount of characters to read from `str`.
**		NOTE: if `0` is given, then there will be no maximum amount.
**	@returns the total amount of bytes read from the given `str` buffer.
*/
t_size		Char_Parse_Unicode_N(t_utf32* dest, t_char const* str, t_size n);



/*! @} */
HEADER_END
#endif
