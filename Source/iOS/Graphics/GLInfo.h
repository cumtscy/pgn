#include <PGN/Common/debug_new.h>
#include <PGN/Platform/Graphics/GL.h>

class GLInfo : public pgn::GLInfo
{
    int refCount;
public:
    void addRef();
    void release();
};

extern GLInfo glInfo;
