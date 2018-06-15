#include "stdafx.h"
#include "..\Include\ServerHelper.h"

std::wstring ServerHelper::GetIP()
{
	USES_CONVERSION;

	// Init WinSock
	WSADATA wsa_Data;
	int wsa_ReturnCode = WSAStartup(0x101, &wsa_Data);

	// Get the local hostname
	char szHostName[255];
	gethostname(szHostName, 255);
	struct hostent *host_entry;
	host_entry = gethostbyname(szHostName);
	char * szLocalIP;
	szLocalIP = inet_ntoa(*(struct in_addr *)*host_entry->h_addr_list);
	WSACleanup();

	std::wstring ip = A2W(szLocalIP);
	return ip;
}

std::wstring ServerHelper::BuildURL(std::wstring server, std::wstring port)
{
	TCHAR sz[255];
	_stprintf_s(sz, _T("http://%s:%s/MyServer/LMDB/"), server.c_str(), port.c_str());
	std::wstring address = sz;
	return address;
}

std::wstring ServerHelper::FindParameterInQuery(std::map<std::wstring, std::wstring>query, std::wstring var)
{
	auto it = query.find(var);
	std::wstring value;
	if (it != query.end())
	{
		value = it->second;
		std::wcout << _T("Extracted param ") << var << _T(": ") << value << std::endl;
	}
	return value;
}

std::wstring ServerHelper::FindParameter(http_request message, std::wstring var)
{
	auto query = uri::split_query(uri::decode(message.relative_uri().query()));
	std::wstring value = ServerHelper::FindParameterInQuery(query, var);
	return value;
}


