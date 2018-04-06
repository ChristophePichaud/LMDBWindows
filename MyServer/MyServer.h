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
	MyServer(utility::string_t url);

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
