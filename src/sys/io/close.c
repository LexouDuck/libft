
#ifndef __NOSTD__
	#include <unistd.h>
	#include <fcntl.h>
#else
	int	close(int fd);
#endif
#ifndef __NOSTD__
	#include <errno.h>
#else
	#ifndef	errno
	#define errno	(*_errno())
	extern	int*	_errno(void);
	#endif
#endif

#include "libccc/sys/io.h"

#include LIBCONFIG_HANDLE_INCLUDE



inline
t_io_error	IO_Close(t_fd fd)
{
	return (close(fd) ? errno : OK);
}
