#include "stdafx.h"
#include "MyServer.h"
#include "..\Include\ServerHelper.h"
#include "WorkerNodeClient.h"
#include "..\Include\Constants.h"
#include "MyServerHelper.h"


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
	TCHAR sz[255];

	while (TRUE)
	{

		::Sleep(10000);

		std::shared_ptr<WorkerNodeAttributes> pObj = nullptr;

		g_Guard.WaitToWrite();

		for (auto itr = _nodes.begin(); itr != _nodes.end(); itr++)
		{
			pObj = *itr;

			std::wstring url = ServerHelper::BuildURL(pObj->_server, pObj->_port);
			http_client client(url);

			std::wostringstream buf;
			buf << Constants::Request << Constants::VerbPing;

			http_response response;

			try
			{
				_stprintf_s(sz, _T("Ping testing ip:%s on port:%s"), pObj->_server.c_str(), pObj->_port.c_str());
				g_Logger.WriteLog(sz);
				response = client.request(methods::GET, buf.str()).get();
				g_Logger.WriteLog(response.to_string().c_str());
			}
			catch (http_exception ex)
			{
				g_Logger.WriteLog(_T("http_exception... Node is removed."));
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
				g_Logger.WriteLog(_T("no JSON data..."));

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
	g_Logger.WriteLog(message.to_string().c_str());
	message.reply(status_codes::OK);
};

void MyServer::handle_delete(http_request message)
{
	g_Logger.WriteLog(message.to_string().c_str());
	message.reply(status_codes::OK);
}

void MyServer::handle_put(http_request message)
{
	g_Logger.WriteLog(message.to_string().c_str());
	message.reply(status_codes::OK);
};

void MyServer::handle_get(http_request message)
{
	g_Logger.WriteLog(_T("handle_get"));

	MyServerHelper::PrintRequest(message);

	std::wstring request = ServerHelper::FindParameter(message, _T("request"));

	if (request == Constants::VerbRegisterNode)
	{
		RequestVerbRegisterNode(message);
		return;
	}
		
	if (request == Constants::VerbShowNodes)
	{
		RequestVerbShowNodes(message);
		return;
	}
	
	if (request == Constants::VerbGetNode)
	{
		RequestVerbGetNode(message);
		return;
	}

	if (request == Constants::VerbReleaseNode)
	{
		RequestVerbReleaseNode(message);
		return;
	}
		
	message.reply(status_codes::OK);
};

void MyServer::RequestVerbRegisterNode(http_request message)
{
	g_Logger.WriteLog(Constants::VerbRegisterNode.c_str());

	std::wstring server = ServerHelper::FindParameter(message, _T("server"));
	std::wstring port = ServerHelper::FindParameter(message, _T("port"));
	std::wstring name = ServerHelper::FindParameter(message, _T("name"));

	if (MyServer::ExistsNode(server, port, name) == true)
	{
		g_Logger.WriteLog(_T("Node already registered !"));
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
		g_Logger.WriteLog(_T("Node registered !"));
		g_Guard.Done();
	}

	message.reply(status_codes::OK);
}

void MyServer::RequestVerbShowNodes(http_request message)
{
	g_Logger.WriteLog(Constants::VerbShowNodes.c_str());
	MyServer::ShowNodes();
	message.reply(status_codes::OK);
}

void MyServer::RequestVerbGetNode(http_request message)
{
	g_Logger.WriteLog(Constants::VerbGetNode.c_str());

	std::wstring dbname = ServerHelper::FindParameter(message, _T("dbname"));

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
		g_Logger.WriteLog(response.c_str());

		message.reply(status_codes::OK, data.AsJSON());

		MyServer::SendDbName(data);
	}
	else
	{
		g_Logger.WriteLog(_T("No node available..."));
		message.reply(status_codes::OK);
	}

	g_Guard.Done();
}

void MyServer::RequestVerbReleaseNode(http_request message)
{
	g_Logger.WriteLog(Constants::VerbReleaseNode.c_str());

	std::wstring dbname = ServerHelper::FindParameter(message, _T("dbname"));

	std::shared_ptr<WorkerNodeAttributes> pObj = nullptr;

	g_Guard.WaitToWrite();

	for (auto itr = _nodes.begin(); itr != _nodes.end(); itr++)
	{
		pObj = *itr;

		if (pObj->_dbName == dbname) // && pObj->_isActive == true
		{
			// We find an existing entry
			pObj->_isActive = false;
			pObj->_dbName = _T("");
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
		g_Logger.WriteLog(response.c_str());

		message.reply(status_codes::OK, data.AsJSON());

		MyServer::SendReleaseDbName(data);
	}
	else
	{
		g_Logger.WriteLog(_T("No node available..."));
		message.reply(status_codes::OK);
	}

	g_Guard.Done();
}

void MyServer::SendDbName(GetNodeData data)
{
	std::wstring url = ServerHelper::BuildURL(data.ip, data.port);
	std::wstring address = url;

	http_client client(address);

	std::wostringstream buf;
	buf << Constants::Request << Constants::VerbSetNode
		<< _T("&dbname=") << data.dbName;

	http_response response;

	response = client.request(methods::GET, buf.str()).get();
	g_Logger.WriteLog(response.to_string().c_str());

	json::value jdata = json::value::array();
	jdata = response.extract_json().get();

	if (jdata.is_null())
	{
		g_Logger.WriteLog(_T("no JSON data..."));
		return;
	}
}

void MyServer::SendReleaseDbName(GetNodeData data)
{
	std::wstring url = ServerHelper::BuildURL(data.ip, data.port);
	std::wstring address = url;

	http_client client(address);

	std::wostringstream buf;
	buf << Constants::Request << Constants::VerbReleaseDB
		<< _T("&dbname=") << data.dbName;

	http_response response;

	response = client.request(methods::GET, buf.str()).get();
	g_Logger.WriteLog(response.to_string().c_str());

	json::value jdata = json::value::array();
	jdata = response.extract_json().get();

	if (jdata.is_null())
	{
		g_Logger.WriteLog(_T("no JSON data..."));
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
		g_Logger.WriteLog(sz);
	}

	g_Guard.Done();
}



