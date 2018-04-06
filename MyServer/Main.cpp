#include "stdafx.h"
#include "MyServer.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

std::unique_ptr<MyServer> g_http;

int wmain(int argc, wchar_t *argv[])
{
    utility::string_t port = U("7001");
    if(argc == 2)
    {
        port = argv[1];
    }

    utility::string_t address = U("http://localhost:");
    address.append(port);
	uri_builder uri(address);
	uri.append_path(U("MyServer/LMDB/"));

	auto addr = uri.to_uri().to_string();

	//
	// Create the server instance
	//

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