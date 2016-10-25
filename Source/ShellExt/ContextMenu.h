#include <Shlobj.h>
#include <vector>

class ContextMenu
{
public:
	typedef void (ContextMenu::* Cmd)();

	struct MenuItem
	{
		std::string menuItemText;
		Cmd cmd;
	};

	std::vector<MenuItem> menuItemList;

	std::string binDir;
	std::string workingDir;
	std::vector<std::string> selectedTextureFiles;

	ContextMenu(PCIDLIST_ABSOLUTE folder, IDataObject *dataObj);
	void makeTextureMapPackageForMesh();
};
