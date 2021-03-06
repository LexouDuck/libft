
#ifndef __NOSTD__
	#include <stdarg.h>
#else
	typedef __builtin_va_list va_list;
	#define va_start(v,l)	__builtin_va_start(v,l)
	#define va_end(v)		__builtin_va_end(v)
	#define va_arg(v,l)		__builtin_va_arg(v,l)
	#define va_copy(d,s)	__builtin_va_copy(d,s)
#endif

#include "libccc/memory.h"
#include "libccc/monad/array.h"

#include LIBCONFIG_ERROR_INCLUDE

/* ++C fix for variadic arg type promotion:
#if (#typeof(T) is #float)
	#define T_VA_ARG	double
#elif (#typeof(T) is #char)
	#define T_VA_ARG	int
#elif (#typeof(T) is #short)
	#define T_VA_ARG	int
#else
	#define T_VA_ARG	T
#endif
*/

_GENERIC()
s_array_T*	CONCAT(Array_Create,T_NAME)(t_uint n, ...)
{
	va_list		args;
	T			item;
	s_array_T*	result;

	result = (s_array_T*)Memory_Allocate(sizeof(s_array_T));
	HANDLE_ERROR(ALLOCFAILURE, (result == NULL), return (result);)
	result->length = n;
	result->items = NULL;
	if (n == 0)
		return (result);
	result->items = (T*)Memory_Allocate(sizeof(T) * n);
	HANDLE_ERROR(ALLOCFAILURE, (result->items == NULL), return (result);)
	va_start(args, n);
	for (t_uint i = 1; i <= n; ++i)
	{
		item = va_arg(args, T);
		result->items[i - 1] = item;
	}
	va_end(args);
	return (result);
}
