#include <Shobjidl.h>
#include "ContextMenu.h"

class ShellExt : public IShellExtInit, IContextMenu
{
	volatile long refCount;
	ContextMenu* ctxMenu;

public:
	ShellExt();
	~ShellExt();

	// IUnknown
	HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

	// IShellExtInit
	HRESULT Initialize(PCIDLIST_ABSOLUTE pidlFolder, IDataObject *pdtobj, HKEY hkeyProgID);

	// IContextMenu
	HRESULT GetCommandString(UINT_PTR idCmd, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax);
	HRESULT InvokeCommand(LPCMINVOKECOMMANDINFO pici);
	HRESULT QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
};
