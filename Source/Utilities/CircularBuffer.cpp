#define PGN_DLL_EXPORT
#include <PGN/Utilities/CircularBuffer.h>
#undef PGN_DLL_EXPORT

#include <PGN/Common/debug_new.h>
#include <PGN/Platform/Thread/Signal.h>

class CircularBuffer : public pgn::CircularBuffer
{
	static const int alignment = 16;
	char *_buf, *buf, *bufEnd;
	size_t bufSize;
	volatile char* start;
	char* end;
	pgn::Signal* spaceBecomesAvailable;

public:

	CircularBuffer(size_t bufSize)
	{
		_buf = debug_new char[bufSize + alignment - 1];
		buf = (char*)(((size_t)(_buf + alignment - 1)) & ~(alignment - 1));
		bufEnd = buf + bufSize;
		this->bufSize = bufSize;
		start = buf;
		end = buf;
		spaceBecomesAvailable = pgn::Signal::create();
	}

	virtual void dispose()
	{
		delete[] _buf;
		spaceBecomesAvailable->destroy();
	}

	virtual void _free()
	{
		delete this;
	}

	virtual void* alloc(size_t size)
	{
		end = (char*)(((size_t)(end + alignment - 1)) & ~(alignment - 1));

		if(end >= start)
		{
			char* _end = end;
			end += size;
			if(end <= bufEnd)
			{
				return _end;
			}
			end = buf;
		}

		while((size_t)(start - end) <= size) // Áô1µ½size×Ö½ÚµÄ¿Õ
			spaceBecomesAvailable->wait();

		char* _end = end;
		end += size;
		return _end;
	}

	virtual void _free(size_t size)
	{
		char* _start = (char*) start + size;

		if(_start > bufEnd)
			start = buf + size;
		else
			start = _start;

		spaceBecomesAvailable->raise();
	}
};

pgn::CircularBuffer* pgn::CircularBuffer::create(size_t bufSize)
{
	return debug_new ::CircularBuffer(bufSize);
}
