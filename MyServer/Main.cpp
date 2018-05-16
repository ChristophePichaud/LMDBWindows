#include "stdafx.h"
#include "MyServer.h"
#include "Helper.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

int wmain(int argc, wchar_t *argv[])
{
    std::wstring port = U("7001");
	std::wstring defaultAddress = _T("localhost");
	std::wstring mode = _T("master");
	std::wstring name = _T("");
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
	if (argc == 5)
	{
		defaultAddress = argv[1];
		port = argv[2];
		mode = argv[3];
		name = argv[4];
	}

	std::wstring ip = CHelper::GetIP();
	std::wcout << L"IP : " << ip << std::endl;

	TCHAR sz[255];
	_stprintf(sz, _T("http://%s:%s/MyServer/LMDB/"), ip.c_str(), port.c_str());
	http::uri uri = http::uri(sz);
	std::wstring address = uri.to_string();

	if (mode == _T("node"))
	{
		NodeClient client(address);
		client._server = ip;
		client._port = _tstoi(port.c_str());
		client._name = name;
		client.Init();
		std::wcout << L"Server node " << address << std::endl;
		std::wcout << _T("Press ENTER to exit.") << std::endl;
		std::string line;
		std::getline(std::cin, line);
		client.Close();
	}
	else
	{
		//
		// Create the server instance
		//

		MyServer server(address);
		server._server = ip;
		server._port = _tstoi(port.c_str());
		server.Init();
		std::wcout << L"Server node " << address << std::endl;
		std::wcout << _T("Press ENTER to exit.") << std::endl;
		std::string line;
		std::getline(std::cin, line);
		server.Close();
	}

	return 0;
}