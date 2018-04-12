#include "stdafx.h"
#include "MyServer.h"
#include "DataHandler.h"
#include "WorkerItemHandler.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

std::unique_ptr<MyServer> g_http;


CString GetIP()
{
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

	CString ip(szLocalIP);
	return ip;
}

int wmain(int argc, wchar_t *argv[])
{
    utility::string_t port = U("7001");
	std::wstring defaultAddress = _T("localhost");
	if(argc == 2)
    {
        port = argv[1];
    }
	if (argc == 3)
	{
		defaultAddress = argv[1];
		port = argv[2];
	}

	//CoInitializeEx(0, COINIT_MULTITHREADED);
	//WorkerItemHandler wih;
	//CString strIP = wih.GetNetworkAdapterInformation2();
	//std::wstring ip = (LPCTSTR)strIP;
	//std::wcout << L"IP : " << ip << std::endl;

	CString strIP = GetIP();
	std::wstring ip = (LPCTSTR)strIP;
	std::wcout << L"IP : " << ip << std::endl;

	std::wstring address = _T("http://");
	address.append(ip); // defaultAddress); //ip
	address.append(_T(":"));
	address.append(port);
	address.append(_T("/MyServer/LMDB/"));
	http::uri uri = http::uri(address);
	auto addr = uri.to_string();

	//
	// Create the server instance
	//

	std::wcout << L"Server " << addr << std::endl;

	g_http = std::unique_ptr<MyServer>(new MyServer(addr));
	g_http->Init_LMDB();
	g_http->open().wait();

	ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;
	std::cout << "Press ENTER to exit." << std::endl;
    std::string line;
    std::getline(std::cin, line);

	g_http->close().wait();
	return 0;
}