#ifndef NDEBUG
#	ifdef _MSC_VER
#	define _CRTDBG_MAP_ALLOC
#	include <stdlib.h>
#	include <crtdbg.h>
#	define debug_new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#	endif
#endif

#ifndef debug_new
#define debug_new new
#endif
