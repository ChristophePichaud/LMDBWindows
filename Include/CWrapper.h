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

	bool Init(LPSTR lpszDatabase);
	bool Uninit();
	bool GetSet();
	bool Set(LPSTR lpszKey, LPSTR lpszValue);
	bool Set(LPSTR lpszKey, LPSTR lpszValue, DWORD dwLen);
	bool Get(LPSTR lpszKey, LPSTR * lpszValue);

private:
	bool m_Init = false;

	MDB_env *_env;
	MDB_dbi _dbi;
	MDB_txn *_txn;
	char szKey[255];
	char szValue[255];
	MDB_val _key, _data;
};

extern LMDBWRAPPER_API int nLMDBWrapper;

LMDBWRAPPER_API int fnLMDBWrapper(void);

