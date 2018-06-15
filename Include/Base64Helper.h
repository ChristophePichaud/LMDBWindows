#pragma once
#include "MySharedStuff.h"

class MYSHAREDSTUFF_API Base64Helper
{
public:
	Base64Helper();
	~Base64Helper();

public:
	static const std::string base64_chars;
	static bool is_base64(unsigned char c);
	static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
	static std::string base64_decode(std::string const& encoded_string);
};

