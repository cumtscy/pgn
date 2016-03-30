#ifdef PGN_DLL_INTERFACE
#undef PGN_DLL_INTERFACE
#endif

#if defined(PGN_STATIC_IMPORT) || defined(PGN_STATIC_BUILD)
#define PGN_DLL_INTERFACE
#elif defined PGN_DLL_EXPORT
#define PGN_DLL_INTERFACE __declspec(dllexport)
#else
#define PGN_DLL_INTERFACE __declspec(dllimport)
#endif
