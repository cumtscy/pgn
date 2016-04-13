
namespace MachO{
    
    class Segment;
    class Section;
    
    class File
    {
    public:
        static File* create();
        virtual void destroy() = 0;
        virtual Segment* addSegment(char name[]) = 0;
        virtual Section* addSection(char name[], Segment* segment, int sizeData, void* data) = 0;
        virtual void addSymbol(const char name[], Section* section, int offset) = 0;
        virtual void serialize(char** buf, int* size) = 0;
    };
    
}
