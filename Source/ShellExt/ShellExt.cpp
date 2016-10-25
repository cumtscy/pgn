#include <cassert>
#include <Shlobj.h>
#include "DllEntry.h"
#include "ShellExt.h"

ShellExt::ShellExt()
{
	refCount = 0;
	InterlockedIncrement(&dllRefCount);

	ctxMenu = 0;
}

ShellExt::~ShellExt()
{
	if (ctxMenu) delete ctxMenu;

	InterlockedDecrement(&dllRefCount);
}

HRESULT ShellExt::QueryInterface(REFIID riid, void **ppvObject)
{
	if (IsEqualIID(riid, IID_IUnknown))
		*ppvObject = (IShellExtInit*)this;
	else if (IsEqualIID(riid, IID_IShellExtInit))
		*ppvObject = (IShellExtInit*)this;
	else if (IsEqualIID(riid, IID_IContextMenu))
		*ppvObject = (IContextMenu*)this;
	else
		return E_NOINTERFACE;

	AddRef();
	return S_OK;
}

ULONG ShellExt::AddRef()
{
	return InterlockedIncrement(&refCount);
}

ULONG ShellExt::Release()
{
	long n = InterlockedDecrement(&refCount);

	if (n == 0)
		delete this;

	return n;
}

HRESULT ShellExt::Initialize(PCIDLIST_ABSOLUTE pidlFolder, IDataObject *pdtobj, HKEY hkeyProgID)
{
	if (ctxMenu) delete ctxMenu;
	ctxMenu = new ContextMenu(pidlFolder, pdtobj);

	return S_OK;
}

HRESULT ShellExt::GetCommandString(UINT_PTR idCmd, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax)
{
	pszName[0] = '\0';
	return S_OK;
}

HRESULT ShellExt::InvokeCommand(LPCMINVOKECOMMANDINFO pici)
{
	assert(!HIWORD(pici->lpVerb));

	unsigned idCmd = LOWORD(pici->lpVerb);
	ContextMenu::Cmd cmd = ctxMenu->menuItemList[idCmd].cmd;
	(ctxMenu->*cmd)();

	return S_OK;
}

HRESULT ShellExt::QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
	if (uFlags & CMF_DEFAULTONLY)
		return MAKE_HRESULT(SEVERITY_SUCCESS, 0, 0);

	unsigned idCmd = idCmdFirst;

	InsertMenu(hmenu, indexMenu++, MF_SEPARATOR | MF_BYPOSITION, 0, 0);

	for (int i = 0; i < ctxMenu->menuItemList.size(); i++)
	{
		ContextMenu::MenuItem& menuItem = ctxMenu->menuItemList[i];

		MENUITEMINFO menuItemInfo = { 0 };
		menuItemInfo.cbSize = sizeof(menuItemInfo);
		menuItemInfo.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STRING;
		menuItemInfo.fType = MFT_STRING;
		menuItemInfo.dwTypeData = (char*)menuItem.menuItemText.c_str();
		menuItemInfo.wID = idCmd + i;
		InsertMenuItem(hmenu, indexMenu++, TRUE, &menuItemInfo);
	}

	InsertMenu(hmenu, indexMenu++, MF_SEPARATOR | MF_BYPOSITION, 0, 0);

	return MAKE_HRESULT(SEVERITY_SUCCESS, 0, ctxMenu->menuItemList.size());
}
