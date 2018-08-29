#pragma once
#include "LMDBWindowsDll.h" 
#include "lmdb-dll.h"
#include "midl-dll.h"
#include "Constants.h"

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

	static void Init();
	bool Init(LPSTR lpszDatabase);
	bool Init(std::wstring database);
	bool Uninit();
	bool Set(LPSTR lpszKey, LPSTR lpszValue);
	bool Set(LPSTR lpszKey, LPSTR lpszValue, DWORD dwLen);
	bool Get(LPSTR lpszKey, LPSTR * lpszValue);
	bool Get(std::string key, std::string & value);
	bool Set(std::string key, std::string value);
	bool Get(std::wstring key, std::wstring & value);
	bool Set(std::wstring key, std::wstring value);

private:
	bool m_Init = false;

	MDB_env *_env;
	MDB_dbi _dbi;
	MDB_txn *_txn;
	MDB_val _key, _data;
	char _szKey[255];
	char _szValue[255];
};

class LMDBWRAPPER_API CLMDBWrapperEx
{
public:
	CLMDBWrapperEx() {}
	~CLMDBWrapperEx()
	{
		mdb_env_close(env);
	}

private:
	MDB_env * env;
	MDB_dbi dbi;
	char szKey[255];
	char szValue[255];
	MDB_val key, data;
	MDB_txn *txn;

public:

	void Init(std::wstring db)
	{
		std::string wdb(db.begin(), db.end());
		Init(wdb);
	}

	void Init(std::string db)
	{
		char sz[255];
		sprintf_s(sz, "%s\\%s", Constants::LMDBRootPath.c_str(), db.c_str());
		::CreateDirectoryA(sz, NULL);

		mdb_env_create(&env);
		mdb_env_set_maxreaders(env, 1);
		mdb_env_set_mapsize(env, 10485760 * 1000);
		mdb_env_open(env, sz, MDB_CREATE, 0);
	}

	void Set(std::wstring k, std::wstring v)
	{
		std::string key(k.begin(), k.end());
		std::string value(v.begin(), v.end());
		Set(key, value);
	}

	void Set(std::string k, std::string v)
	{
		mdb_txn_begin(env, NULL, 0, &txn);
		mdb_dbi_open(txn, NULL, 0, &dbi);

		key.mv_size = k.length() + 1;
		key.mv_data = (void *)k.c_str();
		data.mv_size = v.length() + 1;
		data.mv_data = (void *)v.c_str();
		int err = mdb_put(txn, dbi, &key, &data, 0); // MDB_NOOVERWRITE);
		printf("Set err:%d Key:%s Data:%s\n", err, key.mv_data, data.mv_data);

		mdb_txn_commit(txn);
		mdb_dbi_close(env, dbi);
	}

	bool Get(std::wstring k, std::wstring & v)
	{
		std::string key(k.begin(), k.end());
		std::string value;
		bool ret = Get(key, value);

		std::wstring wvalue(value.begin(), value.end());
		v = wvalue;
		return ret;
	}

	bool Get(std::string k, std::string & value)
	{
		mdb_txn_begin(env, NULL, 0, &txn);
		mdb_dbi_open(txn, NULL, 0, &dbi);

		key.mv_size = k.length() + 1;
		key.mv_data = (void *)k.c_str();

		int err = mdb_get(txn, dbi, &key, &data);
		printf("Get err:%d Key:%s Data:%s\n", err, key.mv_data, data.mv_data);
		value = (char *)(data.mv_data);

		mdb_txn_commit(txn);
		mdb_dbi_close(env, dbi);

		return err == 0 ? true : false;
	}
};

