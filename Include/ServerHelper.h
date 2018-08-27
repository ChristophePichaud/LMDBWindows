#pragma once
#include "MySharedStuff.h"

class MYSHAREDSTUFF_API ServerHelper
{
public:
	ServerHelper() {}
	~ServerHelper() {}

	static std::wstring GetIP();
	static std::vector<std::wstring> GetIPs();
	static std::wstring GetHostName();
	static std::wstring BuildURL(std::wstring server, std::wstring port);
	static std::wstring FindParameterInQuery(std::map<std::wstring, std::wstring>query, std::wstring var);
	static std::wstring FindParameter(http_request message, std::wstring var);
};
