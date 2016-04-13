#include <mach/machine.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <string>
#include <vector>
#include "MachO.h"

class MachO::Section
{
public:
    std::string name;
    int sizeData;
    void* data;
    int index;
    
    Section(std::string name, int sizeData, void* data)
    : name(name), sizeData(sizeData), data(data)
    {
        
    }
};

class MachO::Segment
{
public:
    std::string name;
    std::vector<Section> sections;
    
    Segment(std::string name)
    : name(name)
    {
        
    }
};

class Symbol
{
public:
    std::string name;
    MachO::Section* section;
    int offset;
    
    Symbol(std::string name, MachO::Section* section, int offset)
    : name(name), section(section), offset(offset)
    {
        
    }
};

class File : public MachO::File
{
    std::vector<MachO::Segment> segments;
    std::vector<Symbol> symbols;
    std::vector<char> machOFileBuf;
public:
    File();
    ~File();
    virtual void destroy();
    virtual MachO::Segment* addSegment(char name[]);
    virtual MachO::Section* addSection(char name[], MachO::Segment* segment, int sizeData, void* data);
    virtual void addSymbol(const char name[], MachO::Section* section, int offset);
    virtual void serialize(char** buf, int* size);
};

MachO::File* MachO::File::create()
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

MachO::Segment* File::addSegment(char name[])
{
    segments.emplace_back(name);
    return &segments.back();
}

MachO::Section* File::addSection(char name[], MachO::Segment* segment, int sizeData, void* data)
{
    segment->sections.emplace_back(name, sizeData, data);
    return &segment->sections.back();
}

void File::addSymbol(const char name[], MachO::Section* section, int offset)
{
    symbols.emplace_back(name, section, offset);
}

#define align(x,a) ((x + ((a)-1)) & ~((a)-1))

void File::serialize(char** buf, int* size)
{
    mach_header_64 machOFileHeader;
    std::vector<segment_command_64> segmentCmds;
    std::vector<section_64> sectionTable;
    symtab_command symbolTableCmd;
    std::vector<nlist_64> symbolTable;
    
    int numSects = 0;
    
    for (auto seg : segments)
        numSects += seg.sections.size();
    
    int sizeCmds = sizeof(segment_command_64) * segments.size() + sizeof(section_64) * numSects + sizeof(symtab_command);
    
    machOFileHeader.magic = MH_MAGIC_64;
    machOFileHeader.cputype = CPU_TYPE_X86_64;
    machOFileHeader.cpusubtype = CPU_SUBTYPE_X86_64_ALL;
    machOFileHeader.filetype = MH_OBJECT;
    machOFileHeader.ncmds = segments.size() + symbols.size();
    machOFileHeader.sizeofcmds = sizeCmds;
    machOFileHeader.flags = MH_SUBSECTIONS_VIA_SYMBOLS;
    
    uint64_t vmaddr = 0;
    uint64_t fileoff = sizeof(mach_header_64) + sizeCmds;
    uint64_t sizeSeg = 0;
    
    for (auto &seg : segments) {
        segment_command_64 segCmd;
        segCmd.cmd = LC_SEGMENT_64;
        segCmd.cmdsize = sizeof(segment_command_64) + sizeof(section_64) * seg.sections.size();
        memcpy(segCmd.segname, seg.name.c_str(), seg.name.length() + 1);
        segCmd.vmaddr = vmaddr;
        segCmd.fileoff = fileoff;
        
        for (auto &sec : seg.sections) {
            section_64 sectionTableEntry;
            memcpy(sectionTableEntry.sectname, sec.name.c_str(), sec.name.length() + 1);
            memcpy(sectionTableEntry.segname, seg.name.c_str(), seg.name.length() + 1);
            sectionTableEntry.addr = vmaddr;
            sectionTableEntry.size = sec.sizeData;
            sectionTableEntry.offset = fileoff;
            sectionTableEntry.align = 4;
            sectionTableEntry.reloff = 0;
            sectionTableEntry.nreloc = 0;
            sectionTableEntry.flags = S_REGULAR;
            sectionTableEntry.reserved1 = 0;
            sectionTableEntry.reserved2 = 0;
            
            sec.index = sectionTable.size();
            sectionTable.push_back(sectionTableEntry);
            vmaddr += sec.sizeData;
            fileoff += sec.sizeData;
            sizeSeg += sec.sizeData;
        }
        
        segCmd.vmsize = sizeSeg;
        segCmd.filesize = sizeSeg;
        segCmd.maxprot = 0x7;
        segCmd.initprot = 0x7;
        segCmd.nsects = seg.sections.size();
        segCmd.flags = 0;
        
        segmentCmds.push_back(segCmd);
    }
    
    int symbolTableOffset = fileoff;
    int stringTableOffset = symbolTableOffset + sizeof(nlist_64) * symbols.size();
    
    std::vector<char> strings;
    
    auto addString = [&strings](std::string str)
    {
        int oldSize = (int)strings.size();
        strings.resize(oldSize + str.length() + 1);
        memcpy(&strings[oldSize], str.c_str(), str.length() + 1);
        
        return oldSize;
    };
    
    strings.push_back('\0');
    
    for (auto sym : symbols) {
        nlist_64 symbolTableEntry;
        symbolTableEntry.n_un.n_strx = addString(sym.name);
        symbolTableEntry.n_type = N_SECT | N_EXT;
        symbolTableEntry.n_sect = sym.section->index + 1;
        symbolTableEntry.n_desc = REFERENCE_FLAG_UNDEFINED_NON_LAZY;
        symbolTableEntry.n_value = sym.offset; // todo: 修改这里，以适应section首地址不为0的情况。
        symbolTable.push_back(symbolTableEntry);
    }
    
    symbolTableCmd.cmd = LC_SYMTAB;
    symbolTableCmd.cmdsize = sizeof(symtab_command);
    symbolTableCmd.symoff = symbolTableOffset;
    symbolTableCmd.nsyms = symbols.size();
    symbolTableCmd.stroff = stringTableOffset;
    symbolTableCmd.strsize = strings.size();
    
    int machOFileSize = stringTableOffset + strings.size();
    machOFileBuf.resize(machOFileSize);
    
    *(mach_header_64*)&machOFileBuf[0] = machOFileHeader;
    
    int offset = sizeof(mach_header_64);
    
    for (int i = 0; i < segments.size(); i++) {
        *(segment_command_64*)&machOFileBuf[offset] = segmentCmds[i];
        offset += sizeof(segment_command_64);
        int size = sizeof(section_64) * segments[i].sections.size();
        memcpy(&machOFileBuf[offset], &sectionTable[segments[i].sections[0].index], size);
        offset += size;
    }
    
    memcpy(&machOFileBuf[offset], &symbolTableCmd, sizeof(symtab_command));
    
    for (auto seg : segments)
        for (auto sec : seg.sections)
            memcpy(&machOFileBuf[sectionTable[sec.index].offset], sec.data, sec.sizeData);
    
    memcpy(&machOFileBuf[symbolTableCmd.symoff], &symbolTable[0], sizeof(nlist_64) * symbols.size());
    memcpy(&machOFileBuf[symbolTableCmd.stroff], &strings[0], strings.size());
    
    *buf = &machOFileBuf[0];
    *size = (int)machOFileBuf.size();
}
