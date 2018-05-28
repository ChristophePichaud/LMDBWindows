#include "stdafx.h"
#include "WorkerNodeClient.h"
#include "LMDBData.h"
#include "MyServer\\messagetypes.h"
#include "..\Include\MyServer\Helper.h"
#include "..\Include\MyServer\Constants.h"


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
	std::wstring port = Constants::MasterNodePort;
	std::wstring url = CHelper::BuildURL(ip, port);

	std::wostringstream buf;
	buf << Constants::Request << Constants::VerbRegisterNode
		<< _T("&server=") << this->_server
		<< _T("&port=") << this->_port
		<< _T("&name=") << this->_name;

	http_response response;
	http_client client(url);
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

	std::wstring request = CHelper::FindParameterInQuery(query, _T("request"));
	std::wstring dbname = CHelper::FindParameterInQuery(query, _T("dbname"));
	std::wstring key = CHelper::FindParameterInQuery(query, _T("key"));
	std::wstring value = CHelper::FindParameterInQuery(query, _T("value"));

	if (request == Constants::VerbPing)
	{
		std::wcout << Constants::VerbPing << std::endl;

		PingData data;
		data.ip = _server;
		data.port = _port;
		data.status = _T("OK");

		std::wstring response = data.AsJSON().serialize();
		std::wcout << response << endl;

		message.reply(status_codes::OK, data.AsJSON());
	}

	if (request == Constants::VerbSetNode)
	{
		std::wcout << Constants::VerbSetNode << std::endl;
		_dbName = dbname;
		message.reply(status_codes::OK);

		Init_LMDB();
	}

	if (request == Constants::VerbGetData)
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

	if (request == Constants::VerbSetData)
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
	sprintf_s(sz, "%s\\%s", Constants::LMDBRootPath.c_str(), W2A(_dbName.c_str()));
	::CreateDirectoryA(sz, NULL);

	std::wcout << _T("Init LMDB") << std::endl;
	mdb_env_create(&m_lmdb.m_env);
	mdb_env_set_maxreaders(m_lmdb.m_env, 1);
	mdb_env_set_mapsize(m_lmdb.m_env, 10485760);
	mdb_env_open(m_lmdb.m_env, sz, MDB_CREATE/*|MDB_NOSYNC*/, 0664);
}

