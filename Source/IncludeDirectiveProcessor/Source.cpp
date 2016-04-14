#include <fstream>
#include <regex>

int main(int argc, char *argv[])
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
		std::regex rgx("(.*)\\..*");
		dst = std::regex_replace(src, rgx, "$1.expanded");
	}

	size_t found = src.find_last_of("/\\");
	if (found != std::string::npos) found++;
	std::string srcFileDir = src.substr(0, found);

	std::ifstream is;
	is.open(src, std::ios_base::binary);
	is.seekg(0, std::ios_base::end);
	size_t size = is.tellg();
	is.seekg(0, std::ios_base::beg);
	char* buf = new char[size];
	is.read(buf, size);
	is.close();

	std::string s(buf, size);
	std::smatch m;
	std::regex rgx("#include \"(.*)\"");

	while (std::regex_search(s, m, rgx))
	{
		std::string includeFilePath = srcFileDir + (std::string)m[1];

		std::ifstream is;
		is.open(includeFilePath, std::ios_base::binary);
		is.seekg(0, std::ios_base::end);
		size_t size = is.tellg();
		is.seekg(0, std::ios_base::beg);
		char* buf = new char[size];
		is.read(buf, size);
		is.close();

		std::string includeFile(buf, size);
		s = std::regex_replace(s, rgx, includeFile, std::regex_constants::format_first_only);

		delete[] buf;
	}

	delete[] buf;

	std::ofstream os;
	os.open(dst, std::ios_base::binary);
	os.write(s.c_str(), s.length());
	os.close();
    
    return 0;
}
