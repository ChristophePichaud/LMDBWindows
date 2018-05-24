#include "stdafx.h"
#include "WorkerNodeClient.h"
#include "Helper.h"
#include "LMDBData.h"
#include "MyServer\\messagetypes.h"

LMDBData WorkerNodeClient::m_lmdb;
std::wstring WorkerNodeClient::_dbName;
std::wstring WorkerNodeClient::_server;
std::wstring  WorkerNodeClient::_port;
std::wstring WorkerNodeClient::_name;


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// WorkerNodeClient class
//
////////////////////////////////////////////////////////////////////////////////////////////////////


WorkerNodeClient::WorkerNodeClient(std::wstring url) : m_listener(url)
{
	std::function<void(http_request)> fnGet = &WorkerNodeClient::handle_get;
	m_listener.support(methods::GET, fnGet);
	this->_url = url;
}

WorkerNodeClient::~WorkerNodeClient()
{
}

void WorkerNodeClient::Init()
{
	this->_http = std::unique_ptr<WorkerNodeClient>(new WorkerNodeClient(this->_url));
	this->_http->open().wait();
	RegisterToMaster();
}

void WorkerNodeClient::Close()
{
	this->_http->close().wait();
}

bool WorkerNodeClient::RegisterToMaster()
{
	std::wstring ip = CHelper::GetIP();
	//std::wcout << L"IP : " << ip << std::endl;

	std::wstring port = _T("7001");
	std::wstring url = CHelper::BuildURL(ip, port);
	std::wstring address = url;

	std::wostringstream buf;
	buf << _T("?request=") << _T("register-node")
		<< _T("&server=") << this->_server
		<< _T("&port=") << this->_port
		<< _T("&name=") << this->_name;

	http_response response;
	http_client client(address);
	response = client.request(methods::GET, buf.str()).get();
	wcout << response.to_string() << endl;

	return true;
}

void WorkerNodeClient::handle_get(http_request message)
{
	std::wcout << _T("handle_get") << std::endl;

	std::wcout << _T("Message") << _T(" ") << message.to_string() << endl;
	std::wcout << _T("Relative URI") << _T(" ") << message.relative_uri().to_string() << endl;

	auto paths = uri::split_path(uri::decode(message.relative_uri().path()));
	for (auto it1 = paths.begin(); it1 != paths.end(); it1++)
	{
		std::wcout << _T("Path") << _T(" ") << *it1 << endl;
	}

	auto query = uri::split_query(uri::decode(message.relative_uri().query()));
	for (auto it2 = query.begin(); it2 != query.end(); it2++)
	{
		std::wcout << _T("Query") << _T(" ") << it2->first << _T(" ") << it2->second << endl;
	}

	auto queryItr = query.find(_T("request"));
	std::wstring request = queryItr->second;
	std::wcout << _T("Request") << _T(" ") << request << endl;

	auto dbnameItr = query.find(_T("dbname"));
	std::wstring dbname;
	if (dbnameItr != query.end())
	{
		dbname = dbnameItr->second;
		std::wcout << _T("dbname") << _T(" ") << dbname << endl;
	}

	auto keyItr = query.find(_T("key"));
	std::wstring key;
	if (keyItr != query.end())
	{
		key = keyItr->second;
		std::wcout << _T("key") << _T(" ") << key << endl;
	}

	auto valueItr = query.find(_T("value"));
	std::wstring value;
	if (valueItr != query.end())
	{
		value = valueItr->second;
		std::wcout << _T("value") << _T(" ") << value << endl;
	}

	if (request == _T("ping"))
	{
		std::wcout << _T("ping...") << std::endl;

		PingData data;
		data.ip = _server;
		data.port = _port;
		data.status = _T("OK");

		std::wstring response = data.AsJSON().serialize();
		std::wcout << response << endl;

		message.reply(status_codes::OK, data.AsJSON());
	}

	if (request == _T("set-node"))
	{
		std::wcout << _T("set-node...") << std::endl;
		_dbName = dbname;
		message.reply(status_codes::OK);

		Init_LMDB();
	}

	if (request == _T("get-data"))
	{
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
		mdb_env_stat(m_lmdb.m_env, &m_lmdb.m_mst);

		if (err == MDB_NOTFOUND)
		{
			Data data;
			data.key = szKey;
			data.value = _T("");

			std::wstring response = data.AsJSON().serialize();
			std::wcout << response << endl;

			message.reply(status_codes::OK, data.AsJSON());
		}
		else
		{
			Data data;
			data.key = szKey;
			data.value = (TCHAR *)VData.mv_data;

			std::wstring response = data.AsJSON().serialize();
			std::wcout << response << endl;

			message.reply(status_codes::OK, data.AsJSON());
		}

		return;
	}

	if (request == _T("set-data"))
	{
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
		_tprintf(_T("Add Key:%s Data:%s\n"), szKey, szValue);
		mdb_txn_begin(m_lmdb.m_env, NULL, 0, &m_lmdb.m_txn);
		mdb_dbi_open(m_lmdb.m_txn, NULL, 0, &m_lmdb.m_dbi);
		mdb_put(m_lmdb.m_txn, m_lmdb.m_dbi, &VKey, &VData, MDB_NOOVERWRITE);
		mdb_txn_commit(m_lmdb.m_txn);
		mdb_env_stat(m_lmdb.m_env, &m_lmdb.m_mst);

		Data data;
		data.key = szKey;
		data.value = szValue;

		std::wstring response = data.AsJSON().serialize();
		std::wcout << response << endl;

		message.reply(status_codes::OK, data.AsJSON());
		return;
	}

	message.reply(status_codes::OK);
}

void WorkerNodeClient::Init_LMDB()
{
	USES_CONVERSION;

	char sz[255];
	sprintf(sz, "c:\\temp\\%s", W2A(_dbName.c_str()));
	::CreateDirectoryA(sz, NULL);

	std::wcout << _T("Init LMDB") << std::endl;
	mdb_env_create(&m_lmdb.m_env);
	mdb_env_set_maxreaders(m_lmdb.m_env, 1);
	mdb_env_set_mapsize(m_lmdb.m_env, 10485760);
	mdb_env_open(m_lmdb.m_env, sz, MDB_CREATE/*|MDB_NOSYNC*/, 0664);
}

