#include "stdafx.h"
#include "MyServer\\messagetypes.h"
#include "MyServer.h"

using namespace std;
using namespace web; 
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

LMDBData MyServer::m_lmdb;

MyServer::MyServer(utility::string_t url) : m_listener(url)
{
	std::function<void(http_request)> fnGet = &MyServer::handle_get;
	m_listener.support(methods::GET, fnGet);

	std::function<void(http_request)> fnPut = &MyServer::handle_put;
	m_listener.support(methods::PUT, fnPut);

	std::function<void(http_request)> fnPost = &MyServer::handle_post;
    m_listener.support(methods::POST, fnPost);

	std::function<void(http_request)> fnDel = &MyServer::handle_delete;
    m_listener.support(methods::DEL, fnDel);
   
}

void MyServer::Init_LMDB()
{
	mdb_env_create(&m_lmdb.m_env);
	mdb_env_set_maxreaders(m_lmdb.m_env, 1);
	mdb_env_set_mapsize(m_lmdb.m_env, 10485760);
	mdb_env_open(m_lmdb.m_env, "c:\\temp", MDB_CREATE/*|MDB_NOSYNC*/, 0664);
}

void MyServer::handle_post(http_request message)
{
    ucout <<  message.to_string() << endl;
	message.reply(status_codes::OK);
};

void MyServer::handle_delete(http_request message)
{
	ucout << message.to_string() << endl;
	message.reply(status_codes::OK);
}

void MyServer::handle_put(http_request message)
{
	ucout << message.to_string() << endl;
	message.reply(status_codes::OK);
};

void MyServer::handle_get(http_request message)
{
	//ucout << U("Message") << U(" ") << message.to_string() << endl;
	//ucout << U("Relative URI") << U(" ") << message.relative_uri().to_string() << endl;

	//auto paths = uri::split_path(uri::decode(message.relative_uri().path()));
	//for (auto it1 = paths.begin(); it1 != paths.end(); it1++)
	//{
	//	ucout << U("Path") << U(" ") << *it1 << endl;
	//}

	auto query = uri::split_query(uri::decode(message.relative_uri().query()));
	//for (auto it2 = query.begin(); it2 != query.end(); it2++)
	//{
	//	ucout << U("Query") << U(" ") << it2->first << U(" ") << it2->second << endl;
	//}

	auto queryItr = query.find(U("request"));
	utility::string_t request = queryItr->second;
	//ucout << U("Request") << U(" ") << request << endl;

	auto keyItr = query.find(U("key"));
	utility::string_t key; 
	if (keyItr != query.end())
	{
		key = keyItr->second;
		//ucout << U("key") << U(" ") << key << endl;
	}

	auto valueItr = query.find(U("value"));
	utility::string_t value;
	if (valueItr != query.end())
	{
		value = valueItr->second;
		//ucout << U("value") << U(" ") << value << endl;
	}

	if (request == U("get-data"))
	{
		TCHAR szKey[255];
		TCHAR szValue[255];

		MDB_val VKey;
		MDB_val VData;

		_tcscpy(szKey, key.c_str());

		VKey.mv_size = sizeof(szKey);
		VKey.mv_data = szKey;
		VData.mv_size = sizeof(szValue);
		VData.mv_data = szValue;

		mdb_txn_begin(m_lmdb.m_env, NULL, 0, &m_lmdb.m_txn);
		mdb_dbi_open(m_lmdb.m_txn, NULL, 0, &m_lmdb.m_dbi); 
		int err = mdb_get(m_lmdb.m_txn, m_lmdb.m_dbi, &VKey, &VData);
		mdb_txn_commit(m_lmdb.m_txn);
		mdb_env_stat(m_lmdb.m_env, &m_lmdb.m_mst);

		if (err == MDB_NOTFOUND)
		{
			Data data;
			data.key = szKey;
			data.value = U("");

			utility::string_t response = data.AsJSON().serialize();
			//ucout << response << endl;

			message.reply(status_codes::OK, data.AsJSON());
		}
		else
		{
			Data data;
			data.key = szKey;
			data.value = (TCHAR *)VData.mv_data;

			utility::string_t response = data.AsJSON().serialize();
			//ucout << response << endl;

			message.reply(status_codes::OK, data.AsJSON());
		}
		return;
	}

	if (request == U("set-data"))
	{
		TCHAR szKey[255];
		TCHAR szValue[255];

		MDB_val VKey;
		MDB_val VData;

		_tcscpy(szKey, key.c_str());
		_tcscpy(szValue, value.c_str());

		VKey.mv_size = sizeof(szKey);
		VKey.mv_data = szKey;
  		VData.mv_size = sizeof(szValue);
		VData.mv_data = szValue;
		//printf("Add Key:%s Data:%s\n", key.mv_data, data.mv_data);
		mdb_txn_begin(m_lmdb.m_env, NULL, 0, &m_lmdb.m_txn);
		mdb_dbi_open(m_lmdb.m_txn, NULL, 0, &m_lmdb.m_dbi);
		mdb_put(m_lmdb.m_txn, m_lmdb.m_dbi, &VKey, &VData, MDB_NOOVERWRITE);
		mdb_txn_commit(m_lmdb.m_txn);
		mdb_env_stat(m_lmdb.m_env, &m_lmdb.m_mst);

		Data data;
		data.key = szKey;
		data.value = szValue;

		utility::string_t response = data.AsJSON().serialize();
		//ucout << response << endl;

		message.reply(status_codes::OK, data.AsJSON());
		return;
	}

	message.reply(status_codes::OK);
};

