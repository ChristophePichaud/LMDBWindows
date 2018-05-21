#pragma once

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

#include "LMDBData.h"
#include "MyServer\\messagetypes.h"

class NodeAttributes
{
public:
	NodeAttributes() {}

	bool _isActive = false;
	std::wstring _server;
	std::wstring _port;
	std::wstring _name;
	std::wstring _dbName;
};

class MyServer
{
public:
	MyServer() {}
	MyServer(std::wstring url);
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

private:
	static void handle_get(http_request message);
	static void handle_put(http_request message);
	static void handle_post(http_request message);
	static void handle_delete(http_request message);

	http_listener m_listener;
	static std::vector<std::shared_ptr<NodeAttributes>> _nodes;

public:
	std::wstring _server;
	std::wstring _port;

public:
	std::unique_ptr<MyServer> _http;
	std::wstring _url;
};

class NodeClient
{
public:
	NodeClient(std::wstring url);
	void Init();
	void Close();
	bool RegisterToMaster();
	static void Init_LMDB();

	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

private:
	static void handle_get(http_request message);

private:
	http_listener m_listener;

public:
	static std::wstring _server;
	static std::wstring _port;
	static std::wstring _name;
	static std::wstring _dbName;
	static LMDBData m_lmdb;

public:
	std::unique_ptr<NodeClient> _http;
	std::wstring _url;
};


