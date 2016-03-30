#define PGN_DLL_EXPORT
#include <PGN/FileStream/StdFileStream.h>
#undef PGN_DLL_EXPORT

#include <cassert>
#include <fstream>
#include <PGN/Common/debug_new.h>
#include <PGN/FileStream/FileStream.h>

class StdFileStream : public pgn::FileStream
{
	std::fstream f;
	static const int maxNameLen = 511;
	char fullName[maxNameLen + 1];
	size_t workingDirStrLen;
	char* subNameBaseAddr;
public:
	StdFileStream(const char workingDir[]);
	virtual void open(const char fileName[], OpenMode mode);
	virtual bool isOpen();
	virtual void close();
	virtual void read(void* buf, std::streamsize numBytes);
	virtual void write(void* buf, std::streamsize numBytes);
	virtual void flush();
	virtual long long tellg();
	virtual long long tellp();
	virtual void seekg(long long offset, SeekOrigin origin);
	virtual void seekp(long long offset, SeekOrigin origin);
};

StdFileStream::StdFileStream(const char workingDir[])
{
	workingDirStrLen = strlen(workingDir);
	assert(workingDirStrLen <= maxNameLen);
	strcpy(fullName, workingDir);
	subNameBaseAddr = fullName + workingDirStrLen;
}

void StdFileStream::open(const char fileName[], OpenMode mode)
{
	const char* name;

	if (fileName[0] == '^') // ����·��
	{
		name = fileName + 1;
	}
	else
	{
		assert(workingDirStrLen + strlen(fileName) <= maxNameLen);
		strcpy(subNameBaseAddr, fileName);
		name = fullName;
	}

	std::ios_base::openmode _mode = std::ios_base::binary;

	if(mode & in)
		_mode |= std::ios_base::in;

	if(mode & out)
		_mode |= std::ios_base::out;

	f.open(name, _mode);
}

bool StdFileStream::isOpen()
{
	return f.is_open();
}

void StdFileStream::close()
{
	f.close();
}

void StdFileStream::read(void* buf, std::streamsize numBytes)
{
	f.read((char*)buf, numBytes);
}

void StdFileStream::write(void* buf, std::streamsize numBytes)
{
	f.write((char*)buf, numBytes);
}

void StdFileStream::flush()
{
	f.flush();
}

long long StdFileStream::tellg()
{
	return f.tellg();
}

long long StdFileStream::tellp()
{
	return f.tellp();
}

std::ios_base::seekdir ways[] =
{
	std::ios_base::beg,
	std::ios_base::cur,
	std::ios_base::end
};

void StdFileStream::seekg(long long offset, SeekOrigin origin)
{
	f.seekg(offset, ways[origin]);
}

void StdFileStream::seekp(long long offset, SeekOrigin origin)
{
	f.seekp(offset, ways[origin]);
}

pgn::FileStream* pgn::createStdFileStream(const char workingDir[])
{
	return debug_new ::StdFileStream(workingDir);
}

void pgn::destroyStdFileStream(FileStream* f)
{
	delete f;
}
