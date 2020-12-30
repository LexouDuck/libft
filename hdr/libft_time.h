/*============================================================================*/
/*                                            ______________________________  */
/*  libccc_time.h                            |    __    __  ___      _____  | */
/*                                           |   / /\  / /\/ . |\   /  __|\ | */
/*  https://github.com/LexouDuck/libccc.git  |  / /_/ / / / . <_/  |  /___| | */
/*                                           | /___/\/_/ /___-'\   \____/\  | */
/* Comprehensive|Cross-platform|Customizable | \____/\__/\____-'    \____/  | */
/* This source code follows the MIT License. |______________________________| */
/*                                                                            */
/*============================================================================*/

#ifndef __LIBFT_TIME_H
#define __LIBFT_TIME_H
/*! @file libft_time.h
**	This header defines all the functions for date & time related functions.
**	@addtogroup libft_time
**	@{
*/

/*
** ************************************************************************** *|
**                                   Includes                                 *|
** ************************************************************************** *|
*/

#include <time.h>
/*
#ifndef __APPLE__
	#define _XOPEN_SOURCE 600
#endif
#include <sys/time.h>
*/
#include "libft.h"

HEADER_CPP

/*
** ************************************************************************** *|
**                                 Definitions                                *|
** ************************************************************************** *|
*/

//! The standard type used to represent a date+time (smallest unit is seconds)
/*!
**	This type stores integer standard UNIX epoch time, so that means that a value of zero
**	corresponds to midnight (00:00:00), January 1, 1970, UTC (Coordinated Universal Time)
*/
typedef time_t		t_time;

#define TIME_NULL	((t_time)0)



//! This struct is used to store a precise, arbitrary timestamp (smallest unit is nanoseconds)
/*!
**	This 'timespec' has an arbitrary begin point (typically the start of program
**	execution, but it can be the last machine boot, among many other things).
**	It is meant to be used measure intervals of time during program execution,
**	by calling the 'clock()' function several times and comparing their outputs.
**	
**	This struct is equivalent to the ISO C library's 'struct timespec',
**	although it's sub-fields have different names:
**		tv_sec	->	sec
**		tv_nsec	->	nanosec
*/
typedef struct	s_timespec_
{
	t_time		sec;
	t_s64		nanosec;
}				s_timespec;
// typedef struct timespec	s_timespec;

#define TIMESPEC_NULL	((s_timespec){0})



//! This enum lists each day of the week, Sunday counting as zero
typedef enum	e_weekday_
{
	WEEKDAY_SUNDAY,
	WEEKDAY_MONDAY,
	WEEKDAY_TUESDAY,
	WEEKDAY_WEDNESDAY,
	WEEKDAY_THURSDAY,
	WEEKDAY_FRIDAY,
	WEEKDAY_SATURDAY,
	ENUMLENGTH_WEEKDAY
}				e_weekday;

#define WEEKDAY_SUN		WEEKDAY_SUNDAY
#define WEEKDAY_MON		WEEKDAY_MONDAY
#define WEEKDAY_TUE		WEEKDAY_TUESDAY
#define WEEKDAY_WED		WEEKDAY_WEDNESDAY
#define WEEKDAY_THU		WEEKDAY_THURSDAY
#define WEEKDAY_FRI		WEEKDAY_FRIDAY
#define WEEKDAY_SAT		WEEKDAY_SATURDAY

//! This global constant stores all the string names of the WEEKDAY enum, in lowercase
extern char const* const g_weekday[ENUMLENGTH_WEEKDAY];
//! This global constant stores all the string names of the WEEKDAY enum, abbreviated to 3 letters
extern char const* const g_weekday_abbreviated[ENUMLENGTH_WEEKDAY];



//! This enum lists each month of the year, January counting as zero
typedef enum	e_month_
{
	MONTH_JANUARY,
	MONTH_FEBRUARY,
	MONTH_MARCH,
	MONTH_APRIL,
	MONTH_MAY,
	MONTH_JUNE,
	MONTH_JULY,
	MONTH_AUGUST,
	MONTH_SEPTEMBER,
	MONTH_OCTOBER,
	MONTH_NOVEMBER,
	MONTH_DECEMBER,
	ENUMLENGTH_MONTH
}				e_month;

#define MONTH_JAN	MONTH_JANUARY
#define MONTH_FEB	MONTH_FEBRUARY
#define MONTH_MAR	MONTH_MARCH
#define MONTH_APR	MONTH_APRIL
#define MONTH_MAY	MONTH_MAY
#define MONTH_JUN	MONTH_JUNE
#define MONTH_JUL	MONTH_JULY
#define MONTH_AUG	MONTH_AUGUST
#define MONTH_SEP	MONTH_SEPTEMBER
#define MONTH_OCT	MONTH_OCTOBER
#define MONTH_NOV	MONTH_NOVEMBER
#define MONTH_DEC	MONTH_DECEMBER

