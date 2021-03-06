
#include <string.h>

#include "test_catch.h"



char const*	signals[ENUMLENGTH_SIGNAL + 1] =
{
	"",
	"#SIGTERM",	//!< (SIGnal: TERMination) termination request, sent to the program
	"#SIGSEGV",	//!< (SIGnal: SEGmentation Violation) invalid memory access (segmentation fault)
	"#SIGINT",	//!< (SIGnal: INTerrupt) external interrupt, usually initiated by the user
	"#SIGILL",	//!< (SIGnal: ILLegal operation) invalid program image, such as invalid instruction
	"#SIGABRT",	//!< (SIGnal: ABoRT) abnormal termination condition, as is e.g. initiated by abort()
	"#SIGFPE",	//!< (SIGnal: Floating-Point Error) erroneous arithmetic operation such as divide by zero
	NULL
};



e_signal	sig;

jmp_buf	restore;

#ifndef __MINGW32__
struct sigaction signal_action;
#endif


/*
** ************************************************************************** *|
**                              Segfault Handling                             *|
** ************************************************************************** *|
*/

#ifdef __MINGW32__
void	signal_handler(int signaltype)
#else
void	signal_handler(int signaltype, siginfo_t *info, void *ptr)
#endif
{
	e_signal value;
	switch (signaltype)
	{
//		case SIGTERM:	value = SIGNAL_SIGTERM;	break;
		case SIGSEGV:	value = SIGNAL_SIGSEGV;	break;
		case SIGINT:	value = SIGNAL_SIGINT;	break;
		case SIGILL:	value = SIGNAL_SIGILL;	break;
		case SIGABRT:	value = SIGNAL_SIGABRT;	break;
		case SIGFPE:	value = SIGNAL_SIGFPE;	break;
		default:		value = SIGNAL_NULL;	break;
	}
	longjmp(restore, value);
}



void	init_segfault_handler(void)
{
#ifdef __MINGW32__
//	signal(SIGTERM,	signal_handler);
	signal(SIGSEGV,	signal_handler);
	signal(SIGINT,	signal_handler);
	signal(SIGILL,	signal_handler);
	signal(SIGABRT,	signal_handler);
	signal(SIGFPE,	signal_handler);
#else
	memset(&signal_action, 0, sizeof(sigaction));
	sigemptyset(&signal_action.sa_mask);
	signal_action.sa_flags     = SA_NODEFER;
	signal_action.sa_sigaction = signal_handler;
//	sigaction(SIGTERM,	&signal_action, NULL);
	sigaction(SIGSEGV,	&signal_action, NULL);
	sigaction(SIGINT,	&signal_action, NULL);
	sigaction(SIGILL,	&signal_action, NULL);
	sigaction(SIGABRT,	&signal_action, NULL);
	sigaction(SIGFPE,	&signal_action, NULL);
#endif
}
