#pragma once
#include "LMDBWindowsDll.h" 
#include "lmdb-dll.h"
#include "midl-dll.h"

class LMDBWRAPPER_API LMDBData
{
public:
	LMDBData();
	virtual ~LMDBData();

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

	bool Init(std::wstring dbname);
	bool Uninit(std::wstring dbname);
	bool SetData(std::wstring key, std::wstring value);
	bool SetData(std::wstring key, std::wstring valueb64, DWORD dwLen);
	bool GetData(std::wstring key, std::wstring & value);

private:
	LMDBData m_lmdb;
};

extern LMDBWRAPPER_API int nLMDBWrapper;

LMDBWRAPPER_API int fnLMDBWrapper(void);

