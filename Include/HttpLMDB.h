// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HTTPLMDB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HTTPLMDB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef HTTPLMDB_EXPORTS
#define HTTPLMDB_API __declspec(dllexport)
#else
#define HTTPLMDB_API __declspec(dllimport)
#endif

// This class is exported from the HttpLMDB.dll
class HTTPLMDB_API CHttpLMDB {
public:
	CHttpLMDB(void);
	// TODO: add your methods here.
};

extern HTTPLMDB_API int nHttpLMDB;

HTTPLMDB_API int fnHttpLMDB(void);

bool HTTPLMDB_API Init(std::wstring dbname);
bool HTTPLMDB_API Uninit(std::wstring dbname);
bool HTTPLMDB_API SetData(std::wstring key, std::wstring value, std::wstring name);
bool HTTPLMDB_API SetDataB64(std::wstring key, std::wstring valueb64, DWORD dwLen, std::wstring name);
bool HTTPLMDB_API GetData(std::wstring key, std::wstring & value, std::wstring name);
bool HTTPLMDB_API GetDataB64(std::wstring key, std::wstring & value, std::wstring name);
