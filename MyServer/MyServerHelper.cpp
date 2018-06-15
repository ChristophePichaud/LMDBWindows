#include "stdafx.h"
#include "MyServerHelper.h"


MyServerHelper::MyServerHelper()
{
}


MyServerHelper::~MyServerHelper()
{
}

void MyServerHelper::PrintRequest(http_request message)
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
