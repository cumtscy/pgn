#include <Windows.h>
#include "ShellExtClassFactory.h"

volatile long dllRefCount;

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
		dllRefCount = 0;

	return TRUE;
}

extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv)
{
	ShellExtClassFactory* factory = new ShellExtClassFactory;
	HRESULT hr = factory->QueryInterface(riid, ppv);

	if (FAILED(hr))
		delete factory;

	return hr;
}

extern "C" HRESULT __stdcall DllCanUnloadNow()
{
	return dllRefCount == 0 ? S_OK : S_FALSE;
}
