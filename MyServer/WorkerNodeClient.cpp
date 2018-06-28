#include "stdafx.h"
#include "WorkerNodeClient.h"
#include "LMDBData.h"
#include "..\Include\messagetypes.h"
#include "..\Include\ServerHelper.h"
#include "..\Include\Constants.h"
#include "MyServerHelper.h"


//LMDBData WorkerNodeClient::m_lmdb;

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
	std::wstring ip = ServerHelper::GetIP();
	std::wstring port = Constants::MasterNodePort;
	std::wstring url = ServerHelper::BuildURL(ip, port);

	std::wostringstream buf;
	buf << Constants::Request << Constants::VerbRegisterNode
		<< _T("&server=") << this->_server
		<< _T("&port=") << this->_port
		<< _T("&name=") << this->_name;
	g_Logger.WriteLog(buf.str().c_str());

	http_response response;
	http_client client(url);
	response = client.request(methods::GET, buf.str()).get();
	g_Logger.WriteLog(response.to_string().c_str());

	return true;
}

void WorkerNodeClient::handle_get(http_request message)
{
	g_Logger.WriteLog(_T("handle_get"));

	MyServerHelper::PrintRequest(message);

	std::wstring request = ServerHelper::FindParameter(message, _T("request"));

	if (request == Constants::VerbPing)
	{
		RequestVerbPing(message);
		return;
	}

	if (request == Constants::VerbSetNode)
	{
		RequestVerbSetNode(message);
		return;
	}

	if (request == Constants::VerbReleaseDB)
	{
		RequestVerbReleaseDB(message);
		return;
	}

	if (request == Constants::VerbGetData)
	{
		RequestVerbGetData(message);
		return;
	}

	if (request == Constants::VerbSetData)
	{
		RequestVerbSetData(message);
		return;
	}

	message.reply(status_codes::OK);
}

void WorkerNodeClient::RequestVerbPing(http_request message)
{
	g_Logger.WriteLog(Constants::VerbPing.c_str());

	PingData data;
	data.ip = _server;
	data.port = _port;
	data.status = _T("OK");

	std::wstring response = data.AsJSON().serialize();
	g_Logger.WriteLog(response.c_str());

	message.reply(status_codes::OK, data.AsJSON());

}

void WorkerNodeClient::RequestVerbSetNode(http_request message)
{

	g_Logger.WriteLog(Constants::VerbSetNode.c_str());

	std::wstring dbname = ServerHelper::FindParameter(message, _T("dbname"));

	_dbName = dbname;
	message.reply(status_codes::OK);

	Init_LMDB();
}

void WorkerNodeClient::RequestVerbReleaseDB(http_request message)
{
	g_Logger.WriteLog(Constants::VerbReleaseDB.c_str());

	std::wstring dbname = ServerHelper::FindParameter(message, _T("dbname"));

	_dbName = dbname;
	message.reply(status_codes::OK);

	Uninit_LMDB();
}

void WorkerNodeClient::RequestVerbGetData(http_request message)
{
	USES_CONVERSION;
	CLMDBWrapper lmdb;

	g_Logger.WriteLog(Constants::VerbGetData.c_str());

	std::wstring key = ServerHelper::FindParameter(message, _T("key"));
	std::wstring dbNameW = ServerHelper::FindParameter(message, _T("name"));
	std::string dbName(dbNameW.begin(), dbNameW.end());

	if (lmdb.Init((LPSTR) dbName.c_str()) == false)
	{
		g_Logger.WriteLog(_T("LMDB Init not done !"));
		message.reply(status_codes::OK);
		return;
	}

	char szKey[255];
	LPSTR lpszValue;

	strcpy(szKey, W2A(key.c_str()));

	if (lmdb.GetData((LPSTR) szKey, &lpszValue) == true )
	{
		Data data;
		data.key = key;
		data.value = std::wstring((A2W(lpszValue)));

		free(lpszValue);
		
		TCHAR sz[255];
		_stprintf_s(sz, _T("Get Key:%s Value:%s"), data.key.c_str(), data.value.c_str());
		g_Logger.WriteLog(sz);

		std::wstring response = data.AsJSON().serialize();
		g_Logger.WriteLog(response.c_str());

		message.reply(status_codes::OK, data.AsJSON());
	}
	else
	{
		message.reply(status_codes::OK);
	}

	lmdb.Uninit((LPSTR)dbName.c_str());
}

void WorkerNodeClient::RequestVerbSetData(http_request message)
{
	USES_CONVERSION;
	CLMDBWrapper lmdb;
	g_Logger.WriteLog(Constants::VerbSetData.c_str());

	std::wstring key = ServerHelper::FindParameter(message, _T("key"));
	std::wstring value = ServerHelper::FindParameter(message, _T("value"));
	std::wstring dbNameW = ServerHelper::FindParameter(message, _T("name"));
	std::string dbName(dbNameW.begin(), dbNameW.end());

	if (lmdb.Init((LPSTR)dbName.c_str()) == false)
	{
		g_Logger.WriteLog(_T("LMDB Init not done !"));
		message.reply(status_codes::OK);
		return;
	}

	LPSTR lpszKey = W2A(key.c_str());
	LPSTR lpszValue = W2A(value.c_str());
	lmdb.SetData(lpszKey, lpszValue);

	char sz[255];
	sprintf_s(sz, "Set Key:%s Value:%s", lpszKey, lpszValue);
 	g_Logger.WriteLog(A2W(sz));

	Data data;
	data.key = A2W(lpszKey);
	data.value = A2W(lpszValue);

	std::wstring response = data.AsJSON().serialize();
	g_Logger.WriteLog(response.c_str());

	message.reply(status_codes::OK, data.AsJSON());

	lmdb.Uninit((LPSTR)dbName.c_str());
}

void WorkerNodeClient::Init_LMDB()
{
}

void WorkerNodeClient::Uninit_LMDB()
{
}
