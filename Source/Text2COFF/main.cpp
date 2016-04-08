#include <fstream>
#include <regex>
#include "COFF.h"

void main(int argc, char *argv[])
{
	std::string src;
	std::string dst;
	std::string symbolName;

	src = argv[1];

	for (int i = 2; i < argc; i++)
	{
		std::string arg = argv[i];

		if (arg == "-s")
		{
			symbolName = argv[++i];
			continue;
		}

		dst = arg;
	}

	if (symbolName.empty())
	{
		std::regex rgx("(.*)\\..*");
		symbolName = std::regex_replace(src, rgx, "$1");
	}

	if (dst.empty())
	{
		std::regex rgx("(.*)\\..*");
		dst = std::regex_replace(src, rgx, "$1.obj");
	}

	std::ifstream is;
	is.open(src, std::ios_base::binary);
	is.seekg(0, std::ios_base::end);
	size_t size = is.tellg();
	is.seekg(0, std::ios_base::beg);
	char* buf = new char[size + 1];
	is.read(buf, size);
	is.close();
	buf[size] = '\0';

	COFF::File* coffFile = COFF::File::create();
	int section = coffFile->addSection(".data", (int)size + 1, buf);
	coffFile->addSymbol(symbolName.c_str(), section, 0);

	char* coffFileBuf;
	int coffFileSize;
	coffFile->serialize(&coffFileBuf, &coffFileSize);

	std::ofstream os;
	os.open(dst, std::ios_base::binary);
	os.write(coffFileBuf, coffFileSize);
	os.close();

	coffFile->destroy();
	delete[] buf;
}
