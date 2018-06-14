// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MYSHAREDSTUFF_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MYSHAREDSTUFF_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MYSHAREDSTUFF_EXPORTS
#define MYSHAREDSTUFF_API __declspec(dllexport)
#else
#define MYSHAREDSTUFF_API __declspec(dllimport)
#endif

// This class is exported from the MySharedStuff.dll
class MYSHAREDSTUFF_API CMySharedStuff {
public:
	CMySharedStuff(void);
	// TODO: add your methods here.
};

extern MYSHAREDSTUFF_API int nMySharedStuff;

MYSHAREDSTUFF_API int fnMySharedStuff(void);
