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
	mdb_env_set_mapsize(_env, 10485760 * 100);
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
	//std::wcout << _T("mdb_dbi_open: ") << err << std::endl;

	_key.mv_size = strlen(lpszKey);
	_key.mv_data = lpszKey;
	_data.mv_size = strlen(lpszValue);
	_data.mv_data = lpszValue;

	err = mdb_put(_txn, _dbi, &_key, &_data, MDB_NOOVERWRITE);
	
	std::wostringstream buf;
	buf << _T("mdb_put:") << err;
	BasicLogger::InternalLogger.WriteLog(buf.str());
		
	std::wostringstream buf2;
	buf2 << _T("set: key:") << lpszKey << _T(" value:") << lpszValue;
	BasicLogger::InternalLogger.WriteLog(buf2.str());

	mdb_txn_commit(_txn);
	mdb_env_sync(_env, TRUE);
	mdb_dbi_close(_env, _dbi);

	return true;
}

bool CLMDBWrapper::Set(LPSTR lpszKey, LPSTR lpszValueb64, DWORD dwLen)
{
	mdb_txn_begin(_env, NULL, 0, &_txn);
	int err = mdb_dbi_open(_txn, NULL, 0, &_dbi);

	strcpy(_szKey, lpszKey);
	std::string buffer = std::string(lpszValueb64);

	_key.mv_size = sizeof(_szKey);
	_key.mv_data = _szKey;
	_data.mv_size = buffer.length();
	_data.mv_data = (LPSTR)buffer.c_str();

	err = mdb_put(_txn, _dbi, &_key, &_data, MDB_NOOVERWRITE);

	std::wostringstream buf;
	buf << _T("mdb_put:") << err;
	BasicLogger::InternalLogger.WriteLog(buf.str());

	std::wostringstream buf2;
	buf2 << _T("set: key:") << lpszKey << _T(" value:") << lpszValueb64;
	BasicLogger::InternalLogger.WriteLog(buf2.str());

	mdb_txn_commit(_txn);
	mdb_env_sync(_env, TRUE);
	mdb_dbi_close(_env, _dbi);

	return true;
}

bool CLMDBWrapper::Get(LPSTR lpszKey, LPSTR * lpszValue)
{
	/*
	mdb_txn_begin(_env, NULL, 0, &_txn);
	int err = mdb_dbi_open(_txn, NULL, 0, &_dbi);

	strcpy(_szKey, lpszKey);
	_key.mv_size = sizeof(_szKey);
	_key.mv_data = _szKey;
	_data.mv_size = 0;
	//_data.mv_data = NULL;

	err = mdb_get(_txn, _dbi, &_key, &_data);

	std::wostringstream buf;
	buf << _T("mdb_get:") << err;
	BasicLogger::InternalLogger.WriteLog(buf.str());

	mdb_txn_commit(_txn);
	mdb_dbi_close(_env, _dbi);

	if (err == MDB_NOTFOUND)
	{
		lpszValue = NULL;

		std::wostringstream buf2;
		buf2 << _T("mdb_get data not found...");
		BasicLogger::InternalLogger.WriteLog(buf2.str());
		return false;
	}
	else
	{
		*lpszValue = (char*)malloc(_data.mv_size + 1);
		memset(*lpszValue, 0, _data.mv_size + 1);
		strcpy(*lpszValue, (char *)_data.mv_data);

		std::wostringstream buf2;
		buf2 << _T("get: ") << *lpszValue;
		BasicLogger::InternalLogger.WriteLog(buf2.str());
		return true;
	}

	return true;
	*/

	char szKey[255];
	//char szValue[255];

	strcpy(szKey, lpszKey);
	//strcpy(szValue, "");

	MDB_val VKey;
	MDB_val VData;

	VKey.mv_size = sizeof(szKey);
	VKey.mv_data = szKey;
	VData.mv_size = 0; // sizeof(szValue);

	mdb_txn_begin(_env, NULL, 0, &_txn);
	mdb_dbi_open(_txn, NULL, 0, &_dbi);
	int err = mdb_get(_txn, _dbi, &VKey, &VData);
	//printf("Get Key:%s Data:%s\n", VKey.mv_data, VData.mv_data);
	mdb_txn_commit(_txn);

	if (err == MDB_NOTFOUND)
	{
		lpszValue = NULL;
		return false;
	}
	else
	{
		*lpszValue = (char*)malloc(VData.mv_size + 1);
		strcpy(*lpszValue, (char *)VData.mv_data);
		return true;
	}

	return true;
}

bool CLMDBWrapper::Get(std::string key, std::string & value)
{
	LPSTR lpszValue;
	if (Get((LPSTR)key.c_str(), &lpszValue) == true)
	{
		std::string v = lpszValue;
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

void CLMDBWrapper::Init()
{
	InitLogger();
}
