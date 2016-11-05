#define PGN_DLL_EXPORT
#include <PGN/Platform/FileStream.h>
#undef PGN_DLL_EXPORT

#include <string.h>

bool pgn::isAbsolutePath(const char* path)
{
	return strlen(path) > sizeof("C:\\") && path[1] == ':' && path[2] == '\\';
}
