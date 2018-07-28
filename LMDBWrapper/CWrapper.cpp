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

bool CLMDBWrapper::Uninit()
{
	std::wcout << _T("Uninit LMDB") << std::endl;

	mdb_env_close(_env);

	_dbi = 0;
	_env = NULL;
	_txn = NULL;
	m_Init = false;
		
	return true;
}

bool CLMDBWrapper::Init(LPSTR lpszDatabase)
{
	std::wcout << _T("Init LMDB: ") << lpszDatabase << std::endl;

	char sz[255];
	sprintf_s(sz, "%s\\%s", Constants::LMDBRootPath.c_str(), lpszDatabase);
	::CreateDirectoryA(sz, NULL);

	mdb_env_create(&_env);
	mdb_env_set_maxreaders(_env, 1);
	mdb_env_set_mapsize(_env, 10485760 * 1000);
	mdb_env_open(_env, sz, MDB_CREATE, 0);
	return true;
}

bool CLMDBWrapper::Init(std::wstring database)
{
	std::string db(database.begin(), database.end());
	return Init((LPSTR)db.c_str());
}


bool CLMDBWrapper::Set(LPSTR lpszKey, LPSTR lpszValue)
{
	mdb_txn_begin(_env, NULL, 0, &_txn);
	int err = mdb_dbi_open(_txn, NULL, 0, &_dbi);
	std::wcout << _T("mdb_dbi_open: ") << err << std::endl;

	strcpy(_szKey, lpszKey);
	strcpy(_szValue, lpszValue);
	std::cout << "Set key: " << _szKey << std::endl;

	_key.mv_size = sizeof(_szKey);
	_key.mv_data = _szKey;
	_data.mv_size = sizeof(_szValue);
	_data.mv_data = _szValue;

	err = mdb_put(_txn, _dbi, &_key, &_data, 0); // MDB_NOOVERWRITE);
	//printf("Add err=%d Key:%s Data:%s\n", err, key.mv_data, data.mv_data);
	std::wcout << _T("mdb_put: ") << err << std::endl;
	std::cout << "set data: " << (LPSTR)_data.mv_data << std::endl;

	mdb_txn_commit(_txn);
	mdb_env_sync(_env, TRUE);
	mdb_dbi_close(_env, _dbi);

	return true;
}

bool CLMDBWrapper::Set(LPSTR lpszKey, LPSTR lpszValueb64, DWORD dwLen)
{
	mdb_txn_begin(_env, NULL, 0, &_txn);
	int err = mdb_dbi_open(_txn, NULL, 0, &_dbi);
	std::wcout << _T("mdb_dbi_open: ") << err << std::endl;

	strcpy(_szKey, lpszKey);
	std::cout << "Set key: " << _szKey << std::endl;

	std::string buffer = std::string(lpszValueb64);

	_key.mv_size = sizeof(_szKey);
	_key.mv_data = _szKey;
	_data.mv_size = buffer.length();
	_data.mv_data = (LPSTR)buffer.c_str();

	err = mdb_put(_txn, _dbi, &_key, &_data, MDB_NOOVERWRITE);
	//printf("Add err=%d Key:%s Data:%s\n", err, key.mv_data, data.mv_data);
	std::wcout << _T("mdb_put: ") << err << std::endl;
	std::cout << "set data: " << (LPSTR)_data.mv_data << std::endl;

	int err2 = mdb_txn_commit(_txn);
	std::wcout << _T("mdb_txn_commit: ") << err2 << std::endl;
	mdb_env_sync(_env, TRUE);
	mdb_dbi_close(_env, _dbi);

	return true;
}

bool CLMDBWrapper::Get(LPSTR lpszKey, LPSTR * lpszValue)
{
	mdb_txn_begin(_env, NULL, 0, &_txn);
	int err = mdb_dbi_open(_txn, NULL, 0, &_dbi);
	std::wcout << _T("mdb_dbi_open: ") << err << std::endl;

	strcpy(_szKey, lpszKey);
	std::cout << "Get key: " << _szKey << std::endl;

	_key.mv_size = sizeof(_szKey);
	_key.mv_data = _szKey;
	//data.mv_size = sizeof(szValue);
	//data.mv_data = NULL; // szValue;

	err = mdb_get(_txn, _dbi, &_key, &_data);
	//printf("Get err=%d Key:%s Data:%s\n", err, key.mv_data, data.mv_data);
	std::wcout << _T("mdb_get: ") << err << std::endl;

	mdb_txn_commit(_txn);
	mdb_dbi_close(_env, _dbi);

	if (err == MDB_NOTFOUND)
	{
		lpszValue = NULL;
		return false;
	}
	else
	{
		*lpszValue = (char*)malloc(_data.mv_size + 1);
		strcpy(*lpszValue, (char *)_data.mv_data);
		std::cout << "get data: " << *lpszValue << std::endl;
		return true;
	}

	return true;
}

bool CLMDBWrapper::Get(std::string key, std::string & value)
{
	LPSTR lpszValue;
	if (Get((LPSTR)key.c_str(), &lpszValue) == true)
	{
		std::string v = std::string(lpszValue);
		free(lpszValue);
		value = v;
		return true;
	}
	else
		return false;
}

bool CLMDBWrapper::Set(std::string key, std::string value)
{
	return Set((LPSTR)key.c_str(), (LPSTR)value.c_str());
}

bool CLMDBWrapper::Get(std::wstring key, std::wstring & value)
{
	std::string keya(key.begin(), key.end());
	std::string valuea;
	bool ret = Get(keya, valuea);
	std::wstring valuew(valuea.begin(), valuea.end());
	value = valuew;
	return ret;
}

bool CLMDBWrapper::Set(std::wstring key, std::wstring value)
{
	std::string keya(key.begin(), key.end());
	std::string valuea(value.begin(), value.end());

	return Set(keya, valuea);
}
