#pragma once
#include <PGN/Common/DllInterface.h>
namespace pgn {

class FileStream;

PGN_DLL_INTERFACE FileStream* createZipFileStream(const char workingDir[]);
PGN_DLL_INTERFACE void destroyZipFileStream(FileStream* f);

}
