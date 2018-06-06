// ConsoleApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define E(expr) CHECK((rc = (expr)) == MDB_SUCCESS, #expr)
#define RES(err, expr) ((rc = expr) == (err) || (CHECK(!rc, #expr), 0))
#define CHECK(test, msg) ((test) ? (void)0 : ((void)fprintf(stderr, \
	"%s:%d: %s: %s\n", __FILE__, __LINE__, msg, mdb_strerror(rc)), abort()))

int main(int argc, char * argv[])
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

	printf("Adding %d values\n", count);
	DWORD dwStart = GetTickCount();
	for (i = 0; i<count; i++)
	{
		char szKey[255];
		char szValue[255];
		sprintf(szKey, "key_%d", i);
		sprintf(szValue, "value_%d", i);

		key.mv_size = sizeof(szKey);
		key.mv_data = szKey;
		data.mv_size = sizeof(szValue);
		data.mv_data = szValue;
		printf("Add Key:%s Data:%s\n", key.mv_data, data.mv_data);
		if (RES(MDB_KEYEXIST, mdb_put(txn, dbi, &key, &data, MDB_NOOVERWRITE))) 
		{
			j++;
			data.mv_size = sizeof(sval);
			data.mv_data = sval;
		}
	}
	DWORD dwStop = GetTickCount();
	sprintf_s(szDebug, ("Elapsed for put: %ld ms\n"), dwStop - dwStart);
	printf(szDebug);

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
		printf("Get Key:%s Data:%s\n", key.mv_data, data.mv_data);
		}
	dwStop = GetTickCount();
	sprintf_s(szDebug, ("Elapsed for get: %ld ms\n"), dwStop - dwStart);
	printf(szDebug);

	mdb_dbi_close(env, dbi);
	mdb_env_close(env);
	return 0;
}

