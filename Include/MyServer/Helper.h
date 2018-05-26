#pragma once

class CHelper
{
public:
	CHelper() {}
	~CHelper() {}

	static std::wstring GetIP()
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

	static std::wstring BuildURL(std::wstring server, std::wstring port)
	{
		TCHAR sz[255];
		_stprintf_s(sz, _T("http://%s:%s/MyServer/LMDB/"), server.c_str(), port.c_str());
		std::wstring address = sz;
		return address;
	}

	static std::wstring FindParameterInQuery(std::map<std::wstring, std::wstring>query, std::wstring var)
	{
		auto it = query.find(var);
		std::wstring value;
		if (it != query.end())
		{
			value = it->second;
			std::wcout << _T("Extracted param ") << var << _T(": ") << value << endl;
		}
		return value;
	}

};

