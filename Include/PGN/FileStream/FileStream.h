#pragma once
#include <iostream>
namespace pgn {

class FileStream
{
public:
	typedef unsigned OpenMode;
	static const OpenMode in = 0x01;
	static const OpenMode out = 0x02;

	enum SeekOrigin
	{
		begin,
		current,
		end
	};

	virtual void open(const char fileName[], OpenMode mode) = 0;
	virtual bool isOpen() = 0;
	virtual void close() = 0;
	virtual void read(void* buf, std::streamsize numBytes) = 0;
	virtual void write(void* buf, std::streamsize numBytes) = 0;
	virtual void flush() = 0;
	virtual long long tellg() = 0;
	virtual long long tellp() = 0;
	virtual void seekg(long long offset, SeekOrigin origin) = 0;
	virtual void seekp(long long offset, SeekOrigin origin) = 0;
};

}
