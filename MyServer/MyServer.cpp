#include "stdafx.h"
#include "MyServer.h"
#include "..\Include\MyServer\Helper.h"
#include "WorkerNodeClient.h"
#include "..\Include\MyServer\Constants.h"


std::vector<std::shared_ptr<WorkerNodeAttributes>> MyServer::_nodes;

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

		::Sleep(10000);

		std::shared_ptr<WorkerNodeAttributes> pObj = nullptr;

		g_Guard.WaitToWrite();

		for (auto itr = _nodes.begin(); itr != _nodes.end(); itr++)
		{
			pObj = *itr;

			std::wstring url = CHelper::BuildURL(pObj->_server, pObj->_port);
			http_client client(url);

			std::wostringstream buf;
			buf << _REQUEST_ << _VERB_PING_;

			http_response response;

			try
			{
				wcout << _T("Ping testing ip:") << pObj->_server.c_str() << _T(" on port:") << pObj->_port.c_str() << endl;
				response = client.request(methods::GET, buf.str()).get();
				wcout << response.to_string() << endl;
			}
			catch (http_exception ex)
			{
				wcout << _T("http_exception... Node is removed.") << endl;
				std::string err = ex.what();
				// Something goes wrong !
				pObj->_isActive = false;
				*itr = pObj;

				_nodes.erase(itr);
				break;
			}

			json::value jdata = json::value::array();
			jdata = response.extract_json().get();

			if (jdata.is_null())
			{
				std::wcout << _T("no JSON data...") << std::endl;

				// Something goes wrong !
				pObj->_isActive = false;
				*itr = pObj;
				break;
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
				break;
			}

		}

		g_Guard.Done();
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
	std::wstring server = CHelper::FindParameterInQuery(query, _T("server"));
	std::wstring port = CHelper::FindParameterInQuery(query, _T("port"));
	std::wstring name = CHelper::FindParameterInQuery(query, _T("name"));
	std::wstring dbname = CHelper::FindParameterInQuery(query, _T("dbname"));

	if (request == _VERB_REGISTER_NODE_)
	{
		std::wcout << _VERB_REGISTER_NODE_ << std::endl;

		if (MyServer::ExistsNode(server, port, name) == true)
		{
			std::wcout << _T("Node already registered !") << std::endl;
		}
		else
		{
			std::shared_ptr<WorkerNodeAttributes> pNode = std::make_shared<WorkerNodeAttributes>();
			pNode->_server = server;
			pNode->_port = port;
			pNode->_name = name;
			pNode->_dbName = _T("");

			g_Guard.WaitToWrite();
			_nodes.push_back(pNode);
			std::wcout << _T("Node registered !") << _T(" count: ") << _nodes.size() << std::endl;
			g_Guard.Done();
		}

		message.reply(status_codes::OK);
		return;
	}
		
	if (request == _VERB_SHOW_NODE_)
	{
		std::wcout << _VERB_SHOW_NODE_ << std::endl;
		MyServer::ShowNodes();
		message.reply(status_codes::OK);
		return;
	}
	
	if (request == _VERB_GET_NODE_ )
	{
		std::wcout << _VERB_GET_NODE_ << std::endl;
			
		std::shared_ptr<WorkerNodeAttributes> pObj = nullptr;

		g_Guard.WaitToWrite();

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

		g_Guard.Done();

		return;
	}
		
	message.reply(status_codes::OK);
};

void MyServer::SendDbName(GetNodeData data)
{
	std::wstring url = CHelper::BuildURL(data.ip, data.port);
	std::wstring address = url;

	http_client client(address);

	std::wostringstream buf;
	buf << _REQUEST_ << _VERB_SET_NODE_
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

	std::shared_ptr<WorkerNodeAttributes> pObj = nullptr;

	g_Guard.WaitToWrite();

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

	g_Guard.Done();

	return ret;
}

void MyServer::ShowNodes()
{
	std::shared_ptr<WorkerNodeAttributes> pObj = nullptr;

	g_Guard.WaitToWrite();

	for (auto itr = _nodes.begin(); itr != _nodes.end(); itr++)
	{
		pObj = *itr;

		TCHAR sz[255];
		_stprintf_s(sz, _T("Active:%d Server:%s Port:%s Name:%s DBName:%s\n"), pObj->_isActive, pObj->_server.c_str(), pObj->_port.c_str(), pObj->_name.c_str(), pObj->_dbName.c_str());
		_tprintf(sz);
	}

	g_Guard.Done();
}



