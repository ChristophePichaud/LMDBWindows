#include "header.h"
#include "..\Include\LMDBWrapper.h"
#include "..\Include\CWrapper.h"
#include "..\Include\Constants.h"
#include "..\Include\ServerHelper.h"
#include "..\Include\Base64Helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


LMDBPrivateData::LMDBPrivateData()
{
	m_dbi = 0;
	m_env = NULL;
	m_txn = NULL;
}

LMDBPrivateData::~LMDBPrivateData()
{
}

/*
bool CLMDBWrapper::Init2(LPSTR lpszDbName)
{
	char sz[255];

	sprintf_s(sz, "CreateDirectory %s\\%s\n", Constants::LMDBRootPath.c_str(), lpszDbName);
	printf_s(sz);
	sprintf_s(sz, "%s\\%s", Constants::LMDBRootPath.c_str(), lpszDbName);
	::CreateDirectoryA(sz, NULL);
	
	mdb_env_create(&m_env);
	mdb_env_set_maxreaders(m_env, 1);
	mdb_env_set_mapsize(m_env, 10485760 * 1000);
	int err = mdb_env_open(m_env, sz, MDB_CREATE, 0664);

	printf_s("mdb_env_open=%d - dbname:%s\n", err, sz);

	m_Init = true;
	return true;
}
*/

bool CLMDBWrapper::Uninit()
{
	std::wcout << _T("Uninit LMDB") << std::endl;

	mdb_env_close(env);

	dbi = 0;
	env = NULL;
	txn = NULL;
	m_Init = false;
		
	return true;
}

/*
bool CLMDBWrapper::SetData(LPSTR lpszKey, LPSTR lpszValue)
{
	if (m_Init == false)
	{
		std::wcout << _T("Init not done !") << std::endl;
		return false;
	}

	mdb_txn_begin(m_env, NULL, 0, &m_txn);
	mdb_dbi_open(m_txn, NULL, 0, &m_dbi);

	char szKey[255];
	char szValue[255];
	sprintf(szKey, "key_%d", 10);
	sprintf(szValue, "value_%d", 10);

	MDB_val key, data;
	key.mv_size = sizeof(szKey);
	key.mv_data = szKey;
	data.mv_size = sizeof(szValue);
	data.mv_data = szValue;
	printf("Add Key:%s Data:%s\n", key.mv_data, data.mv_data);
	int err = mdb_put(m_txn, m_dbi, &key, &data, MDB_NOOVERWRITE);

	mdb_txn_commit(m_txn);
	mdb_dbi_close(m_env, m_dbi);

	return true;
}

bool CLMDBWrapper::SetData(LPSTR lpszKey, LPSTR lpszValueb64, DWORD dwLen)
{
	if (m_Init == false)
	{
		std::wcout << _T("Init not done !") << std::endl;
		return false;
	}

	char szKey[255];
	strcpy(szKey, lpszKey);

	//std::string buffer = Base64Helper::base64_encode((const unsigned char*)lpszValueb64, dwLen);
	std::string buffer = lpszValueb64;

	MDB_val VKey;
	MDB_val VData;

	VKey.mv_size = sizeof(szKey);
	VKey.mv_data = szKey;
	VData.mv_size = buffer.length() + 1;
	VData.mv_data = (LPSTR) buffer.c_str();

	mdb_txn_begin(m_env, NULL, 0, &m_txn);
	mdb_dbi_open(m_txn, NULL, 0, &m_dbi);
	int ret = mdb_put(m_txn, m_dbi, &VKey, &VData, MDB_NOOVERWRITE);
	_tprintf(_T("mdb_put=%d\n"), ret);
	mdb_txn_commit(m_txn);
	mdb_dbi_close(m_env, m_dbi);

	return true;
}

bool CLMDBWrapper::GetData(LPSTR lpszKey, LPSTR * lpszValue)
{
	if (m_Init == false)
	{
		std::wcout << _T("Init not done !") << std::endl;
		return false;
	}

	mdb_txn_begin(m_env, NULL, 0, &m_txn);
	mdb_dbi_open(m_txn, NULL, 0, &m_dbi);

	char szKey[255];
	char szValue[255];
	MDB_val key, data;
	
	sprintf(szKey, "key_%d", 10);
	memset(szValue, 0, 255);

	key.mv_size = sizeof(szKey);
	key.mv_data = szKey;
	//data.mv_size = sizeof(szValue);
	//data.mv_data = szValue;

	int err = mdb_get(m_txn, m_dbi, &key, &data);
	printf("mdb_get=%d\n", err);

	printf("Get Key:%s Data:%s\n", key.mv_data, data.mv_data);

	mdb_txn_commit(m_txn);
	mdb_dbi_close(m_env, m_dbi);

	if (err == MDB_NOTFOUND)
	{
		lpszValue = NULL;
		return false;
	}
	else
	{
		*lpszValue = (char*)malloc(data.mv_size + 1);
		strcpy(*lpszValue, (char *)data.mv_data);
		return true;
	}

	return true;
}
*/

bool CLMDBWrapper::Init(LPSTR lpszDatabase)
{
	std::wcout << _T("Init LMDB: ") << lpszDatabase << std::endl;

	char sz[255];
	sprintf_s(sz, "%s\\%s", Constants::LMDBRootPath.c_str(), lpszDatabase);
	::CreateDirectoryA(sz, NULL);

	mdb_env_create(&env);
	mdb_env_set_maxreaders(env, 1);
	mdb_env_set_mapsize(env, 10485760 * 1000);
	mdb_env_open(env, sz, MDB_CREATE, 0);
	return true;
}

