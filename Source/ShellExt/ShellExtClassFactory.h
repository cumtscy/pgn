#include <Unknwn.h>

class ShellExtClassFactory : public IClassFactory
{
	volatile long refCount;

public:
	ShellExtClassFactory();
	~ShellExtClassFactory();

	// IUnknown
	HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

	// IClassFactory
	HRESULT CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject);
	HRESULT LockServer(BOOL fLock);
};
