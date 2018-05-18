#include "stdafx.h"
#include "MyServer\\messagetypes.h"
#include "MyServer.h"
#include "Helper.h"

using namespace std;
using namespace web; 
using namespace utility;
using namespace http;
using namespace http::client;
using namespace web::http::experimental::listener;

LMDBData MyServer::m_lmdb;
std::vector<std::shared_ptr<NodeAttributes>> MyServer::_nodes;

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
	//this->_http->Init_LMDB();
	this->_http->open().wait();
}

void MyServer::Init_LMDB()
{
	std::wcout << _T("Init LMDB") << std::endl;
	mdb_env_create(&m_lmdb.m_env);
	mdb_env_set_maxreaders(m_lmdb.m_env, 1);
	mdb_env_set_mapsize(m_lmdb.m_env, 10485760);
	mdb_env_open(m_lmdb.m_env, "c:\\temp", MDB_CREATE/*|MDB_NOSYNC*/, 0664);
}

void MyServer::Close()
{
	this->_http->close().wait();
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

	if (request == _T("register-node"))
	{
		std::wcout << _T("register-node...") << std::endl;

		std::wcout << _T("server: ") << server << std::endl;
		std::wcout << _T("port: ") << port << std::endl;

		if (MyServer::ExistsNode(server, port) == true)
		{
			std::wcout << _T("Node already registered !") << std::endl;
		}
		else
		{
			std::shared_ptr<NodeAttributes> pNode = std::make_shared<NodeAttributes>();
			pNode->_server = server;
			pNode->_port = port;

			_nodes.push_back(pNode);
			std::wcout << _T("Node registered !") << _T(" count: ") << _nodes.size() << std::endl;
		}
	}
		
	if (request == _T("show-nodes"))
	{
		std::wcout << _T("show-nodes...") << std::endl;
		MyServer::ShowNodes();
	}
	
	if (request == _T("get-node"))
	{
		std::wcout << _T("get-node...") << std::endl;
			
		std::shared_ptr<NodeAttributes> pObj = nullptr;

		for (auto itr = _nodes.begin(); itr != _nodes.end(); itr++)
		{
			pObj = *itr;
			if (pObj->_isActive == false)
			{
				// We find an entry
				pObj->_isActive = true;
				*itr = pObj;
				break;
			}
		}
				
		if (pObj != nullptr)
		{
			GetNodeData data;
			data.ip = pObj->_server;
			data.port = pObj->_port;
			data.name = name;

			pObj->_name = name;

			std::wstring response = data.AsJSON().serialize();
			std::wcout << response << endl;

			message.reply(status_codes::OK, data.AsJSON());
		}
		else
		{
			std::wcout << _T("No node available...") << std::endl;
			message.reply(status_codes::OK);
		}
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
};

bool MyServer::ExistsNode(std::wstring server, std::wstring port)
{
	bool ret = false;

	std::shared_ptr<NodeAttributes> pObj = nullptr;

	for (auto itr = _nodes.begin(); itr != _nodes.end(); itr++)
	{
		pObj = *itr;

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
		_stprintf(sz, _T("Active:%d Server:%s Port:%s\n"), pObj->_isActive, pObj->_server.c_str(), pObj->_port.c_str());
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
	std::wcout << L"IP : " << ip << std::endl;

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
}
