#pragma once
#include "LMDBWindowsDll.h" 
#include "lmdb-dll.h"
#include "midl-dll.h"

class LMDBWRAPPER_API LMDBPrivateData
{
public:
	LMDBPrivateData();
	virtual ~LMDBPrivateData();

public:
	bool m_Init = false;
	MDB_env * m_env;
	MDB_dbi m_dbi;
	MDB_txn * m_txn;
};


// This class is exported from the LMDBWrapper.dll
class LMDBWRAPPER_API CLMDBWrapper {	
public:
	CLMDBWrapper(void) { }
	~CLMDBWrapper() { }

	//bool Init2(LPSTR lpszDbName);
	bool Init(LPSTR lpszDatabase);
	bool Uninit();
	//bool SetData(LPSTR lpszKey, LPSTR lpszValue);
	//bool SetData(LPSTR lpszKey, LPSTR lpszValue, DWORD dwLen);
	//bool GetData(LPSTR lpszKey, LPSTR * lpszValue);
	bool GetSet();
	bool Set(LPSTR lpszKey, LPSTR lpszValue);
	bool Set(LPSTR lpszKey, LPSTR lpszValue, DWORD dwLen);
	bool Get(LPSTR lpszKey, LPSTR * lpszValue);

private:
	bool m_Init = false;

	MDB_env *env;
	MDB_dbi dbi;
	MDB_txn *txn;

	char szKey[255];
	char szValue[255];
	MDB_val key, data;
};

extern LMDBWRAPPER_API int nLMDBWrapper;

LMDBWRAPPER_API int fnLMDBWrapper(void);

