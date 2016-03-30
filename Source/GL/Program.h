#include <map>
#include <PGN/RenderingSystem/Program.h>
#include <string>
#include <vector>

namespace pgn {
	class Blob;
}

class Shader;

class Program : public pgn::Program
{
public:
	unsigned program;
	std::map<std::string, int> uniformBlockBindingPointMap;
	std::map<std::string, int> attribLocationMap;
	class UniformVariable
	{
	public:
		std::string		name;
		int				location;
	};
	std::vector<UniformVariable> samplerVariables;

	void init_create();
	void init_link(Shader* vs, Shader* ps);
	bool init_link(pgn::Blob* programBlob);
	void init_doTheRest();

	void init(Shader* vs, Shader* ps);
	bool init(pgn::Blob* programBlob);
	pgn::Blob* serialize();
	virtual void dispose();
	virtual void _free();
	virtual int getNumTextureVariables();
	virtual const char* getTextureVariableName(int i);
};
