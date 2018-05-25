#include "stdafx.h"
#include "MyServer.h"
#include "..\Include\MyServer\Helper.h"
#include "WorkerNodeClient.h"
#include "..\Include\MyServer\Constants.h"

CSWMRGuard g_Guard;

int wmain(int argc, wchar_t *argv[])
{
	std::wstring port = _MASTER_NODE_PORT_;
	std::wstring defaultAddress = _T("localhost");
	std::wstring mode = _MODE_MASTER_NODE_;
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

	std::wstring url = CHelper::BuildURL(ip, port);
	http::uri uri = http::uri(url);
	std::wstring address = uri.to_string();						

	if (mode == _MODE_WORKER_NODE_) // _T("node"))
	{
		//
		// Create the a worker node instance
		//

		WorkerNodeClient client(address);
		client._server = ip;
		client._port = port;
		client._name = name;
		client.Init();
		std::wcout << L"Worker node " << address << std::endl;
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
		server._port = port;
		server.Init();
		std::wcout << L"Server node " << address << std::endl;
		std::wcout << _T("Press ENTER to exit.") << std::endl;
		std::string line;
		std::getline(std::cin, line);
		server.Close();
	}

	return 0;
}