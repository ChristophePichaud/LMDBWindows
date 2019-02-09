// ConsoleApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define E(expr) CHECK((rc = (expr)) == MDB_SUCCESS, #expr)
#define RES(err, expr) ((rc = expr) == (err) || (CHECK(!rc, #expr), 0))
#define CHECK(test, msg) ((test) ? (void)0 : ((void)fprintf(stderr, \
	"%s:%d: %s: %s\n", __FILE__, __LINE__, msg, mdb_strerror(rc)), abort()))

class CGetSet
{
public:
	CGetSet() {}
	~CGetSet() 
	{
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
		mdb_env_create(&env);
		mdb_env_set_maxreaders(env, 1);
		mdb_env_set_mapsize(env, 10485760 * 1000);
		mdb_env_open(env, db.c_str(), MDB_CREATE/*|MDB_NOSYNC*/, 0664);
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

		key.mv_size = k.length()+1;
		key.mv_data = (void *)k.c_str();
		data.mv_size = v.length()+1;
		data.mv_data = (void *)v.c_str();
		int err = mdb_put(txn, dbi, &key, &data, 0); // MDB_NOOVERWRITE);
		printf("Set err:%d Key:%s Data:%s\n", err, key.mv_data, data.mv_data);

		mdb_txn_commit(txn);
		mdb_dbi_close(env, dbi);
	}

	void Get(std::wstring k, std::wstring & v)
	{
		std::string key(k.begin(), k.end());
		std::string value;
		Get(key, value);

		std::wstring wvalue(value.begin(), value.end());
		v = wvalue;
	}

	void Get(std::string k, std::string & value)
	{
		mdb_txn_begin(env, NULL, 0, &txn);
		mdb_dbi_open(txn, NULL, 0, &dbi);

		key.mv_size = k.length()+1;
		key.mv_data = (void *)k.c_str();

		int err = mdb_get(txn, dbi, &key, &data);
		printf("Get err:%d Key:%s Data:%s\n", err, key.mv_data, data.mv_data);
		value = (char *)(data.mv_data);

		mdb_txn_commit(txn);
		mdb_dbi_close(env, dbi);
	}
};


int main5(int argc, char * argv[])
{
	int i = 0;

loop:
	if (i == 2)
		goto end;

	goto thanks;

you:
	std::cout << "you !" << std::endl;
	i++;
	goto loop;

thanks:
	std::cout << "Thanks ";
	goto you;

end:
	return 0;
}

int main(int argc, char * argv[])
{
	std::string db = "c:\\temp";
	std::string key = "key_v000";
	std::string value = "value_v000";

	CGetSet gs;
	gs.Init(db);
	gs.Set(key, value);
	std::string value1;
	gs.Get(key, value1);

	CGetSet gs2;
	gs2.Init(db);
	std::string value2;
	gs.Get(key, value2);
	
	return 0;
}

int main3(int argc, char * argv[])
{
	int i = 0, j = 0, rc;
	MDB_env *env;
	MDB_dbi dbi;
	char szKey[255];
	char szValue[255];
	MDB_val key, data;
	MDB_txn *txn;
	MDB_stat mst;
	MDB_cursor *cursor, *cur2;
	MDB_cursor_op op;
	int count;
	int *values;
	char sval[32] = "";
	char szDebug[255];

	count = 200;

	if (argc == 2)
	{
		count = atoi(argv[1]);
	}


	values = (int *)malloc(count * sizeof(int));

	for (i = 0; i<count; i++) {
		values[i] = rand() % 1024;
	}

	mdb_env_create(&env);
	mdb_env_set_maxreaders(env, 1);
	mdb_env_set_mapsize(env, 10485760 * 1000);
	mdb_env_open(env, "c:\\temp", MDB_CREATE/*|MDB_NOSYNC*/, 0664);

	mdb_txn_begin(env, NULL, 0, &txn);
	mdb_dbi_open(txn, NULL, 0, &dbi);

	sprintf(szKey, "key_%d", 10);
	sprintf(szValue, "value_%d", 10);

	key.mv_size = sizeof(szKey);
	key.mv_data = szKey;
	data.mv_size = sizeof(szValue);
	data.mv_data = szValue;
	int err = mdb_put(txn, dbi, &key, &data, 0); // MDB_NOOVERWRITE);
	printf("Set err:%d Key:%s Data:%s\n", err, key.mv_data, data.mv_data);

	mdb_txn_commit(txn);
	mdb_dbi_close(env, dbi);

	mdb_txn_begin(env, NULL, 0, &txn);
	mdb_dbi_open(txn, NULL, 0, &dbi);

	sprintf(szKey, "key_%d", 10);
	memset(szValue, 0, 255);

	key.mv_size = sizeof(szKey);
	key.mv_data = szKey;
	//data.mv_size = sizeof(szValue);
	//data.mv_data = szValue;

	err = mdb_get(txn, dbi, &key, &data);
	printf("Get err:%d Key:%s Data:%s\n", err, key.mv_data, data.mv_data);
	
	mdb_txn_commit(txn);
	mdb_dbi_close(env, dbi);

	mdb_env_close(env);
	return 0;
}

