#include <fstream>
#include <PGN/Assets/PNT.h>
#include <regex>
#include <string>

#define GL_COMPRESSED_RGB8_ETC2           0x9274
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9276
#define GL_COMPRESSED_RGBA8_ETC2_EAC      0x9278

struct KTXHeader
{
	char identifier[12];
	unsigned endianness;
	unsigned glType;
	unsigned glTypeSize;
	unsigned glFormat;
	unsigned glInternalFormat;
	unsigned glBaseInternalFormat;
	unsigned pixelWidth;
	unsigned pixelHeight;
	unsigned pixelDepth;
	unsigned numberOfArrayElements;
	unsigned numberOfFaces;
	unsigned numberOfMipmapLevels;
	unsigned bytesOfKeyValueData;
	char data[1];
};

struct KTXLevel
{
	unsigned imageSize;
	char data[1];
};

void main(int argc, char* argv[])
{
	std::string src;
	std::string dst;

	src = argv[1];

	for (int i = 2; i < argc; i++)
	{
		std::string arg = argv[i];

		dst = arg;
	}

	if (dst.empty())
	{
		std::regex rgx("(.*)\\.[Kk][Tt][Xx]");
		dst = std::regex_replace(src, rgx, "$1.PNT");
	}

	std::ifstream is;
	is.open(src, std::ios_base::binary);
	is.seekg(0, std::ios_base::end);
	size_t size = is.tellg();
	is.seekg(0, std::ios_base::beg);
	char* buf = new char[size];
	is.read(buf, size);
	is.close();

	KTXHeader* ktxHeader = (KTXHeader*)buf;

	pgn::PNTHeader pntHeader;

	switch (ktxHeader->glInternalFormat)
	{
	case GL_COMPRESSED_RGB8_ETC2:
		pntHeader.format = pgn::PNTHeader::RGB8_ETC2;
		break;
	case GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
		pntHeader.format = pgn::PNTHeader::RGB8_A1_ETC2;
		break;
	case GL_COMPRESSED_RGBA8_ETC2_EAC:
		pntHeader.format = pgn::PNTHeader::RGBA8_ETC2_EAC;
		break;
	}

	pntHeader.width = ktxHeader->pixelWidth;
	pntHeader.height = ktxHeader->pixelHeight;
	pntHeader.numLevels = ktxHeader->numberOfMipmapLevels;

	std::ofstream os;
	os.open(dst, std::ios_base::binary);
	os.seekp(sizeof(pgn::PNTHeader), std::ios_base::beg);

	KTXLevel* level = (KTXLevel*)(ktxHeader->data + ktxHeader->bytesOfKeyValueData);
	int offset = sizeof(pgn::PNTHeader);

	for (unsigned i = 0; i < ktxHeader->numberOfMipmapLevels; i++)
	{
		pntHeader.levelOffsets[i] = offset;
		os.write(level->data, level->imageSize);
		offset += level->imageSize;
		int sizeMipPadding = 3 - (level->imageSize + 3) % 4;
		level = (KTXLevel*)(level->data + level->imageSize + sizeMipPadding);
	}

	os.seekp(0, std::ios_base::beg);
	os.write((char*)&pntHeader, sizeof(pntHeader));
	os.close();
}
