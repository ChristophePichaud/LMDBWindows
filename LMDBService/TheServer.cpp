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
	try
	{
		g_Logger.WriteLog(_T("handle_get"));

		PrintRequest(message);

		//http://192.168.175.241:7001/MyServer/LMDB/?request=set-data&key=toto0&value=toto1&name=cache2
		//http://192.168.175.241:7001/MyServer/LMDB/?request=get-data&key=toto2_k&name=cache3

		std::wstring request = ServerHelper::FindParameter(message, _T("request"));

		if (request == Constants::VerbPing)
		{
			RequestVerbPing(message);
			return;
		}
		else if (request == Constants::VerbGetData)
		{
			RequestVerbGetData(message);
			return;
		}
		else if (request == Constants::VerbSetData)
		{
			RequestVerbSetData(message);
			return;
		}
		else if (request == Constants::VerbGetDataB64)
		{
			RequestVerbGetData64(message);
			return;
		}
		else
		{
			RequestUsage(message);
			return;
		}
	}
	catch (...)
	{
		// an internal problem occured
		g_Logger.WriteLog(_T("handle_get exception..."));
	}

	message.reply(status_codes::OK);
}

void TheServer::RequestUsage(http_request message)
{
	UsageData data;
	data.company = _T("NEOS-SDI France");
	data.version = _T("July 2018 BETA 0.1");
	data.description = _T("Built by Christophe Pichaud");
	//std::wstring response = data.AsJSON().serialize();
	message.reply(status_codes::OK, data.AsJSON());
}

void TheServer::handle_post(http_request message)
{
	try
	{
		g_Logger.WriteLog(_T("handle_post"));

		PrintRequest(message);

		std::wstring request = ServerHelper::FindParameter(message, _T("request"));
		g_Logger.WriteLog(request.c_str());

		if (request == Constants::VerbSetDataB64)
		{
			RequestVerbSetData64(message);
			return;
		}
		else if (request == Constants::VerbGetDataB64)
		{
			RequestVerbGetData64(message);
			return;
		}
	}
	catch (...)
	{
		// an internal problem occured
		g_Logger.WriteLog(_T("handle_post exception..."));
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

	LPSTR lpszValue;
	std::string akey(key.begin(), key.end());

	if (lmdb.Get((LPSTR)akey.c_str(), &lpszValue) == true)
	{
		Data data;
		data.key = key;

		int len = strlen(lpszValue);
		if (len == 0)
			data.value = _T("");
		else
		{
			std::string value = std::string(lpszValue);
			std::wstring wvalue(value.begin(), value.end());
			data.value = wvalue;
		}

		free(lpszValue);

		TCHAR sz[10240];
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

	lmdb.Uninit();
}

void TheServer::RequestVerbGetData64(http_request message)
{
	CLMDBWrapper lmdb;

	g_Logger.WriteLog(Constants::VerbGetDataB64.c_str());

	std::wstring key = ServerHelper::FindParameter(message, _T("key"));
	std::wstring dbNameW = ServerHelper::FindParameter(message, _T("name"));
	std::string dbName(dbNameW.begin(), dbNameW.end());

	if (lmdb.Init((LPSTR)dbName.c_str()) == false)
	{
		g_Logger.WriteLog(_T("LMDB Init not done !"));
		message.reply(status_codes::OK);
		return;
	}

	std::string akey(key.begin(), key.end());
	LPSTR lpszValue;

	if (lmdb.Get((LPSTR)akey.c_str(), &lpszValue) == true)
	{
		Data data;
		data.key = key;

		std::string valueb64 = std::string(lpszValue);
		std::string value = Base64Helper::base64_decode(valueb64.c_str());
		std::wstring wvalue(value.begin(), value.end());
		data.value = wvalue;

		free(lpszValue);

		TCHAR sz[255];
		_stprintf_s(sz, _T("Get Key:%s Value:..."), data.key.c_str());
		g_Logger.WriteLog(sz);
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

	lmdb.Uninit();
}

void TheServer::RequestVerbSetData(http_request message)
{
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

	std::string akey(key.begin(), key.end());
	std::string avalue(value.begin(), value.end());
	lmdb.Set((LPSTR)akey.c_str(), (LPSTR)avalue.c_str());

	TCHAR sz[10240];
	_stprintf_s(sz, _T("Set Key:%s Value:%s"), key.c_str(), value.c_str());
	g_Logger.WriteLog(sz);

	Data data;
	data.key = key;
	data.value = value;

	std::wstring response = data.AsJSON().serialize();
	//g_Logger.WriteLog(response.c_str());

	message.reply(status_codes::OK, data.AsJSON());

	lmdb.Uninit();
}

void TheServer::RequestVerbSetData64(http_request message)
{
	CLMDBWrapper lmdb;
	g_Logger.WriteLog(Constants::VerbSetDataB64.c_str());

	std::wstring key = ServerHelper::FindParameter(message, _T("key"));
	std::wstring dbNameW = ServerHelper::FindParameter(message, _T("name"));
	std::string dbName(dbNameW.begin(), dbNameW.end());

	std::wstring json;
	web::json::value jsonV = message.extract_json().get();

	Data data = Data::FromJSON(jsonV.as_object());
	TCHAR sz[255];
	_stprintf_s(sz, _T("Data key:%s value:..."), data.key.c_str());
	g_Logger.WriteLog(sz);

	if (lmdb.Init((LPSTR)dbName.c_str()) == false)
	{
		g_Logger.WriteLog(_T("LMDB Init not done !"));
		message.reply(status_codes::OK);
		return;
	}

	std::string akey(key.begin(), key.end());
	std::string value(data.value.begin(), data.value.end());

	std::string valueb64 = Base64Helper::base64_encode((const unsigned char *)value.c_str(), value.length());

	LPSTR lpszKey = (LPSTR)akey.c_str();
	LPSTR lpszValue = (LPSTR)valueb64.c_str();
	DWORD dwLen = strlen(lpszValue);
	lmdb.Set(lpszKey, lpszValue, dwLen);

	message.reply(status_codes::OK);

	lmdb.Uninit();
}

void TheServer::PrintRequest(http_request message)
{
	//return;

	TCHAR sz[10240];
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