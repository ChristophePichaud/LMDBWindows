#include "header.h"
#include "..\Include\LMDBWrapper.h"
#include "..\Include\CWrapper.h"
#include "..\Include\LMDBWrapper.h"
#include "..\Include\MyServer\Constants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


LMDBData::LMDBData()
{
	m_dbi = 0;
	m_env = nullptr;
	m_txn = nullptr;
}

LMDBData::~LMDBData()
{
}


bool CLMDBWrapper::Init(std::wstring dbname)
{
	char sz[255];

	std::string dbname2(dbname.begin(), dbname.end());

	sprintf_s(sz, "%s\\%s", Constants::LMDBRootPath.c_str(), dbname2.c_str());
	::CreateDirectoryA(sz, NULL);

	std::wcout << _T("Init LMDB") << std::endl;
	mdb_env_create(&m_lmdb.m_env);
	mdb_env_set_maxreaders(m_lmdb.m_env, 1);
	mdb_env_set_mapsize(m_lmdb.m_env, 10485760);
	mdb_env_open(m_lmdb.m_env, sz, MDB_CREATE/*|MDB_NOSYNC*/, 0664);
	m_lmdb.m_Init = true;
	return true;
}

bool CLMDBWrapper::Uninit(std::wstring dbname)
{
	std::wcout << _T("Uninit LMDB") << std::endl;

	mdb_dbi_close(m_lmdb.m_env, m_lmdb.m_dbi);
	mdb_env_close(m_lmdb.m_env);

	m_lmdb.m_dbi = 0;
	m_lmdb.m_env = nullptr;
	m_lmdb.m_txn = nullptr;
	m_lmdb.m_Init = false;

	return true;
}

bool CLMDBWrapper::SetData(std::wstring key, std::wstring value)
{
	if (m_lmdb.m_Init == false)
	{
		std::wcout << _T("Init not done !") << std::endl;
		return false;
	}

	TCHAR szKey[255];
	TCHAR szValue[255];

	MDB_val VKey;
	MDB_val VData;

	_tcscpy_s(szKey, key.c_str());
	_tcscpy_s(szValue, value.c_str());

	VKey.mv_size = sizeof(szKey);
	VKey.mv_data = szKey;
	VData.mv_size = sizeof(szValue);
	VData.mv_data = szValue;
	//_tprintf(_T("Add Key:%s Data:%s\n"), szKey, szValue);
	mdb_txn_begin(m_lmdb.m_env, NULL, 0, &m_lmdb.m_txn);
	mdb_dbi_open(m_lmdb.m_txn, NULL, 0, &m_lmdb.m_dbi);
	mdb_put(m_lmdb.m_txn, m_lmdb.m_dbi, &VKey, &VData, MDB_NOOVERWRITE);
	mdb_txn_commit(m_lmdb.m_txn);
	//mdb_env_stat(m_lmdb.m_env, &m_lmdb.m_mst);

	return true;
}

bool CLMDBWrapper::SetData(std::wstring key, std::wstring valueb64, DWORD dwLen)
{
	return true;
}

bool CLMDBWrapper::GetData(std::wstring key, std::wstring & value)
{
	if (m_lmdb.m_Init == false)
	{
		std::wcout << _T("Init not done !") << std::endl;
		return false;;
	}

	TCHAR szKey[255];
	TCHAR szValue[255];

	MDB_val VKey;
	MDB_val VData;

	_tcscpy_s(szKey, key.c_str());

	VKey.mv_size = sizeof(szKey);
	VKey.mv_data = szKey;
	VData.mv_size = sizeof(szValue);
	VData.mv_data = szValue;

	mdb_txn_begin(m_lmdb.m_env, NULL, 0, &m_lmdb.m_txn);
	mdb_dbi_open(m_lmdb.m_txn, NULL, 0, &m_lmdb.m_dbi);
	int err = mdb_get(m_lmdb.m_txn, m_lmdb.m_dbi, &VKey, &VData);
	mdb_txn_commit(m_lmdb.m_txn);
	//mdb_env_stat(m_lmdb.m_env, &m_lmdb.m_mst);

	if (err == MDB_NOTFOUND)
	{
		value = _T("");
	}
	else
	{
		value = (TCHAR *)VData.mv_data;
	}

	return true;
}
