#include "stdafx.h"
#include "MyServer.h"
#include "Helper.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

std::unique_ptr<MyServer> g_http;

int wmain(int argc, wchar_t *argv[])
{
    std::wstring port = U("7001");
	std::wstring defaultAddress = _T("localhost");
	std::wstring mode = _T("node");
	if(argc == 2)
    {
        port = argv[1];
    }
	if (argc == 3)
	{
		defaultAddress = argv[1];
		port = argv[2];
	}
	if (argc == 4)
	{
		defaultAddress = argv[1];
		port = argv[2];
		mode = argv[3];
	}

	std::wstring ip = CHelper::GetIP();
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

	NodeClient client(addr);
	client.Init();

	/*
	g_http = std::unique_ptr<MyServer>(new MyServer(addr));
	g_http->Init_LMDB();
	g_http->open().wait();
	*/

	std::wcout << _T("Listening for requests at: ") << addr << std::endl;
	std::wcout << _T("Press ENTER to exit.") << std::endl;
    std::string line;
    std::getline(std::cin, line);

	//g_http->close().wait();
	client.Close();
	return 0;
}