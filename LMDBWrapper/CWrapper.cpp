#include "header.h"
#include "..\Include\LMDBWrapper.h"
#include "..\Include\CWrapper.h"
#include "..\Include\Constants.h"
#include "..\Include\Helper.h"

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


bool CLMDBWrapper::Init(LPSTR lpszDbName)
{
	char sz[255];

	sprintf_s(sz, "%s\\%s", Constants::LMDBRootPath.c_str(), lpszDbName);
	::CreateDirectoryA(sz, NULL);

	std::wcout << _T("Init LMDB") << std::endl;
	mdb_env_create(&m_lmdb.m_env);
	mdb_env_set_maxreaders(m_lmdb.m_env, 1);
	mdb_env_set_mapsize(m_lmdb.m_env, 1024 * 1024 * 1024);
	mdb_env_open(m_lmdb.m_env, sz, MDB_CREATE/*|MDB_NOSYNC*/, 0664);

	m_lmdb.m_Init = true;
	return true;
}

bool CLMDBWrapper::Uninit(LPSTR lpszDbName)
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

bool CLMDBWrapper::SetData(LPSTR lpszKey, LPSTR lpszValue)
{
	if (m_lmdb.m_Init == false)
	{
		std::wcout << _T("Init not done !") << std::endl;
		return false;
	}

	char szKey[255];
	char szValue[255];

	strcpy(szKey, lpszKey);
	strcpy(szValue, lpszValue);

	MDB_val VKey;
	MDB_val VData;

	VKey.mv_size = sizeof(szKey);
	VKey.mv_data = szKey;
	VData.mv_size = sizeof(szValue);
	VData.mv_data = szValue;
	//_tprintf(_T("Add Key:%s Data:%s\n"), szKey, szValue);
	mdb_txn_begin(m_lmdb.m_env, NULL, 0, &m_lmdb.m_txn);
	mdb_dbi_open(m_lmdb.m_txn, NULL, 0, &m_lmdb.m_dbi);
	int ret = mdb_put(m_lmdb.m_txn, m_lmdb.m_dbi, &VKey, &VData, MDB_NOOVERWRITE);
	mdb_txn_commit(m_lmdb.m_txn);

	return true;
}

bool CLMDBWrapper::SetData(LPSTR lpszKey, LPSTR lpszValueb64, DWORD dwLen)
{
	if (m_lmdb.m_Init == false)
	{
		std::wcout << _T("Init not done !") << std::endl;
		return false;
	}

	char szKey[255];
	strcpy(szKey, lpszKey);

	std::string buffer = CHelper::base64_encode((const unsigned char*)lpszValueb64, dwLen);

	MDB_val VKey;
	MDB_val VData;

	VKey.mv_size = sizeof(szKey);
	VKey.mv_data = szKey;
	VData.mv_size = buffer.length() + 1;
	VData.mv_data = (LPSTR) buffer.c_str();

	mdb_txn_begin(m_lmdb.m_env, NULL, 0, &m_lmdb.m_txn);
	mdb_dbi_open(m_lmdb.m_txn, NULL, 0, &m_lmdb.m_dbi);
	int ret = mdb_put(m_lmdb.m_txn, m_lmdb.m_dbi, &VKey, &VData, MDB_NOOVERWRITE);
	mdb_txn_commit(m_lmdb.m_txn);

	return true;
}

bool CLMDBWrapper::GetData(LPSTR lpszKey, LPSTR * lpszValue)
{
	if (m_lmdb.m_Init == false)
	{
		std::wcout << _T("Init not done !") << std::endl;
		return false;
	}

	char szKey[255];
	//char szValue[255];

	strcpy(szKey, lpszKey);
	//strcpy(szValue, "");

	MDB_val VKey;
	MDB_val VData;

	VKey.mv_size = sizeof(szKey);
	VKey.mv_data = szKey;
	VData.mv_size = 0; // sizeof(szValue);

	mdb_txn_begin(m_lmdb.m_env, NULL, 0, &m_lmdb.m_txn);
	mdb_dbi_open(m_lmdb.m_txn, NULL, 0, &m_lmdb.m_dbi);
	int err = mdb_get(m_lmdb.m_txn, m_lmdb.m_dbi, &VKey, &VData);
	//printf("Get Key:%s Data:%s\n", VKey.mv_data, VData.mv_data);
	mdb_txn_commit(m_lmdb.m_txn);

	*lpszValue = (char*)malloc(VData.mv_size + 1);

	if (err == MDB_NOTFOUND)
	{
		lpszValue = NULL;
	}
	else
	{
		strcpy(*lpszValue, (char *)VData.mv_data);
	}

	return true;
}
