#pragma once
#include "stdafx.h"


struct Data
{
	std::wstring key;
	std::wstring value;

	static Data FromJSON(const web::json::object & object)
	{
		Data result;
		result.key = object.at(_T("key")).as_string();
		result.value = object.at(_T("value")).as_string();
		return result;
	}

	web::json::value AsJSON() const
	{
		web::json::value result = web::json::value::object();
		result[_T("key")] = web::json::value::string(key);
		result[_T("value")] = web::json::value::string(value);
		return result;
	}
};

struct GetNodeData
{
	std::wstring ip;
	std::wstring port;
	std::wstring name;
	std::wstring dbName;

	static GetNodeData FromJSON(const web::json::object & object)
	{
		GetNodeData result;
		result.ip = object.at(_T("ip")).as_string();
		result.port = object.at(_T("port")).as_string();
		result.name = object.at(_T("name")).as_string();
		result.dbName = object.at(_T("dbName")).as_string();
		return result;
	}

	web::json::value AsJSON() const
	{
		web::json::value result = web::json::value::object();
		result[_T("ip")] = web::json::value::string(ip);
		result[_T("port")] = web::json::value::string(port);
		result[_T("name")] = web::json::value::string(name);
		result[_T("dbName")] = web::json::value::string(dbName);
		return result;
	}
};

struct PingData
{
	std::wstring ip;
	std::wstring port;
	std::wstring status;

	static PingData FromJSON(const web::json::object & object)
	{
		PingData result;
		result.ip = object.at(_T("ip")).as_string();
		result.port = object.at(_T("port")).as_string();
		result.status = object.at(_T("status")).as_string();
		return result;
	}

	web::json::value AsJSON() const
	{
		web::json::value result = web::json::value::object();
		result[_T("ip")] = web::json::value::string(ip);
		result[_T("port")] = web::json::value::string(port);
		result[_T("status")] = web::json::value::string(status);
		return result;
	}
};

struct UsageData
{
	std::wstring company;
	std::wstring version;
	std::wstring description;

	static UsageData FromJSON(const web::json::object & object)
	{
		UsageData result;
		result.company = object.at(_T("company")).as_string();
		result.version = object.at(_T("version")).as_string();
		result.description = object.at(_T("description")).as_string();
		return result;
	}

	web::json::value AsJSON() const
	{
		web::json::value result = web::json::value::object();
		result[_T("company")] = web::json::value::string(company);
		result[_T("version")] = web::json::value::string(version);
		result[_T("description")] = web::json::value::string(description);
		return result;
	}
};

