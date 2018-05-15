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
};

