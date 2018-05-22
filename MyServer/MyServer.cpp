#include "stdafx.h"
#include "MyServer.h"
#include "Helper.h"

using namespace std;
using namespace web; 
using namespace utility;
using namespace http;
using namespace http::client;
using namespace web::http::experimental::listener;

std::vector<std::shared_ptr<NodeAttributes>> MyServer::_nodes;
LMDBData NodeClient::m_lmdb;
std::wstring NodeClient::_dbName;
std::wstring NodeClient::_server;
std::wstring  NodeClient::_port;
std::wstring NodeClient::_name;

MyServer::MyServer(std::wstring url) : m_listener(url)
{
	std::function<void(http_request)> fnGet = &MyServer::handle_get;
	m_listener.support(methods::GET, fnGet);

	std::function<void(http_request)> fnPut = &MyServer::handle_put;
	m_listener.support(methods::PUT, fnPut);

	std::function<void(http_request)> fnPost = &MyServer::handle_post;
    m_listener.support(methods::POST, fnPost);

	std::function<void(http_request)> fnDel = &MyServer::handle_delete;
    m_listener.support(methods::DEL, fnDel);
   
	_url = url;
}

void MyServer::Init()
{
	this->_http = std::unique_ptr<MyServer>(new MyServer(this->_url));
	this->_http->open().wait();
	ActivatePingThread();
}

void MyServer::Close()
{
	this->_http->close().wait();
}

void MyServer::ActivatePingThread()
{
	DWORD dwThreadId = 0;
	HANDLE hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &MyServer::PingThread, NULL, 0, &dwThreadId);
}

void MyServer::PingThread(LPVOID param)
{
	while (TRUE)
	{

		::Sleep(2000);

		std::shared_ptr<NodeAttributes> pObj = nullptr;

		for (auto itr = _nodes.begin(); itr != _nodes.end(); itr++)
		{
			pObj = *itr;

			TCHAR sz[255];
			_stprintf(sz, _T("http://%s:%s/MyServer/LMDB/"), pObj->_server.c_str(), pObj->_port.c_str());

			std::wstring address = sz;

			http_client client(address);

			std::wostringstream buf;
			buf << _T("?request=") << _T("ping");

			http_response response;

			try
			{
				wcout << _T("Ping testing ip:") << pObj->_server.c_str() << _T(" on port:") << pObj->_port.c_str() << endl;
				response = client.request(methods::GET, buf.str()).get();
				wcout << response.to_string() << endl;
			}
			catch (...)
			{
				// Something goes wrong !
				pObj->_isActive = false;
				*itr = pObj;
			}

			json::value jdata = json::value::array();
			jdata = response.extract_json().get();

			if (jdata.is_null())
			{
				std::wcout << _T("no JSON data...") << std::endl;

				// Something goes wrong !
				pObj->_isActive = false;
				*itr = pObj;
				continue;
			}

			PingData data = PingData::FromJSON(jdata.as_object());

			if (data.status == _T("OK"))
			{
				// Normal behaviour
			}
			else
			{
				// Something goes wrong !
				pObj->_isActive = false;
				*itr = pObj;
			}

			::Sleep(1000);
		}
	}
}

void MyServer::handle_post(http_request message)
{
	std::wcout <<  message.to_string() << endl;
	message.reply(status_codes::OK);
};

void MyServer::handle_delete(http_request message)
{
	std::wcout << message.to_string() << endl;
	message.reply(status_codes::OK);
}

void MyServer::handle_put(http_request message)
{
	std::wcout << message.to_string() << endl;
	message.reply(status_codes::OK);
};

