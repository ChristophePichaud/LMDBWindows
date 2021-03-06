// HttpLMDB_Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\Include\Base64Helper.h"

int main()
{
	std::wstring dbname = _T("cache_v100_R2");
	if (Init(dbname) == false)
	{
		std::wcout << _T("Init failed") << std::endl;
		return 0;
	}

	::Sleep(1000);

	TCHAR sz[255];
	_stprintf_s(sz, _T("key_%ld"), GetTickCount());
	std::wstring key = std::wstring(sz);
	_stprintf_s(sz, _T("value_%ld"), GetTickCount());
	std::wstring value = std::wstring(sz);
	SetData(key, value, dbname);
	value = _T("");
	GetData(key, value, dbname);
	std::wcout << _T("key:") << key << _T(" value:") << value << std::endl;
	//return 0;

	int count = 10;
	for (int i = 0; i < count; i++)
	{
		//Sleep(25);
		_stprintf_s(sz, _T("key_%ld"), GetTickCount());
		//_stprintf_s(sz, _T("key_104481812"));
		key = std::wstring(sz);
		std::wstring value2 = _T("azertyuiopqsdfghjklmwxcvbn");
		SetDataB64(key, value2.c_str(), value2.length(), dbname);
		//SetData(key, value2.c_str(), dbname);
		value2 = _T("");
		GetDataB64(key, value2, dbname);
		std::wcout << _T("key:") << key << _T(" value:") << value2 << std::endl;
		value2 = _T("");
		GetData(key, value2, dbname);
		std::wcout << _T("key:") << key << _T(" value:") << value2 << std::endl;
	}
	return 0;

	std::wcout << _T("Set/Get simple values...") << std::endl;
	for (int i = 0; i < count; i++)
	{
		_stprintf_s(sz, _T("key_%d"), i);
		std::wstring key = std::wstring(sz);
		_stprintf_s(sz, _T("value_%d"), i);
		std::wstring value = std::wstring(sz);
		SetData(key, value, dbname);
		std::wcout << _T("SetData key:") << key << _T(" value:") << value << std::endl;
	}

	for (int i = 0; i < count; i++)
	{
		_stprintf_s(sz, _T("key_%d"), i);
		std::wstring key = std::wstring(sz);
		std::wstring value;
		GetData(key, value, dbname);
		std::wcout << _T("GetData key:") << key << _T(" value:") << value << std::endl;
	}

	std::wcout << _T("Set/Get base64 values...") << std::endl;

	for (int i = 0; i < count; i++)
	{
		_stprintf_s(sz, _T("key_%d"), i);
		std::wstring key = std::wstring(sz);
		_stprintf_s(sz, _T("value_%d"), i);
		char sz2[255];
		sprintf_s(sz2, "value_%d", i);
		//std::wstring value = std::wstring(sz);
		std::string buffer = Base64Helper::base64_encode((const unsigned char*)sz2, strlen(sz2));
		std::wstring bufferw(buffer.begin(), buffer.end());
		SetDataB64(key, bufferw, bufferw.length(), dbname);
		std::wcout << _T("SetDataB64 key:") << key << _T(" value:") << bufferw << std::endl;
	}

	for (int i = 0; i < count; i++)
	{
		_stprintf_s(sz, _T("key_%d"), i);
		std::wstring key = std::wstring(sz);
		std::wstring value;
		GetDataB64(key, value, dbname);
		std::wcout << _T("GetDataB64key:") << key << _T(" value:") << value << std::endl;
	}

	Uninit(dbname);
    return 0;
}

				