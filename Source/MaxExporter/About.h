#include <string>

class About
{
public:
	unsigned int version;
	std::wstring authorName;
	std::wstring copyright;
	std::wstring aboutCaption;
	std::wstring aboutText;

	About();
};

extern About about;
