
#include <stdlib.h>
#include <time.h>

#include "test.h"
#include "test_timer.h"



#ifdef __MINGW32__
	#define TIMER_FORMATSTRING	"%I64d.%.09ld"
#else
	#define TIMER_FORMATSTRING	"%lld.%.09ld"
#endif

/*	Define a 10e9 macro we use for nanosecond modulo */
#define BILLION 1000000000L



/*
** ************************************************************************** *|
**                           Timer utility functions                          *|
** ************************************************************************** *|
*/

/*	There are a couple of different cross-platform clock possibilities:
**	- CLOCK_REALTIME	System-wide real-time clock. This clock returns the number of seconds and nanoseconds since the Epoch.
**						This clock can be set via clock_settime but doing so requires appropriate privileges, because it's linked to the system time.
**	- CLOCK_MONOTONIC 			Represents monotonic time since some unspecified starting point. This clock cannot be set.
**	- CLOCK_PROCESS_CPUTIME_ID 	High-resolution per-process timer from the CPU.
**	- CLOCK_THREAD_CPUTIME_ID 	Thread-specific CPU-time clock. 
*/
inline
void	timer_clock(s_time* result)
{
	if (clock_gettime(CLOCK_MONOTONIC, result) == -1)
	{
		perror("clock_gettime() returned -1");
		exit(EXIT_FAILURE);
	}
}



inline
s_time	timer_getdiff(s_time start, s_time end)
{
	s_time result;
	
	result.tv_sec = end.tv_sec - start.tv_sec;
	result.tv_nsec = end.tv_nsec - start.tv_nsec;
	if (result.tv_nsec >= BILLION)
	{
		result.tv_nsec -= BILLION;
		result.tv_sec++;
	}
	return (result);
}



/* Returns negative if 'a' is lower than 'b', positive if 'b' > 'a' and 0 if equal. */
inline
t_s64	timer_compare(s_time a, s_time b)
{
	if (a.tv_sec == b.tv_sec)
		return (a.tv_nsec - b.tv_nsec);
	else
		return (a.tv_sec - b.tv_sec);
}



/* prints the result of a timer (and potentially a comparison with the secondary timer) */
void	print_timer_result(s_timer* t, t_s64 compare)
{
	char result1[64] = { 0 };
	char result2[64] = { 0 };

	if (!g_test.flags.verbose || !g_test.flags.show_speed)
		return;

	t->time1 = timer_getdiff(t->start1, t->end1);
	if (t->time1.tv_nsec < 0 || t->time1.tv_nsec < 0)
		sprintf((char*)&result1, "SEGV");
	else sprintf((char*)&result1, TIMER_FORMATSTRING, (long long)t->time1.tv_sec, t->time1.tv_nsec);

	printf(" [libccc:");
	if (compare)
	{
		t->time2 = timer_getdiff(t->start2, t->end2);
		if (t->time2.tv_nsec < 0 || t->time2.tv_nsec < 0)
			sprintf((char*)&result2, "SEGV");
		else sprintf((char*)&result2, TIMER_FORMATSTRING, (long long)t->time2.tv_sec, t->time2.tv_nsec);

		if ((t->time1.tv_sec >= 0 && t->time1.tv_nsec >= 0) ||
			(t->time2.tv_sec >= 0 && t->time2.tv_nsec >= 0))
			compare = timer_compare(t->time1, t->time2);
		else compare = 0;

		if (compare == 0)
			printf("%s, libc:", result1);
		else if (compare < 0)
			printf(C_GREEN"%s"C_RESET", libc:", result1);
		else
			printf(C_RED"%s"C_RESET", libc:", result1);

		printf("%s]", result2);
	}
	else printf("%s]", result1);
}
