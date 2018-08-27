#include "stdafx.h"
#include "..\Include\ServerHelper.h"

std::wstring ServerHelper::GetIP()
{
	//return L"127.0.0.1";

	// Init WinSock
	WSADATA wsa_Data;
	int wsa_ReturnCode = WSAStartup(MAKEWORD(2, 2), &wsa_Data);

	// Get the local hostname
	char szHostName[255];
	gethostname(szHostName, 255);

	struct hostent *remoteHost;
	struct in_addr addr;
	std::string ip;

	remoteHost = gethostbyname(szHostName);

	if (remoteHost->h_addrtype == AF_INET) 
	{
		int i = 0;
		while (remoteHost->h_addr_list[i] != 0) 
		{
			addr.s_addr = *(u_long *)remoteHost->h_addr_list[i++];
			printf("\tIPv4 Address #%d: %s\n", i, inet_ntoa(addr));
			ip = inet_ntoa(addr);
		}
	}

	WSACleanup();
	std::wstring ipw(ip.begin(), ip.end());
	return ipw;

	int iRetval;
	DWORD dwRetval;
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	struct sockaddr_in  *sockaddr_ipv4;
	//    struct sockaddr_in6 *sockaddr_ipv6;
	LPSOCKADDR sockaddr_ip;

	char ipstringbuffer[46];
	DWORD ipbufferlength = 46;
	//std::string ip;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	dwRetval = getaddrinfo(szHostName, NULL, &hints, &result);
	if (dwRetval != 0)
		return L"";

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) 
	{
		//printf("\tFlags: 0x%x\n", ptr->ai_flags);
		//printf("\tFamily: ");
		switch (ptr->ai_family) 
		{
		case AF_INET:
			//printf("AF_INET (IPv4)\n");
			sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
			//printf("\tIPv4 address %s\n", inet_ntoa(sockaddr_ipv4->sin_addr));
			ip = inet_ntoa(sockaddr_ipv4->sin_addr);
			//	goto fin;
			//break;
		}
	}

fin:

	freeaddrinfo(result);
	WSACleanup();

	std::wstring ipw2(ip.begin(), ip.end());
	return ipw2;
}

std::vector<std::wstring> ServerHelper::GetIPs()
{
	std::vector<std::wstring> v;

	// Init WinSock
	WSADATA wsa_Data;
	int wsa_ReturnCode = WSAStartup(MAKEWORD(2, 2), &wsa_Data);

	// Get the local hostname
	char szHostName[255];
	gethostname(szHostName, 255);

	int iRetval;
	DWORD dwRetval;
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	struct sockaddr_in  *sockaddr_ipv4;
	//    struct sockaddr_in6 *sockaddr_ipv6;
	LPSOCKADDR sockaddr_ip;

	char ipstringbuffer[46];
	DWORD ipbufferlength = 46;
	std::string ip;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	dwRetval = getaddrinfo(szHostName, NULL, &hints, &result);
	if (dwRetval != 0)
		return v;

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		switch (ptr->ai_family)
		{
		case AF_INET:
			sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
			ip = inet_ntoa(sockaddr_ipv4->sin_addr);
			std::wstring ipw(ip.begin(), ip.end());
			v.push_back(ipw);
			break;
			//goto fin;
			//break;
		}
	}

	freeaddrinfo(result);
	WSACleanup();

	return v;
}

std::wstring ServerHelper::GetHostName()
{
	std::wstring retval;
	char szHostName[255];
	gethostname(szHostName, 255);
	std::string hn = szHostName;
	std::wstring hnw(hn.begin(), hn.end());
	retval = hnw;
	return retval;
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
		//std::wcout << _T("Extracted param ") << var << _T(": ") << value << std::endl;
	}
	return value;
}

std::wstring ServerHelper::FindParameter(http_request message, std::wstring var)
{
	auto query = uri::split_query(uri::decode(message.relative_uri().query()));
	std::wstring value = ServerHelper::FindParameterInQuery(query, var);
	return value;
}