//! This global constant stores all the string names of the MONTH enum, in lowercase
extern char const* const g_month[ENUMLENGTH_MONTH];
//! This global constant stores all the string names of the MONTH enum, abbreviated to 3 letters
extern char const* const g_month_abbreviated[ENUMLENGTH_MONTH];



//! This signed int represents a timezone difference (in seconds)
/*!
**	This signed int is based on the UTC time system - it expresses a time difference, in seconds.
**	The idea is to notate timezones in the commonly accepted UTC format (eg: UTC+01, UTC-06, etc)
**	There are several defines below that are meant to be used within this type:
**	@see TIMEZONE_GMT,
**		 TIMEZONE_EST,
**		 TIMEZONE_CST,
**		 TIMEZONE_MST,
**		 TIMEZONE_PST,
**		 TIMEZONE_EDT,
**		 TIMEZONE_CDT,
**		 TIMEZONE_MDT,
**		 TIMEZONE_PDT,
*/
typedef t_s32	t_timezone;

// Commonly used timezones
#define TIMEZONE_UTC(X)	(3600 * X)			//!< Timezone (UTC 00): Universal Coordinated Time (same as GMT)
#define TIMEZONE_GMT	TIMEZONE_UTC(0)		//!< Timezone (UTC 00): Greenwich Mean Time (same as UTC)
#define TIMEZONE_UTCMIN	TIMEZONE_UTC(-12)	//!< Timezone (UTC-12): The minimum UTC timezone (most late)
#define TIMEZONE_UTCMAX	TIMEZONE_UTC(+12)	//!< Timezone (UTC+12): The maximum UTC timezone (most early)
// NAST
#define TIMEZONE_EST	TIMEZONE_UTC(-5)	//!< Timezone (UTC-05): North American Eastern Standard Time
#define TIMEZONE_CST	TIMEZONE_UTC(-6)	//!< Timezone (UTC-06): North American Central Standard Time
#define TIMEZONE_MST	TIMEZONE_UTC(-7)	//!< Timezone (UTC-07): North American Mountain Standard Time
#define TIMEZONE_PST	TIMEZONE_UTC(-8)	//!< Timezone (UTC-08): North American Pacific Standard Time
// NADT
#define TIMEZONE_EDT	TIMEZONE_UTC(-4)	//!< Timezone (UTC-04): North American Eastern Daylight Time
#define TIMEZONE_CDT	TIMEZONE_UTC(-5)	//!< Timezone (UTC-05): North American Central Daylight Time
#define TIMEZONE_MDT	TIMEZONE_UTC(-6)	//!< Timezone (UTC-06): North American Mountain Daylight Time
#define TIMEZONE_PDT	TIMEZONE_UTC(-7)	//!< Timezone (UTC-07): North American Pacific Daylight Time



//! This struct is used to store all aspects about a certain date/time (equivalent to 'struct tm')
/*!
**	This struct is equivalent to the ISO C library's 'struct tm',
**	although it's sub-fields have different names:
**		tm_sec	 ->	sec
**		tm_min	 ->	min
**		tm_hour	 ->	hour
**		tm_mday	 ->	day_month
**		tm_mon	 ->	month
**		tm_year	 ->	year
**		tm_wday	 ->	day_week
**		tm_yday	 ->	day_year
**		tm_isdst ->	is_dst
**	NB: This struct does not store timezone information, there are conversion functions for that:
**	@see	Time_To_Date_Timezone, Date_To_Time_Timezone
*/
typedef struct	s_date_
{
	t_u8		sec;		//!< [0,59(61)] seconds after the minute (usually 0-59 - there is extra range to accommodate for leap seconds)
	t_u8		min;		//!< [0,59] minutes after the hour
	t_u8		hour;		//!< [0,23] hours since midnight
	e_weekday	day_week;	//!< [0,6] days since Sunday
	t_u8		day_month;	//!< [1,31] day of the month
	t_s32		day_year;	//!< [0,365] days since January 1 (max value is 365 every 4 years, otherwise 364)
	e_month		month;		//!< [0,11] months since January
	t_s32		year;		//!< Amount of years since 1900	
	t_bool		is_dst;		//!< If TRUE, then Daylight Savings Time is on
}				s_date;
// typedef struct tm	s_time

#define DATE_NULL	((s_date){0})



#define FORMAT_TIME_UNIX		"%H:%M:%S"
#define FORMAT_TIME_REVERSE		"%S:%M:%H"

#define FORMAT_DATE_UNIX	"%Y-%m-%d"
#define FORMAT_DATE_YMD		"%Y/%m/%d"
#define FORMAT_DATE_DMY		"%d/%m/%Y"
#define FORMAT_DATE_MDY		"%m/%d/%Y"



#define TIME_MAX_SECONDS	(60)	//!< The amount of seconds in a minute
#define TIME_MAX_MINUTES	(60)	//!< The amount of minutes in an hour
#define TIME_MAX_HOURS		(24)	//!< The amount of hours in a day
#define TIME_MAX_DAYS_MONTH	(31)	//!< The amount of days in a month
#define TIME_MAX_DAYS_YEAR	(365)	//!< The amount of days in a year

