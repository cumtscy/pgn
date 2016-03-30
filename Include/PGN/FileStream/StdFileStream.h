#pragma once
#include <PGN/Common/DllInterface.h>
namespace pgn {

class FileStream;

PGN_DLL_INTERFACE FileStream* createStdFileStream(const char workingDir[]);
PGN_DLL_INTERFACE void destroyStdFileStream(FileStream* f);

}