bool CLMDBWrapper::Set(LPSTR lpszKey, LPSTR lpszValue)
{
	mdb_txn_begin(env, NULL, 0, &txn);
	int err = mdb_dbi_open(txn, NULL, 0, &dbi);
	std::wcout << _T("mdb_dbi_open: ") << err << std::endl;

	strcpy(szKey, lpszKey);
	strcpy(szValue, lpszValue);
	std::cout << "Set key: " << szKey << std::endl;

	//MDB_val key, data;

	key.mv_size = sizeof(szKey);
	key.mv_data = szKey;
	data.mv_size = sizeof(szValue);
	data.mv_data = szValue;

	err = mdb_put(txn, dbi, &key, &data, 0); // MDB_NOOVERWRITE);
	//printf("Add err=%d Key:%s Data:%s\n", err, key.mv_data, data.mv_data);
	std::wcout << _T("mdb_put: ") << err << std::endl;
	std::cout << "set data: " << (LPSTR)data.mv_data << std::endl;

	mdb_txn_commit(txn);
	mdb_env_sync(env, TRUE);
	mdb_dbi_close(env, dbi);

	return true;
}

bool CLMDBWrapper::Set(LPSTR lpszKey, LPSTR lpszValueb64, DWORD dwLen)
{
	mdb_txn_begin(env, NULL, 0, &txn);
	int err = mdb_dbi_open(txn, NULL, 0, &dbi);
	std::wcout << _T("mdb_dbi_open: ") << err << std::endl;

	strcpy(szKey, lpszKey);
	std::cout << "Set key: " << szKey << std::endl;

	std::string buffer = std::string(lpszValueb64);

	//MDB_val key, data;

	key.mv_size = sizeof(szKey);
	key.mv_data = szKey;
	data.mv_size = buffer.length();
	data.mv_data = (LPSTR)buffer.c_str();

	err = mdb_put(txn, dbi, &key, &data, MDB_NOOVERWRITE);
	//printf("Add err=%d Key:%s Data:%s\n", err, key.mv_data, data.mv_data);
	std::wcout << _T("mdb_put: ") << err << std::endl;
	std::cout << "set data: " << (LPSTR)data.mv_data << std::endl;

	int err2 = mdb_txn_commit(txn);
	std::wcout << _T("mdb_txn_commit: ") << err2 << std::endl;
	mdb_env_sync(env, TRUE);
	mdb_dbi_close(env, dbi);

	return true;
}

bool CLMDBWrapper::Get(LPSTR lpszKey, LPSTR * lpszValue)
{
	mdb_txn_begin(env, NULL, 0, &txn);
	int err = mdb_dbi_open(txn, NULL, 0, &dbi);
	std::wcout << _T("mdb_dbi_open: ") << err << std::endl;

	strcpy(szKey, lpszKey);
	std::cout << "Get key: " << szKey << std::endl;

	//MDB_val key, data;

	key.mv_size = sizeof(szKey);
	key.mv_data = szKey;
	//data.mv_size = sizeof(szValue);
	//data.mv_data = NULL; // szValue;

	err = mdb_get(txn, dbi, &key, &data);
	//printf("Get err=%d Key:%s Data:%s\n", err, key.mv_data, data.mv_data);
	std::wcout << _T("mdb_get: ") << err << std::endl;

	mdb_txn_commit(txn);
	mdb_dbi_close(env, dbi);

	if (err == MDB_NOTFOUND)
	{
		lpszValue = NULL;
		return false;
	}
	else
	{
		*lpszValue = (char*)malloc(data.mv_size + 1);
		strcpy(*lpszValue, (char *)data.mv_data);
		std::cout << "get data: " << *lpszValue << std::endl;
		return true;
	}

	return true;
}

bool CLMDBWrapper::GetSet()
{
	{
		char sz[255];
		sprintf_s(sz, "%s\\%s", Constants::LMDBRootPath.c_str(), "Database");
		::CreateDirectoryA(sz, NULL);

		mdb_env_create(&env);
		mdb_env_set_maxreaders(env, 1);
		mdb_env_set_mapsize(env, 10485760 * 1000);
		mdb_env_open(env, sz, MDB_CREATE/*|MDB_NOSYNC*/, 0664);
	}

	MDB_val key, data;

	{
		//char szKey[255];
		//char szValue[255];
		//MDB_val key, data;

		sprintf(szKey, "key_%d", 10);
		sprintf(szValue, "value_%d", 10);

		key.mv_size = sizeof(szKey);
		key.mv_data = szKey;
		data.mv_size = sizeof(szValue);
		data.mv_data = szValue;

		mdb_txn_begin(env, NULL, 0, &txn);
		mdb_dbi_open(txn, NULL, 0, &dbi);

		int err = mdb_put(txn, dbi, &key, &data, MDB_NOOVERWRITE);
		printf("Add err=%d Key:%s Data:%s\n", err, key.mv_data, data.mv_data);

		mdb_txn_commit(txn);
		mdb_dbi_close(env, dbi);

	}

	{
		//char szKey[255];
		//char szValue[255];
		//MDB_val key, data;

		sprintf(szKey, "key_%d", 10);
		memset(szValue, 0, 255);

		key.mv_size = sizeof(szKey);
		key.mv_data = szKey;
		data.mv_size = 0; // sizeof(szValue);
		data.mv_data = NULL; // szValue;

		mdb_txn_begin(env, NULL, 0, &txn);
		mdb_dbi_open(txn, NULL, 0, &dbi);

		int err = mdb_get(txn, dbi, &key, &data);
		printf("Get err=%d Key:%s Data:%s\n", err, key.mv_data, data.mv_data);

		mdb_txn_commit(txn);
		mdb_dbi_close(env, dbi);
	}

	mdb_env_close(env);
	return true;
}
