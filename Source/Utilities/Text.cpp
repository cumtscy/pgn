#define PGN_DLL_EXPORT
#include <PGN/Utilities/Text.h>
#undef PGN_DLL_EXPORT

#include <PGN/Common/debug_new.h>
#include <regex>

class Text : public pgn::Text
{
	int sizeBuf;
	char *text, *temp;
public:
	Text(int sizeBuf)
	{
		this->sizeBuf = sizeBuf;
		text = debug_new char[sizeBuf];
		temp = debug_new char[sizeBuf];
	}

	virtual void dispose()
	{
		delete text;
		delete temp;
	}

	virtual void _free()
	{
		delete this;
	}

	virtual void setText(char text[])
	{
		strcpy(this->text, text);
	}

	virtual void setText(const char text[])
	{
		strcpy(this->text, text);
	}

	virtual char* getText()
	{
		return text;
	}

	virtual void replace(char pattern[], char replacement[], bool replaceAll)
	{
		std::regex rgx(pattern);
		std::regex_replace(temp, text, text + strlen(text) + 1, rgx, replacement, replaceAll ? std::regex_constants::match_any : std::regex_constants::format_first_only);
		std::swap(text, temp);
	}

	virtual bool extract(char pattern[], char* s, int* i)
	{
		std::regex rgx(pattern);
		std::cmatch match;
		if (std::regex_search(text, match, rgx))
		{
			std::string str;

			str = match[1];
			strcpy(s, str.c_str());

			str = match[2];
			*i = std::stoi(str);

			return true;
		}
		return false;
	}
};

pgn::Text* pgn::Text::create(int sizeBuf)
{
	return debug_new ::Text(sizeBuf);
}
