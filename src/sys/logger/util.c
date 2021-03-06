
#ifndef __NOSTD__
	#include <unistd.h>
#else
	char*	strerror(int error);
#endif
#ifndef __NOSTD__
	#include <errno.h>
#else
	#ifndef	errno
	#define errno	(*_errno())
	extern	int*	_errno(void);
	#endif
#endif
#ifndef __NOSTD__
	#include <stdarg.h>
#else
	typedef __builtin_va_list va_list;
	#define va_start(v,l)	__builtin_va_start(v,l)
	#define va_end(v)		__builtin_va_end(v)
	#define va_arg(v,l)		__builtin_va_arg(v,l)
	#define va_copy(d,s)	__builtin_va_copy(d,s)
#endif

#include "libccc.h"
#include "libccc/memory.h"
#include "libccc/string.h"
#include "libccc/stringarray.h"
#include "libccc/sys/time.h"
#include "libccc/sys/io.h"
#include "libccc/sys/logger.h"

#include LIBCONFIG_ERROR_INCLUDE



e_cccerror	Log_FatalError(s_logger const* logger, t_char const* str)
{
	t_size result = 0;
	t_char* message;

	HANDLE_ERROR(NULLPOINTER, (logger == NULL), return (ERROR_NULLPOINTER);)
	message = Error_STDC(errno);
	if (message == NULL)
		return (OK);
	if (logger->path && IO_IsTerminal(logger->fd))
	{
		if (str)	result = IO_Write_Format(logger->fd, C_RED"Fatal Error"C_RESET": %s\n\t-> %s\n", str, message);
		else		result = IO_Write_Format(logger->fd, C_RED"Fatal Error"C_RESET": %s\n",               message);
	}
	else
	{
		if (str)	result = IO_Write_Format(logger->fd,      "Fatal Error: %s\n\t-> %s\n",          str, message);
		else		result = IO_Write_Format(logger->fd,      "Fatal Error: %s\n",                        message);
	}
	String_Delete(&message);
	HANDLE_ERROR(SYSTEM, (result == 0), return (ERROR_PRINT);)
	return (result);
}



t_char*	Logger_GetTimestamp(t_time utc)
{
	static const t_size max = 24;
	t_char*	result;
	s_date	date;

	date = Time_ToDate_UTC(utc);
	result = String_New(max);
	HANDLE_ERROR(ALLOCFAILURE, (result == NULL), return (NULL);)
	Date_ToString_N(result, max, &date, LOG_TIMESTAMP_FORMAT);
	return (result);
}



//! Functions to help debug the logger
t_char*	Logger_GetSettings(s_logger const* logger)
{
	t_char*		result = NULL;
	t_char const*	logformat = NULL;

	HANDLE_ERROR(NULLPOINTER, (logger == NULL), return (NULL);)
	switch (logger->format)
	{
		case LOGFORMAT_ANSI: logformat = LOGFORMAT_STRING_ANSI; break;
		case LOGFORMAT_TEXT: logformat = LOGFORMAT_STRING_TEXT; break;
		case LOGFORMAT_JSON: logformat = LOGFORMAT_STRING_JSON; break;
		case LOGFORMAT_XML:  logformat = LOGFORMAT_STRING_XML ; break;
		default: break;
	}
	result = String_Format(
		"\n"LOG_TIMESTAMP_INDENT"\tLogger settings:"
		"\n"LOG_TIMESTAMP_INDENT"\t- Silence normal logs and warnings: %s"
		"\n"LOG_TIMESTAMP_INDENT"\t- Silence error logs: %s"
		"\n"LOG_TIMESTAMP_INDENT"\t- Show TimeStamp: %s"
		"\n"LOG_TIMESTAMP_INDENT"\t- Verbose mode: %s"
		"\n"LOG_TIMESTAMP_INDENT"\t- Obfuscated mode: %s"
		"\n"LOG_TIMESTAMP_INDENT"\t- Append mode: %s"
		"\n"LOG_TIMESTAMP_INDENT"\t- Format mode: %s"
		"\n"LOG_TIMESTAMP_INDENT"\t- Logging to fd: %d"
		"\n"LOG_TIMESTAMP_INDENT"\t- Logging to file: \"%s\"",
		(logger->silence_logs   ? "ON" : "OFF"),
		(logger->silence_errors ? "ON" : "OFF"),
		(logger->timestamp      ? "ON" : "OFF"),
		(logger->verbose        ? "ON" : "OFF"),
		(logger->obfuscated     ? "ON" : "OFF"),
		(logger->append         ? "ON" : "OFF"),
		logformat,
		logger->fd,
		logger->path
	);
	return (result);
}



e_cccerror	Logger_LogSettings(s_logger const* logger)
{
	e_cccerror result;
	t_char*	tmp;

	HANDLE_ERROR(NULLPOINTER, (logger == NULL), return (ERROR_NULLPOINTER);)
	tmp = Logger_GetSettings(logger);
	result = Log_Message(logger, "%s", tmp);
	String_Delete(&tmp);
	return (result);
}
