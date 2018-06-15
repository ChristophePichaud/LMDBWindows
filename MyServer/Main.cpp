#include "stdafx.h"
#include "MyServer.h"
#include "..\Include\ServerHelper.h"
#include "WorkerNodeClient.h"
#include "..\Include\Constants.h"

CSWMRGuard g_Guard;
CLogger g_Logger;

int wmain(int argc, wchar_t *argv[])
{
	std::wstring port = Constants::MasterNodePort;
	std::wstring defaultAddress = _T("localhost");
	std::wstring mode = Constants::ModeMasterNode;
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

	std::wstring ip = ServerHelper::GetIP();
	std::wstring url = ServerHelper::BuildURL(ip, port);
	http::uri uri = http::uri(url);
	std::wstring address = uri.to_string();						

	if (mode == Constants::ModeWorkerNode)
	{
		TCHAR sz[255];
		_stprintf_s(sz, _T("Node_%s_%s.log"), port.c_str(), name.c_str());
		g_Logger.Init(sz);
		g_Logger.WriteLog(_T("Init Node..."));

		//
		// Create the a worker node instance
		//
		
		_stprintf_s(sz, _T("IP : %s"), ip.c_str());
		g_Logger.WriteLog(sz);

		WorkerNodeClient client(address);
		client._server = ip;
		client._port = port;
		client._name = name;
		client.Init();
		_stprintf_s(sz, _T("Worker node %s"), address.c_str());
		g_Logger.WriteLog(sz);
		g_Logger.WriteLog(_T("Press ENTER to exit."));
		std::string line;
		std::getline(std::cin, line);
		client.Close();
	}
	else
	{
		TCHAR sz[255];
		_stprintf_s(sz, _T("Node_%s_MASTER.log"), port.c_str());
		g_Logger.Init(sz);
		g_Logger.WriteLog(_T("Init Master..."));

		//
		// Create the server instance
		//

		_stprintf_s(sz, _T("IP : %s"), ip.c_str());
		g_Logger.WriteLog(sz);

		MyServer server(address);
		server._server = ip;
		server._port = port;
		server.Init();
		_stprintf_s(sz, _T("Worker node %s"), address.c_str());
		g_Logger.WriteLog(sz);
		g_Logger.WriteLog(_T("Press ENTER to exit."));
		std::string line;
		std::getline(std::cin, line);
		server.Close();
	}

	return 0;
}