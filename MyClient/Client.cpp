#include "stdafx.h"

#include "..\Include\messagetypes.h"
#include "..\Include\Helper.h"
#include "..\Include\Constants.h"

#ifdef _WIN32
# define iequals(x, y) (_stricmp((x), (y))==0)
#else
# define iequals(x, y) boost::iequals((x), (y))
#endif

int wmain(int argc, wchar_t *argv[])
{
	std::wstring defaultAddress = _T("localhost");
	std::wstring port = Constants::MasterNodePort;
	int count = 2;
	std::wstring name = _T("cache_v1");

	if (argc == 2)
	{
		port = argv[1];
	}
	if (argc == 3)
	{
		defaultAddress = argv[1];
		port = argv[2];
	}

	if (argc == 4)
	{
		defaultAddress = argv[1];
		port = argv[2];
		name = argv[3];
	}

	if (argc == 5)
	{
		defaultAddress = argv[1];
		port = argv[2];
		name = argv[3];
		count = _wtoi(argv[4]);
	}

	std::wstring url = CHelper::BuildURL(defaultAddress, port);
	std::wcout << L"Client " << url << std::endl;
	std::wcout << L"count: " << count << std::endl;
	
	http_client client(url);
	
	std::wostringstream buf;
	buf << Constants::Request << Constants::VerbGetNode
		<< _T("&dbname=") << name;

	http_response response;

	try
	{
		response = client.request(methods::GET, buf.str()).get();
		wcout << response.to_string() << endl;
	}
	catch (http_exception ex)
	{
		std::string err = ex.what();
	}

	json::value jdata = json::value::array();
	jdata = response.extract_json().get();

	if (jdata.is_null())
	{
		std::wcout << _T("no JSON data...") << std::endl;
		return 0;
	}

	GetNodeData data = GetNodeData::FromJSON(jdata.as_object());

	TCHAR sz[255];
	_stprintf_s(sz, _T("GetNodeData ip:%s port:%s name:%s dbname:%s\n"), data.ip.c_str(), data.port.c_str(), data.name.c_str(), data.dbName.c_str());
	_tprintf(sz);

	::Sleep(1000);

	url = CHelper::BuildURL(data.ip, data.port);
	http_client client_lmdb(url);

	DWORD dwStart = GetTickCount();
	for (int i = 0; i < count; i++)
	{
		std::wstring key;
		std::wstring value;

		std::wostringstream k, v;
		k << _T("key_") << i;
		v << _T("value_") << i;

		key = k.str();
		value = v.str();

		std::wostringstream buf;
		buf << Constants::Request << Constants::VerbSetData
			<< _T("&key=") << key
			<< _T("&value=") << value;

		http_response response;
		response = client_lmdb.request(methods::GET, buf.str()).get();

		wcout << response.to_string() << endl;
	}

	DWORD dwStop = GetTickCount();
	_stprintf_s(sz, _T("set-data for %d elements - Elapsed ms = %ld ms\n"), count, dwStop - dwStart);
	_tprintf(sz);

	dwStart = GetTickCount();
	for (int i = 0; i < count; i++)
	{
		std::wstring key;
		std::wstring value;

		std::wostringstream k;
		k << _T("key_") << i;

		key = k.str();

		std::wostringstream buf;
		buf << Constants::Request << Constants::VerbGetData
			<< _T("&key=") << key;

		wcout << buf.str() << endl;

		http_response response;
		response = client_lmdb.request(methods::GET, buf.str()).get();

		wcout << response.to_string() << endl;

		json::value jdata = json::value::array();
		jdata = response.extract_json().get();
		Data data = Data::FromJSON(jdata.as_object());

		_stprintf_s(sz, _T("Data key:%s value:%s\n"), data.key.c_str(), data.value.c_str());
		_tprintf(sz);
	}

	dwStop = GetTickCount();
	_stprintf_s(sz, _T("get-data for %d elements - Elapsed ms = %ld ms\n"), count, dwStop - dwStart);
	_tprintf(sz);

	return 0;
}


