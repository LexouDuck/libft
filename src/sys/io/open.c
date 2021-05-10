
/*
**	Functions used from <unistd.h>:
**	-	int	open(const t_char* pathname, int flags, mode_t mode);
*/
#include <unistd.h>
#include <fcntl.h>

#include "libccc/sys/io.h"



// the following lines allow us to fix windows stupidities
#ifdef _WIN32
#define OPEN_BINARY		O_BINARY
#define OPEN_TEXT		O_TEXT
#else
#define OPEN_BINARY		0
#define OPEN_TEXT		0
#endif



inline t_fd	IO_Open(t_char const* filepath, t_io_open flags, t_io_mode mode)
{
	return (open(filepath, flags | OPEN_BINARY, mode));
}
