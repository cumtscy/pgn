#define PGN_DLL_EXPORT
#include <PGN/Common/Blob.h>
#undef PGN_DLL_EXPORT

#include <PGN/Common/debug_new.h>

class Blob : public pgn::Blob
{
	char* buf;
	int size;

public:
	Blob(int size)
	{
		buf = debug_new char[size];
		this->size = size;
	}

	virtual void dispose()
	{
		delete[] buf;
	}

	virtual void _free()
	{
		delete this;
	}

	virtual void* getBuffer()
	{
		return buf;
	}

	virtual int getSize()
	{
		return size;
	}
};

pgn::Blob* pgn::Blob::create(int size)
{
	return debug_new ::Blob(size);
}
