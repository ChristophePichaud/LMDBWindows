#include "stdafx.h"
#include "TheServer.h"
#include "..\Include\messagetypes.h"
#include "..\Include\ServerHelper.h"
#include "..\Include\Constants.h"
#include "..\Include\Base64Helper.h"


std::wstring TheServer::_server;
std::wstring  TheServer::_port;
std::wstring TheServer::_name;

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TheServer class
//
////////////////////////////////////////////////////////////////////////////////////////////////////


TheServer::TheServer(std::wstring url) : m_listener(url)
{
	std::function<void(http_request)> fnGet = &TheServer::handle_get;
	m_listener.support(methods::GET, fnGet);
	std::function<void(http_request)> fnPost = &TheServer::handle_post;
	m_listener.support(methods::POST, fnPost);
	std::function<void(http_request)> fnDel = &TheServer::handle_del;
	m_listener.support(methods::DEL, fnDel);
	std::function<void(http_request)> fnPut = &TheServer::handle_put;
	m_listener.support(methods::PUT, fnPut);

	this->_url = url;
}

TheServer::~TheServer()
{
}

void TheServer::Init()
{
	this->_http = std::unique_ptr<TheServer>(new TheServer(this->_url));
	this->_http->open().wait();
}

void TheServer::Close()
{
	this->_http->close().wait();
}

void TheServer::handle_get(http_request message)
{
	g_Logger.WriteLog(_T("handle_get"));

	PrintRequest(message);

	// http://192.168.175.241:7001/MyServer/LMDB/?request=set-data&key=toto0&value=toto1&name=cache2
	// http://192.168.175.241:7001/MyServer/LMDB/?request=get-data&key=toto2_k&name=cache3

	std::wstring request = ServerHelper::FindParameter(message, _T("request"));

	if (request == Constants::VerbPing)
	{
		RequestVerbPing(message);
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

void TheServer::handle_post(http_request message)
{
	g_Logger.WriteLog(message.to_string().c_str());

	PrintRequest(message);

	std::wstring request = ServerHelper::FindParameter(message, _T("request"));

	if (request == Constants::VerbSetDataB64)
	{
		RequestVerbSetData2(message);
		return;
	}

	if (request == Constants::VerbGetDataB64)
	{
		RequestVerbGetData2(message);
		return;
	}

	message.reply(status_codes::OK);
};

void TheServer::handle_del(http_request message)
{
}

void TheServer::handle_put(http_request message)
{
}


void TheServer::RequestVerbPing(http_request message)
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

void TheServer::RequestVerbGetData(http_request message)
{
	USES_CONVERSION;
	CLMDBWrapper lmdb;

	g_Logger.WriteLog(Constants::VerbGetData.c_str());

	std::wstring key = ServerHelper::FindParameter(message, _T("key"));
	std::wstring dbNameW = ServerHelper::FindParameter(message, _T("name"));
	std::string dbName(dbNameW.begin(), dbNameW.end());

	if (lmdb.Init((LPSTR)dbName.c_str()) == false)
	{
		g_Logger.WriteLog(_T("LMDB Init not done !"));
		message.reply(status_codes::OK);
		return;
	}

	char szKey[255];
	LPSTR lpszValue;

	strcpy_s(szKey, W2A(key.c_str()));

	if (lmdb.GetData((LPSTR)szKey, &lpszValue) == true)
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

void TheServer::RequestVerbGetData2(http_request message)
{
	USES_CONVERSION;
	CLMDBWrapper lmdb;

	g_Logger.WriteLog(Constants::VerbGetData.c_str());

	std::wstring key = ServerHelper::FindParameter(message, _T("key"));
	std::wstring dbNameW = ServerHelper::FindParameter(message, _T("name"));
	std::string dbName(dbNameW.begin(), dbNameW.end());

	if (lmdb.Init((LPSTR)dbName.c_str()) == false)
	{
		g_Logger.WriteLog(_T("LMDB Init not done !"));
		message.reply(status_codes::OK);
		return;
	}

	char szKey[255];
	LPSTR lpszValue;

	strcpy_s(szKey, W2A(key.c_str()));

	if (lmdb.GetData((LPSTR)szKey, &lpszValue) == true)
	{
		Data data;
		data.key = key;

		std::string valueb64 = lpszValue;
		std::string value = Base64Helper::base64_decode(valueb64.c_str());

		data.value = std::wstring((A2W(value.c_str())));

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

void TheServer::RequestVerbSetData(http_request message)
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

void TheServer::RequestVerbSetData2(http_request message)
{
	USES_CONVERSION;
	CLMDBWrapper lmdb;
	g_Logger.WriteLog(Constants::VerbSetData.c_str());

	std::wstring key = ServerHelper::FindParameter(message, _T("key"));
	std::wstring value = ServerHelper::FindParameter(message, _T("value"));
	std::wstring len = ServerHelper::FindParameter(message, _T("len"));
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

void TheServer::PrintRequest(http_request message)
{
	TCHAR sz[255];
	g_Logger.WriteLog(message.to_string().c_str());
	g_Logger.WriteLog(message.relative_uri().to_string().c_str());

	auto paths = uri::split_path(uri::decode(message.relative_uri().path()));
	for (auto it1 = paths.begin(); it1 != paths.end(); it1++)
	{
		std::wstring path = *it1;
		g_Logger.WriteLog(path.c_str());
	}

	auto query = uri::split_query(uri::decode(message.relative_uri().query()));
	for (auto it2 = query.begin(); it2 != query.end(); it2++)
	{
		_stprintf_s(sz, _T("Query %s %s"), it2->first.c_str(), it2->second.c_str());
		g_Logger.WriteLog(sz);
	}
}