#include <cassert>
#include <ctime>
#include <string>
#include <vector>
#include "COFF.h"

class Section
{
public:
	std::string name;
	int sizeData;
	void* data;

	Section(std::string name, int sizeData, void* data)
		: name(name), sizeData(sizeData), data(data)
	{

	}
};

class Symbol
{
public:
	std::string name;
	int section;
	int offset;

	Symbol(std::string name, int section, int offset)
		: name(name), section(section), offset(offset)
	{

	}
};

class File : public COFF::File
{
	std::vector<Section> sections;
	std::vector<Symbol> symbols;
	std::vector<char> coffFileBuf;
public:
	File();
	~File();
	virtual void destroy();
	virtual int addSection(char name[], int sizeData, void* data);
	virtual void addSymbol(const char name[], int section, int offset);
	virtual void serialize(char** buf, int* size);
};

COFF::File* COFF::File::create()
{
	return new ::File();
}

void File::destroy()
{
	delete this;
}

File::File()
{
}

File::~File()
{
}

int File::addSection(char name[], int sizeData, void* data)
{
	sections.emplace_back(name, sizeData, data);
	return (int)sections.size();
}

void File::addSymbol(const char name[], int section, int offset)
{
	symbols.emplace_back(name, section, offset);
}

struct COFFFileHeader
{
	short machine;
	short numberOfSections;
	int timeDateStamp;
	int pointerToSymbolTable;
	int numberOfSymbols;
	short sizeOfOptionalHeader;
	short characteristics;
};

// machine
#define IMAGE_FILE_MACHINE_UNKNOWN 0x0

struct SectionTableEntry
{
	char name[8];
	int virtualSize;
	int virtualAddress;
	int sizeOfRawData;
	int pointerToRawData;
	int pointerToRelocations;
	int pointerToLinenumbers;
	short numberOfRelocations;
	short numberOfLinenumbers;
	int characteristics;
};

// characteristics
#define IMAGE_SCN_CNT_INITIALIZED_DATA 0x00000040
#define IMAGE_SCN_ALIGN_8BYTES 0x00400000
#define IMAGE_SCN_MEM_READ 0x40000000
#define IMAGE_SCN_MEM_WRITE 0x80000000

#pragma pack(push)
#pragma pack(2)
struct SymbolTableRecord
{
	union {
		char name[8];
		int zeroes_offset[2];
	};
	int value;
	short sectionNumber;
	short type;
	char storageClass;
	char numberOfAuxSymbols;
};
#pragma pack(pop)

// storageClass
#define IMAGE_SYM_CLASS_EXTERNAL 2

struct StringTable
{
	int sizeStringTable;
	char data[1];
};

#define align(x,a) ((x + ((a)-1)) & ~((a)-1))

void File::serialize(char** buf, int* size)
{
	COFFFileHeader coffFileHeader;
	std::vector<SectionTableEntry> sectionTable(sections.size());
	std::vector<SymbolTableRecord> symbolTable(symbols.size());
	StringTable stringTable;

	coffFileHeader.machine = IMAGE_FILE_MACHINE_UNKNOWN;
	coffFileHeader.numberOfSections = (short)sections.size();
	coffFileHeader.timeDateStamp = (int)time(0);
	coffFileHeader.numberOfSymbols = (int)symbols.size();
	coffFileHeader.sizeOfOptionalHeader = 0;
	coffFileHeader.characteristics = 0;

	int offset = sizeof(COFFFileHeader) + sizeof(SectionTableEntry) * (int)sections.size();

	for (size_t i = 0; i < sections.size(); i++)
	{
		offset = align(offset, 8);

		Section* section = &sections[i];
		SectionTableEntry* entry = &sectionTable[i];

		assert(section->name.length() < 8);
		memcpy(entry->name, section->name.c_str(), section->name.length() + 1);

		entry->virtualSize = 0;
		entry->virtualAddress = 0;
		entry->sizeOfRawData = section->sizeData;
		entry->pointerToRawData = offset;
		entry->pointerToRelocations = 0;
		entry->pointerToLinenumbers = 0;
		entry->numberOfRelocations = 0;
		entry->numberOfLinenumbers = 0;
		entry->characteristics = IMAGE_SCN_CNT_INITIALIZED_DATA
								| IMAGE_SCN_ALIGN_8BYTES
								| IMAGE_SCN_MEM_READ
								| IMAGE_SCN_MEM_WRITE
								;

		offset += section->sizeData;
	}

	coffFileHeader.pointerToSymbolTable = offset;

	std::vector<char> strings;

	auto addString = [&strings](std::string str)
	{
		int oldSize = (int)strings.size();
		strings.resize(oldSize + str.length() + 1);
		memcpy(&strings[oldSize], str.c_str(), str.length() + 1);

		return offsetof(StringTable, data) + oldSize;
	};

	for (size_t i = 0; i < symbols.size(); i++)
	{
		Symbol* symbol = &symbols[i];
		SymbolTableRecord* record = &symbolTable[i];

		if (symbol->name.length() < 8)
		{
			memcpy(record->name, symbol->name.c_str(), symbol->name.length() + 1);
		}
		else
		{
			record->zeroes_offset[0] = 0;
			record->zeroes_offset[1] = (int)addString(symbol->name);
		}

		record->value = symbol->offset;
		record->sectionNumber = symbol->section;
		record->type = 0x0;
		record->storageClass = IMAGE_SYM_CLASS_EXTERNAL;
		record->numberOfAuxSymbols = 0;
	}

	offset += sizeof(SymbolTableRecord) * (int)symbols.size();

	int posStringTable = offset;
	stringTable.sizeStringTable = offsetof(StringTable, data) + (int)strings.size();

	int coffFileSize = posStringTable + stringTable.sizeStringTable;
	coffFileBuf.resize(coffFileSize);

	*(COFFFileHeader*)&coffFileBuf[0] = coffFileHeader;
	memcpy(&coffFileBuf[sizeof(COFFFileHeader)], &sectionTable[0], sizeof(SectionTableEntry) * sections.size());

	for (size_t i = 0; i < sections.size(); i++)
		memcpy(&coffFileBuf[sectionTable[i].pointerToRawData], sections[i].data, sections[i].sizeData);

	memcpy(&coffFileBuf[coffFileHeader.pointerToSymbolTable], &symbolTable[0], sizeof(SymbolTableRecord) * symbols.size());

	StringTable* p = (StringTable*)&coffFileBuf[posStringTable];
	memcpy(p, &stringTable, offsetof(StringTable, data));

	if (strings.size())
		memcpy(p->data, &strings[0], strings.size());

	*buf = &coffFileBuf[0];
	*size = (int)coffFileBuf.size();
}
