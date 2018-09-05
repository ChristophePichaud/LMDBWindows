#include "stdafx.h"
#include "TheServer.h"
#include "..\Include\messagetypes.h"
#include "..\Include\ServerHelper.h"
#include "..\Include\Constants.h"
#include "..\Include\Base64Helper.h"


std::wstring TheServer::_server;
std::wstring  TheServer::_port;
std::wstring TheServer::_name;
std::map<std::wstring, std::shared_ptr<CLMDBWrapperEx>> TheServer::_mapWrapper;

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
	data.version = _T("July 2018 BETA 0.2");
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
		g_Logger.WriteLog(request);

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
	g_Logger.WriteLog(Constants::VerbPing);

	PingData data;
	data.ip = _server;
	data.port = _port;
	data.status = _T("OK");

	std::wstring response = data.AsJSON().serialize();
	g_Logger.WriteLog(response);

	message.reply(status_codes::OK, data.AsJSON());
}

std::shared_ptr<CLMDBWrapperEx> TheServer::GetLMDBWrapper(std::wstring dbName)
{
	std::shared_ptr<CLMDBWrapperEx> wr;
	auto it = _mapWrapper.find(dbName);
	if (it == _mapWrapper.end())
	{
		wr = std::make_shared<CLMDBWrapperEx>();
		wr->Init(dbName);

		_mapWrapper[dbName] = wr;

		TCHAR sz[255];
		_stprintf_s(sz, _T("LMDB Init DB %s"), dbName.c_str());
		g_Logger.WriteLog(sz);
	}
	else
	{
		wr = it->second;

		TCHAR sz[255];
		_stprintf_s(sz, _T("Reuse DB %s"), dbName.c_str());
		g_Logger.WriteLog(sz);
	}
	return wr;
}

void TheServer::RequestVerbGetData(http_request message)
{
	g_Logger.WriteLog(Constants::VerbGetData);

	std::wstring key = ServerHelper::FindParameter(message, _T("key"));
	std::wstring dbNameW = ServerHelper::FindParameter(message, _T("name"));

	std::shared_ptr<CLMDBWrapperEx> lmdb = GetLMDBWrapper(dbNameW);
	std::wstring value;

	if (lmdb->Get(key, value) == true)
	{
		Data data;
		data.key = key;
		data.value = value;

		std::wostringstream buf;
		buf << _T("Get key:") << key << _T(" value:") << value;
		g_Logger.WriteLog(buf.str());

		std::wstring response = data.AsJSON().serialize();
		g_Logger.WriteLog(response);

		message.reply(status_codes::OK, data.AsJSON());
	}
	else
	{
		message.reply(status_codes::OK);
	}
}

void TheServer::RequestVerbGetData64(http_request message)
{
	g_Logger.WriteLog(Constants::VerbGetDataB64);

	std::wstring key = ServerHelper::FindParameter(message, _T("key"));
	std::wstring dbNameW = ServerHelper::FindParameter(message, _T("name"));

	std::shared_ptr<CLMDBWrapperEx> lmdb = GetLMDBWrapper(dbNameW);
	std::wstring value;

	if (lmdb->Get(key, value) == true)
	{
		std::string keya(key.begin(), key.end());

		Data data;
		data.key = key;
		data.value = value;

		std::wostringstream buf;
		buf << _T("GetB64 key:") << key << _T(" value:") << value;
		//g_Logger.WriteLog(buf.str());

		std::string buffera64(data.value.begin(), data.value.end());
		string buffera = Base64Helper::base64_decode(buffera64);

		//std::wstring response = data.AsJSON().serialize();
		//g_Logger.WriteLog(response);
		//std::wstring buffer(buffera.begin(), buffera.end());

		TCHAR sz[255];
		_stprintf_s(sz, _T("attachment; filename=%c%s%c"), '"', key.c_str(), '"');
		g_Logger.WriteLog(sz);

		message.headers().add(L"Content-Disposition", sz);	
		//message.headers().add(L"Content-Length", std::to_wstring(buffera.length()));
		message.reply(status_codes::OK, buffera, "application/octet-stream");
	}
	else
	{
		message.reply(status_codes::OK);
	}
}

void TheServer::RequestVerbSetData(http_request message)
{
	g_Logger.WriteLog(Constants::VerbSetData);

	std::wstring key = ServerHelper::FindParameter(message, _T("key"));
	std::wstring value = ServerHelper::FindParameter(message, _T("value"));
	std::wstring dbNameW = ServerHelper::FindParameter(message, _T("name"));

	std::shared_ptr<CLMDBWrapperEx> lmdb = GetLMDBWrapper(dbNameW);

	lmdb->Set(key, value);

	std::wostringstream buf;
	buf << _T("Set key:") << key << _T(" value:") << value;
	g_Logger.WriteLog(buf.str());

	Data data;
	data.key = key;
	data.value = value;

	std::wstring response = data.AsJSON().serialize();
	g_Logger.WriteLog(response);

	message.reply(status_codes::OK, data.AsJSON());
}

void TheServer::RequestVerbSetData64(http_request message)
{
	g_Logger.WriteLog(Constants::VerbSetDataB64);

	std::wstring key = ServerHelper::FindParameter(message, _T("key"));
	std::wstring dbNameW = ServerHelper::FindParameter(message, _T("name"));

	std::shared_ptr<CLMDBWrapperEx> lmdb = GetLMDBWrapper(dbNameW);

	std::wstring json;
	web::json::value jsonV = message.extract_json().get();
	g_Logger.WriteLog(jsonV.serialize());

	Data data = Data::FromJSON(jsonV.as_object());

	//std::string value(data.value.begin(), data.value.end());
	//std::string valueb64 = Base64Helper::base64_encode((const unsigned char *)value.c_str(), value.length());
	//std::wstring valuew(valueb64.begin(), valueb64.end());
	//lmdb->Set(data.key, valuew);

	lmdb->Set(data.key, data.value);

	std::wostringstream buf;
	//buf << _T("Set key:") << key << _T(" value:") << valuew;
	//g_Logger.WriteLog(buf.str());

	message.reply(status_codes::OK);
}

void TheServer::PrintRequest(http_request message)
{
	return;

	g_Logger.WriteLog(message.to_string());
	g_Logger.WriteLog(message.relative_uri().to_string());

	auto paths = uri::split_path(uri::decode(message.relative_uri().path()));
	for (auto it1 = paths.begin(); it1 != paths.end(); it1++)
	{
		std::wstring path = *it1;
		g_Logger.WriteLog(path);
	}

	auto query = uri::split_query(uri::decode(message.relative_uri().query()));
	for (auto it2 = query.begin(); it2 != query.end(); it2++)
	{
		std::wostringstream buf;
		buf << _T("Query ") << it2->first << _T(" ") << it2->second;
		g_Logger.WriteLog(buf.str());
	}
}
