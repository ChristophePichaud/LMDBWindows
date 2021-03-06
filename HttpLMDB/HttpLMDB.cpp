// HttpLMDB.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "HttpLMDB.h"

#include "..\Include\messagetypes.h"
#include "..\Include\ServerHelper.h"
#include "..\Include\Constants.h"

// This is an example of an exported variable
HTTPLMDB_API int nHttpLMDB=0;

// This is an example of an exported function.
HTTPLMDB_API int fnHttpLMDB(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see HttpLMDB.h for the class definition
CHttpLMDB::CHttpLMDB()
{
    return;
}

GetNodeData _data;

bool HTTPLMDB_API Init(std::wstring dbname)
{
#ifdef OLD_LIBRARY
	std::wstring ip = ServerHelper::GetIP();
	std::wstring port = Constants::MasterNodePort;

	std::wstring url = ServerHelper::BuildURL(ip, port);
	std::wcout << L"Client " << url << std::endl;

	http_client client(url);

	std::wostringstream buf;
	buf << Constants::Request << Constants::VerbGetNode
		<< _T("&dbname=") << dbname;

	http_response response;

	try
	{
		response = client.request(methods::GET, buf.str()).get();
		wcout << response.to_string() << endl;
	}
	catch (http_exception ex)
	{
		std::string err = ex.what();
	}	

	json::value jdata = json::value::array();
	jdata = response.extract_json().get();

	if (jdata.is_null())
	{
		std::wcout << _T("no JSON data...") << std::endl;
		return false;
	}

	_data = GetNodeData::FromJSON(jdata.as_object());						

	TCHAR sz[255];
	_stprintf_s(sz, _T("GetNodeData ip:%s port:%s name:%s dbname:%s\n"), _data.ip.c_str(), _data.port.c_str(), _data.name.c_str(), _data.dbName.c_str());
	_tprintf(sz);
#endif

	return true;
}

bool HTTPLMDB_API Uninit(std::wstring dbname)
{
#ifdef OLD_LIBRARY
	std::wstring ip = ServerHelper::GetIP();
	std::wstring port = Constants::MasterNodePort;

	std::wstring url = ServerHelper::BuildURL(ip, port);
	std::wcout << L"Client " << url << std::endl;

	http_client client(url);

	std::wostringstream buf;
	buf << Constants::Request << Constants::VerbReleaseNode
		<< _T("&dbname=") << dbname;

	http_response response;

	try
	{
		response = client.request(methods::GET, buf.str()).get();
		wcout << response.to_string() << endl;
	}
	catch (http_exception ex)
	{
		std::string err = ex.what();
	}

	json::value jdata = json::value::array();
	jdata = response.extract_json().get();

	if (jdata.is_null())
	{
		std::wcout << _T("no JSON data...") << std::endl;
		return false;
	}

	_data = GetNodeData::FromJSON(jdata.as_object());

	TCHAR sz[255];
	_stprintf_s(sz, _T("GetNodeData ip:%s port:%s name:%s dbname:%s\n"), _data.ip.c_str(), _data.port.c_str(), _data.name.c_str(), _data.dbName.c_str());
	_tprintf(sz);

	#endif

	return true;
}

bool HTTPLMDB_API SetDataB64(std::wstring key, std::wstring valueb64, DWORD dwLen, std::wstring name)
{
	std::wstring port = Constants::MasterNodePort;
	std::wstring ip = ServerHelper::GetIP();
	std::wstring url = ServerHelper::BuildURL(ip, port);

	std::wstring contentType = _T("Content-Type");
	std::wstring contentTypeV = _T("application/json");
	std::wstring keepAlive = _T("Keep-Alive");
	std::wstring keepAliveV = _T("false");
	std::wstring contentLength = _T("Content-Length");
	
	std::wostringstream bufLen;
	bufLen << contentType.length() + contentTypeV.length() + keepAlive.length() + keepAliveV.length() + contentLength.length() + 4;
	std::wstring len = bufLen.str().c_str();

	std::wostringstream buf;
	buf << url << Constants::Request << Constants::VerbSetDataB64
		<< _T("&key=") << key
		<< _T("&name=") << name;
	url = buf.str().c_str();	
	//{"key":"key_toto0","value":"value_toto0"}
	std::wostringstream bufjson;
	bufjson << "{" << '"' << "key" << '"' << ":" << '"' << key << '"' << ","
		<< '"' << "value" << '"' << ":" << '"' << valueb64 << '"' << '}';
	std::wstring jsonv = bufjson.str().c_str();
	//wcout << _T("jsonv : ") << jsonv << endl;

	http_client client_lmdb(url);
	http_request request(methods::POST);
	request.headers().add(contentType, contentTypeV);
	request.headers().add(keepAlive, keepAliveV);
	request.headers().add(contentLength, len);
	request.set_body(jsonv);

	http_response response;
	response = client_lmdb.request(request).get();

	//wcout << response.to_string() << endl;

	return true;
}

bool HTTPLMDB_API SetData(std::wstring key, std::wstring value, std::wstring name)
{
	std::wstring port = Constants::MasterNodePort;
	std::wstring ip = ServerHelper::GetIP();
	std::wstring url = ServerHelper::BuildURL(ip, port);
	http_client client_lmdb(url);

	std::wostringstream buf;
	buf << Constants::Request << Constants::VerbSetData
		<< _T("&key=") << key
		<< _T("&value=") << value
		<< _T("&name=") << name;

	http_response response;
	response = client_lmdb.request(methods::GET, buf.str()).get();

	//wcout << response.to_string() << endl;

	return true;
}

bool HTTPLMDB_API GetData(std::wstring key, std::wstring & value, std::wstring name)
{
	std::wstring port = Constants::MasterNodePort;
	std::wstring ip = ServerHelper::GetIP();
	std::wstring url = ServerHelper::BuildURL(ip, port);
	http_client client_lmdb(url);

	std::wostringstream buf;
	buf << Constants::Request << Constants::VerbGetData
		<< _T("&key=") << key
		<< _T("&name=") << name;

	wcout << buf.str() << endl;

	http_response response;
	response = client_lmdb.request(methods::GET, buf.str()).get();

	//wcout << response.to_string() << endl;

	json::value jdata = json::value::array();
	jdata = response.extract_json().get();
	if (jdata.is_null())
	{
		_tprintf(_T("jdata JSON is null\n"));
		return true;
	}

	Data data = Data::FromJSON(jdata.as_object());

	TCHAR sz[255];
	_stprintf_s(sz, _T("Data key:%s value:%s\n"), data.key.c_str(), data.value.c_str());
	_tprintf(sz);

	value = data.value;

	return true;
}

bool HTTPLMDB_API GetDataB64(std::wstring key, std::wstring & value, std::wstring name)
{
	std::wstring port = Constants::MasterNodePort;
	std::wstring ip = ServerHelper::GetIP();
	std::wstring url = ServerHelper::BuildURL(ip, port);
	http_client client_lmdb(url);

	std::wostringstream buf;
	buf << Constants::Request << Constants::VerbGetDataB64
		<< _T("&key=") << key
		<< _T("&name=") << name;

	wcout << buf.str() << endl;

	http_response response;
	response = client_lmdb.request(methods::POST, buf.str()).get();

	//wcout << response.to_string() << endl;

	json::value jdata = json::value::array();
	jdata = response.extract_json().get();
	if (jdata.is_null())
	{
		_tprintf(_T("jdata JSON is null\n"));
		return true;
	}

	Data data = Data::FromJSON(jdata.as_object());

	//TCHAR sz[255];
	//_stprintf_s(sz, _T("Data key:%s value:%s\n"), data.key.c_str(), data.value.c_str());
	//_tprintf(sz);

	value = data.value;

	return true;
}
