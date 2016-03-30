
namespace COFF{

class File
{
public:
	static File* create();
	virtual void destroy() = 0;
	virtual int addSection(char name[], int sizeData, void* data) = 0;
	virtual void addSymbol(const char name[], int section, int offset) = 0;
	virtual void serialize(char** buf, int* size) = 0;
};

}
