#include <Windows.h>
#include "DllEntry.h"
#include "ShellExt.h"
#include "ShellExtClassFactory.h"

ShellExtClassFactory::ShellExtClassFactory()
{
	refCount = 0;
	InterlockedIncrement(&dllRefCount);
}

ShellExtClassFactory::~ShellExtClassFactory()
{
	InterlockedDecrement(&dllRefCount);
}

HRESULT ShellExtClassFactory::QueryInterface(REFIID riid, void **ppvObject)
{
	if (IsEqualIID(riid, IID_IUnknown))
		*ppvObject = (IClassFactory*)this;
	else if (IsEqualIID(riid, IID_IClassFactory))
		*ppvObject = (IClassFactory*)this;
	else
		return E_NOINTERFACE;

	AddRef();
	return S_OK;
}

ULONG ShellExtClassFactory::AddRef()
{
	return InterlockedIncrement(&refCount);
}

ULONG ShellExtClassFactory::Release()
{
	long n = InterlockedDecrement(&refCount);

	if (n == 0)
		delete this;

	return n;
}

HRESULT ShellExtClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject)
{
	ShellExt* shellExt = new ShellExt;
	HRESULT hr = shellExt->QueryInterface(riid, ppvObject);

	if (FAILED(hr))
		delete shellExt;

	return hr;
}

HRESULT ShellExtClassFactory::LockServer(BOOL fLock)
{
	return E_NOTIMPL;
}
