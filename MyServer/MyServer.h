#pragma once

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

#include "LMDBData.h"

class MyServer
{
public:
	MyServer() {}
	MyServer(std::wstring url);

	void Init_LMDB();

	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

private:

	static void handle_get(http_request message);
	static void handle_put(http_request message);
	static void handle_post(http_request message);
	static void handle_delete(http_request message);

	http_listener m_listener;
	static LMDBData m_lmdb;
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

public:
	static void handle_get(http_request message);

private:
	http_listener m_listener;

private:
	std::wstring _server;
	int _port;
	std::wstring _fullURL;

public:
	std::unique_ptr<NodeClient> _http;
	std::wstring _url;
};

#if NODES
class NodeServer
{
public:
	NodeServer(std::wstring url);

public:
	bool _isActive = false;
	std::vector<NodeClient> _nodes;
};
#endif
