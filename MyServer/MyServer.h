#pragma once

#include "LMDBData.h"
#include "MyServer\\messagetypes.h"
#include "WorkerNodeAttributes.h"

class MyServer
{
public:
	MyServer(std::wstring url);

public:
	void Init();
	void Close();
	void ActivatePingThread();
	static void PingThread(LPVOID param);

	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

public:
	static bool ExistsNode(std::wstring server, std::wstring port, std::wstring name);
	static void ShowNodes();
	static void SendDbName(GetNodeData data);
	static void SendReleaseDbName(GetNodeData data);

private:
	static void handle_get(http_request message);
	static void handle_put(http_request message);
	static void handle_post(http_request message);
	static void handle_delete(http_request message);

	http_listener m_listener;
	static std::vector<std::shared_ptr<WorkerNodeAttributes>> _nodes;

public:
	std::wstring _server;
	std::wstring _port;

public:
	std::unique_ptr<MyServer> _http;
	std::wstring _url;
};