void MyServer::handle_get(http_request message)
{
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

	auto serverItr = query.find(_T("server"));
	std::wstring server;
	if (serverItr != query.end())
	{
		server = serverItr->second;
		std::wcout << _T("server") << _T(" ") << server << endl;
	}

	auto portItr = query.find(_T("port"));
	std::wstring port;
	if (portItr != query.end())
	{
		port = portItr->second;
		std::wcout << _T("port") << _T(" ") << port << endl;
	}

	auto nameItr = query.find(_T("name"));
	std::wstring name;
	if (nameItr != query.end())
	{
		name = nameItr->second;
		std::wcout << _T("name") << _T(" ") << name << endl;
	}

	auto dbnameItr = query.find(_T("dbname"));
	std::wstring dbname;
	if (dbnameItr != query.end())
	{
		dbname = dbnameItr->second;
		std::wcout << _T("dbname") << _T(" ") << dbname << endl;
	}

	if (request == _T("register-node"))
	{
		std::wcout << _T("register-node...") << std::endl;

		std::wcout << _T("server: ") << server << std::endl;
		std::wcout << _T("port: ") << port << std::endl;
		std::wcout << _T("name: ") << name << std::endl;

		if (MyServer::ExistsNode(server, port, name) == true)
		{
			std::wcout << _T("Node already registered !") << std::endl;
		}
		else
		{
			std::shared_ptr<NodeAttributes> pNode = std::make_shared<NodeAttributes>();
			pNode->_server = server;
			pNode->_port = port;
			pNode->_name = name;
			pNode->_dbName = _T("");

			_nodes.push_back(pNode);
			std::wcout << _T("Node registered !") << _T(" count: ") << _nodes.size() << std::endl;
		}

		message.reply(status_codes::OK);
		return;
	}
		
	if (request == _T("show-nodes"))
	{
		std::wcout << _T("show-nodes...") << std::endl;
		MyServer::ShowNodes();
		message.reply(status_codes::OK);
		return;
	}
	
	if (request == _T("get-node"))
	{
		std::wcout << _T("get-node...") << std::endl;
			
		std::shared_ptr<NodeAttributes> pObj = nullptr;

		for (auto itr = _nodes.begin(); itr != _nodes.end(); itr++)
		{
			pObj = *itr;

			if (pObj->_dbName == dbname) // && pObj->_isActive == true
			{
				// We find an existing entry
				*itr = pObj;
				break;
			}
			else if (pObj->_isActive == false)
			{
				// We find an entry
				pObj->_isActive = true;
				pObj->_dbName = dbname;
				*itr = pObj;
				break;
			}
			else
				pObj = nullptr;
		}
				
		if (pObj != nullptr)
		{
			GetNodeData data;
			data.ip = pObj->_server;
			data.port = pObj->_port;
			data.name = pObj->_name;
			data.dbName = dbname;

			std::wstring response = data.AsJSON().serialize();
			std::wcout << response << endl;

			message.reply(status_codes::OK, data.AsJSON());

			MyServer::SendDbName(data);
		}
		else
		{
			std::wcout << _T("No node available...") << std::endl;
			message.reply(status_codes::OK);
		}

		return;
	}
		
	message.reply(status_codes::OK);
};

void MyServer::SendDbName(GetNodeData data)
{
	TCHAR sz[255];
	_stprintf(sz, _T("http://%s:%s/MyServer/LMDB/"), data.ip.c_str(), data.port.c_str());

	std::wstring address = sz;

	http_client client(address);

	std::wostringstream buf;
	buf << _T("?request=") << _T("set-node")
		<< _T("&dbname=") << data.dbName;

	http_response response;

	response = client.request(methods::GET, buf.str()).get();
	wcout << response.to_string() << endl;

	json::value jdata = json::value::array();
	jdata = response.extract_json().get();

	if (jdata.is_null())
	{
		std::wcout << _T("no JSON data...") << std::endl;
		return;
	}
}

bool MyServer::ExistsNode(std::wstring server, std::wstring port, std::wstring name)
{
	bool ret = false;

	std::shared_ptr<NodeAttributes> pObj = nullptr;

	for (auto itr = _nodes.begin(); itr != _nodes.end(); itr++)
	{
		pObj = *itr;

		// DB in use
		if (pObj->_name == name)
		{
			ret = true;
			break;
		}

		// port in use
		if (pObj->_server == server && pObj->_port == port)
		{
			ret = true;
			break;
		}
	}

	return ret;
}

void MyServer::ShowNodes()
{
	std::shared_ptr<NodeAttributes> pObj = nullptr;

	for (auto itr = _nodes.begin(); itr != _nodes.end(); itr++)
	{
		pObj = *itr;

		TCHAR sz[255];
		_stprintf(sz, _T("Active:%d Server:%s Port:%s Name:%s DBName:%s\n"), pObj->_isActive, pObj->_server.c_str(), pObj->_port.c_str(), pObj->_name.c_str(), pObj->_dbName.c_str());
		_tprintf(sz);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// NodeClient class
//
////////////////////////////////////////////////////////////////////////////////////////////////////

NodeClient::NodeClient(std::wstring url) : m_listener(url)
{
	std::function<void(http_request)> fnGet = &NodeClient::handle_get;
	m_listener.support(methods::GET, fnGet);
	this->_url = url;
}

void NodeClient::Init()
{
	this->_http = std::unique_ptr<NodeClient>(new NodeClient(this->_url));
	this->_http->open().wait();
	RegisterToMaster();
}

void NodeClient::Close()
{
	this->_http->close().wait();
}

bool NodeClient::RegisterToMaster()
{
	std::wstring ip = CHelper::GetIP();
	//std::wcout << L"IP : " << ip << std::endl;

	int port = 7001;
	TCHAR sz[255];
	_stprintf(sz, _T("http://%s:%d/MyServer/LMDB/"), ip.c_str(), port);

	std::wstring address = sz;

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

void NodeClient::handle_get(http_request message)
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

void NodeClient::Init_LMDB()
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

