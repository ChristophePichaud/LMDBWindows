#pragma once

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

#include "LMDBData.h"

class NodeAttributes
{
public:
	NodeAttributes() {}

	bool _isActive = false;
	std::wstring _server;
	std::wstring _port;
	std::wstring _name;
};

class MyServer
{
public:
	MyServer() {}
	MyServer(std::wstring url);
	void Init();
	void Init_LMDB();
	void Close();

	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

public:
	static bool ExistsNode(std::wstring server, std::wstring port);
	static void ShowNodes();

private:
	static void handle_get(http_request message);
	static void handle_put(http_request message);
	static void handle_post(http_request message);
	static void handle_delete(http_request message);

	http_listener m_listener;
	static LMDBData m_lmdb;
	static std::vector<std::shared_ptr<NodeAttributes>> _nodes;

public:
	std::wstring _server;
	int _port;

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

	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

private:
	static void handle_get(http_request message);

private:
	http_listener m_listener;

public:
	std::wstring _server;
	int _port;
	std::wstring _name;

public:
	std::unique_ptr<NodeClient> _http;
	std::wstring _url;
};