#define TIME_MAX_LEAP_SECONDS	(62)	//!< The amount of seconds in a minute (accounting for leap seconds)
#define TIME_MAX_LEAP_DAYS_YEAR	(366)	//!< The amount of days in a year (when accounting for leap 4 years)



/*
** ************************************************************************** *|
**                           Time integer functions                           *|
** ************************************************************************** *|
*/

//! Returns the current UTC timestamp, according to the system clock
t_time					Time_Now(void);
#define ft_time			Time_Now
#define ft_time_now		Time_Now



//! Converts the given 't_time value' to its equivalent 's_date' representation (in UTC)
s_date							Time_To_Date_UTC(t_time const value);
#define ft_time_to_date			Time_To_Date_UTC
#define ft_time_to_date_utc		Time_To_Date_UTC
#define Time_To_Date			Time_To_Date_UTC

//! Converts the given 't_time value' to its equivalent 's_date' representation (according to the system timezone)
s_date							Time_To_Date_LocalTime(t_time const value);
#define ft_time_to_date_local	Time_To_Date_LocalTime



/*
** ************************************************************************** *|
**                s_timespec: Precise arbitrary time functions                *|
** ************************************************************************** *|
*/

//! Converts the given 's_date' struct to its ISO STD LIBC 'struct tm' equivalent
struct timespec				Timespec_To_STDC(s_timespec const* value);
#define ft_timespec_to_stdc	Timespec_To_STDC

//! Converts the given ISO STD LIBC 'struct tm' to its 's_date' struct equivalent
s_timespec					STDC_To_Timespec(struct timespec const* value);
#define ft_stdc_to_timespec	STDC_To_Timespec



/*
** ************************************************************************** *|
**                       t_timezone: Timezone functions                       *|
** ************************************************************************** *|
*/

//! Returns the value of the given 't_time value', updating the timezone offset from 'old' to 'new'
/*
**	Assuming the given 'value' has a timezone offset of 'old', then the
**	returned value will have a timezone offset of 'new'.
*/
t_time					Time_SetTimezone(t_time value, t_timezone old, t_timezone new);
#define ft_time_tzset	Time_SetTimezone

//! Returns the value of the given 't_time value', updating the timezone offset from 'old' to 'new'
/*
**	Assuming the given 'value' has a timezone offset of 'old', then the
**	returned value will have a timezone offset of 'new'.
*/
s_date					Date_SetTimezone(s_date const* value, t_timezone old, t_timezone new);
#define ft_date_tzset	Date_SetTimezone



/*
** ************************************************************************** *|
**                          Date structure functions                          *|
** ************************************************************************** *|
*/

//! Returns the current UTC timestamp, according to the system clock
s_date					Date_Now(void);
#define ft_date			Date_Now
#define ft_date_now		Date_Now



//! Parses the given string representation of a date/time, and returns the resulting 's_date' struct
s_date						Date_String_Parse(char const* str, char const* format);// __format_strptime(2, 0); TODO check if this exists even
#define ft_strptime			Date_String_Parse
#define ft_date_parse		Date_String_Parse
#define Date_Parse			Date_String_Parse
#define String_To_Date		Date_String_Parse

//! Creates a string representation of the given 'date', according to the given 'format' string
/*!
**	This function works similarly to the strftime() function from 'time.h' STDC header
**	It is closer to 'asprintf()' as well, making for a rather easy-to-use equivalent to strftime().
**	That being said, it is probably better to use Date_String_Format_N for machines with little RAM.
*/
char*						Date_String_Format(s_date const* date, char const* format) __format_strftime(2);
#define ft_date_format		Date_String_Format
#define Date_Format			Date_String_Format
//! @copydef Date_String_Format, but this version is more closely equivalent to 'strftime()'
t_size						Date_String_Format_N(char* dest, t_size max, s_date const* date, char const* format) __format_strftime(4);
#define ft_strftime			Date_String_Format_N
#define ft_date_format_n	Date_String_Format_N
#define Date_Format_N		Date_String_Format_N



//! Converts the given 's_date value' to its equivalent 't_time' representation (in UTC)
t_time							Date_To_Time_UTC(s_date const* value);
#define ft_date_to_time			Date_To_Time_UTC
#define ft_date_to_time_utc		Date_To_Time_UTC
#define Date_To_Time			Date_To_Time_UTC

//! Converts the given 's_date value' to its equivalent 't_time' representation (according to the system timezone)
t_time							Date_To_Time_LocalTime(s_date const* value);
#define ft_date_to_time_local	Date_To_Time_LocalTime



//! Converts the given 's_date' struct to its ISO STD LIBC 'struct tm' equivalent
struct tm					Date_To_STDC(s_date const* date);
#define ft_date_to_stdc		Date_To_STDC

//! Converts the given ISO STD LIBC 'struct tm' to its 's_date' struct equivalent
s_date						STDC_To_Date(struct tm const* value);
#define ft_stdc_to_date		STDC_To_Date



/*! @} */
HEADER_END
#endif
