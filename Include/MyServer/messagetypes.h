#pragma once
#include "stdafx.h"


struct Data
{
	utility::string_t key;
	utility::string_t value;

	static Data FromJSON(const web::json::object & object)
	{
		Data result;
		result.key = object.at(U("key")).as_string();
		result.value = object.at(U("value")).as_string();
		return result;
	}

	web::json::value AsJSON() const
	{
		web::json::value result = web::json::value::object();
		result[U("key")] = web::json::value::string(key);
		result[U("value")] = web::json::value::string(value);
		return result;
	}
};