int main_11(int argc, char * argv[])
{
	int i = 0, j = 0, rc;
	MDB_env *env;
	MDB_dbi dbi;
	MDB_val key, data;
	MDB_txn *txn;
	MDB_stat mst;
	MDB_cursor *cursor, *cur2;
	MDB_cursor_op op;
	int count;
	int *values;
	char sval[32] = "";
	char szDebug[255];

	count = 200;

	if (argc == 2)
	{
		count = atoi(argv[1]);
	}


	values = (int *)malloc(count * sizeof(int));

	for (i = 0; i<count; i++) {
		values[i] = rand() % 1024;
	}

	E(mdb_env_create(&env));
	E(mdb_env_set_maxreaders(env, 1));	
	E(mdb_env_set_mapsize(env, 10485760 * 1000));
	E(mdb_env_open(env, "c:\\temp", MDB_CREATE/*|MDB_NOSYNC*/, 0664));

	E(mdb_txn_begin(env, NULL, 0, &txn));
	E(mdb_dbi_open(txn, NULL, 0, &dbi));
	mdb_txn_commit(txn);

	printf("Adding %d values\n", count);
	DWORD dwStart = GetTickCount();
	for (i = 0; i<count; i++)
	{
		E(mdb_txn_begin(env, NULL, 0, &txn));

		char szKey[255];
		char szValue[255];
		sprintf(szKey, "key_%d", i);
		sprintf(szValue, "value_%d", i);

		key.mv_size = sizeof(szKey);
		key.mv_data = szKey;
		data.mv_size = sizeof(szValue);
		data.mv_data = szValue;
		//printf("Add Key:%s Data:%s\n", key.mv_data, data.mv_data);
		mdb_put(txn, dbi, &key, &data, 0);
		//if (RES(MDB_KEYEXIST, mdb_put(txn, dbi, &key, &data, MDB_NOOVERWRITE)))
		//{
		//	j++;
		//	data.mv_size = sizeof(sval);
		//	data.mv_data = sval;
		//}
		mdb_txn_commit(txn);
	}

	mdb_dbi_close(env, dbi);

	DWORD dwStop = GetTickCount();
	sprintf_s(szDebug, ("Elapsed for put: %ld ms\n"), dwStop - dwStart);
	printf(szDebug);

	E(mdb_txn_begin(env, NULL, 0, &txn));
	E(mdb_dbi_open(txn, NULL, 0, &dbi));

	dwStart = GetTickCount();
	for (int i = 0; i < count; i++)
	{
		char szKey[255];
		char szValue[255];
		sprintf(szKey, "key_%d", i);
		memset(szValue, 0, 255);

		key.mv_size = sizeof(szKey);
		key.mv_data = szKey;
		//data.mv_size = sizeof(szValue);
		//data.mv_data = szValue;

		mdb_get(txn, dbi, &key, &data);
		//printf("Get Key:%s Data:%s\n", key.mv_data, data.mv_data);
	}
	mdb_txn_commit(txn);

	dwStop = GetTickCount();
	sprintf_s(szDebug, ("Elapsed for get: %ld ms\n"), dwStop - dwStart);
	printf(szDebug);

	mdb_dbi_close(env, dbi);
	mdb_env_close(env);
	return 0;
}

