
// make all generic functions be defined as 'static', so that DCE can occur on any compiler
#undef _GENERIC
#if (defined(__GNUC__) || defined(__llvm__))
	#define _GENERIC()		__attribute__((unused))	static
#elif defined(__MSVC__)
	#pragma warning(push)
	#pragma warning(disable : 4505)
	#define _GENERIC()		static
#else
	#define _GENERIC()		static
#endif

// force re-inclusion of header (with the current generic type `T`)
#undef __LIBCCC_MONAD_TREE_H
#include "libccc/monad/tree.h"

// generate code for all generic functions
//#include "libccc/../../src/monad/tree/new.c"

// remove all generic type macros
#undef	tree_T
#undef	s_tree_T

#undef T
#undef T_NAME
#undef T_DEFAULT
#undef T_EQUALS

// redefine the _GENERIC() macro as empty, so everything works as normal again
#undef _GENERIC
#if (defined(__GNUC__) || defined(__llvm__))
	#define _GENERIC()	
#elif defined(__MSVC__)
	#pragma warning(pop)
	#define _GENERIC()	
#else
	#define _GENERIC()	
#endif
