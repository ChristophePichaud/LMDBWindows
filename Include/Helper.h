#pragma once
#include "MySharedStuff.h"

class MYSHAREDSTUFF_API CHelper
{
public:
	CHelper() {}
	~CHelper() {}

	static std::wstring GetIP();
	static std::wstring BuildURL(std::wstring server, std::wstring port);
	static std::wstring FindParameterInQuery(std::map<std::wstring, std::wstring>query, std::wstring var);
	static const std::string base64_chars;
	static bool is_base64(unsigned char c);
	static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
	static std::string base64_decode(std::string const& encoded_string);
};
