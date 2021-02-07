
#include "libccc/char.h"



inline t_bool	Char_IsDigit(t_utf32 c)
{
	return ('0' <= c && c <= '9');
}



inline t_bool	Char_IsDigit_Hex(t_utf32 c)
{
	return (('0' <= c && c <= '9') ||
			('A' <= c && c <= 'F') ||
			('a' <= c && c <= 'f'));
}



inline t_bool	Char_IsDigit_Oct(t_utf32 c)
{
	return (('0' <= c && c <= '7'));
}



inline t_bool	Char_IsDigit_Bin(t_utf32 c)
{
	return (c == '0' || c == '1');
}



inline t_bool	Char_IsAlphaNumeric(t_utf32 c)
{
	return (('A' <= c && c <= 'Z') ||
			('a' <= c && c <= 'z') ||
			('0' <= c && c <= '9'));
}
