#include "stdafx.h"

#ifdef _WIN32
# define iequals(x, y) (_stricmp((x), (y))==0)
#else
# define iequals(x, y) boost::iequals((x), (y))
#endif

int wmain(int argc, wchar_t *argv[])
{
	std::wstring port = _T("7001");
	if (argc == 2)
	{
		port = argv[1];
	}

	std::wstring address = U("http://localhost:");
	address.append(port);
	http::uri uri = http::uri(address);

	http_client client(http::uri_builder(uri).append_path(_T("/MyServer/LMDB/")).to_uri());

	int count = 100;

	DWORD dwStart = GetTickCount();
	for (int i = 0; i < count; i++)
	{
		std::wstring method = _T("set-data");
		std::wstring key;
		std::wstring value;

		std::wostringstream k, v;
		k << _T("key_") << i;
		v << _T("value_") << i;

		key = k.str();
		value = v.str();;

		std::wostringstream buf;
		buf << _T("?request=") << method
			<< _T("&key=") << key
			<< _T("&value=") << value;

		http_response response;
		response = client.request(methods::GET, buf.str()).get();

		//wcout << response.to_string() << endl;
	}
	DWORD dwStop = GetTickCount();
	TCHAR sz[255];
	_stprintf(sz, _T("set-data for %d elements - Elapsed ms = %ld ms\n"), count, dwStop - dwStart);
	_tprintf(sz);

	dwStart = GetTickCount();
	for (int i = 0; i < count; i++)
	{
		std::wstring method = _T("get-data");
		std::wstring key;
		std::wstring value;

		std::wostringstream k;
		k << _T("key_") << i;

		key = k.str();

		std::wostringstream buf;
		buf << _T("?request=") << method
			<< _T("&key=") << key;

		http_response response;
		response = client.request(methods::GET, buf.str()).get();

		//wcout << response.to_string() << endl;

		json::value jdata = json::value::array();
		jdata = response.extract_json().get();
		Data data = Data::FromJSON(jdata.as_object());

		//_stprintf(sz, _T("Data key:%s value:%s\n"), data.key.c_str(), data.value.c_str());
		//_tprintf(sz);
	}
	dwStop = GetTickCount();
	_stprintf(sz, _T("get-data for %d elements - Elapsed ms = %ld ms\n"), count, dwStop - dwStart);
	_tprintf(sz);

	return 0;
}

int wmain2(int argc, wchar_t *argv[])
{
    utility::string_t port = U("7001");
    if(argc == 2)
    {
        port = argv[1];
    }

    utility::string_t address = U("http://localhost:");
    address.append(port);
    http::uri uri = http::uri(address);

	http_client client(http::uri_builder(uri).append_path(U("/MyServer/LMDB/")).to_uri());

    utility::string_t userName;
    utility::string_t table;

    json::value availableTables = json::value::array();

    bool was_refresh = false;

    while (true)
    {
		std::string method;
		std::string key;
		std::string value;
		ucout << "Enter method:";
		cin >> method;

		http_response response;

		if (iequals(method.c_str(), "set-data"))
		{
			ucout << "Enter key:";
			cin >> key;
			ucout << "Enter value:";
			cin >> value;

			utility::ostringstream_t buf;
			buf << table << U("?request=") << utility::conversions::to_string_t(method) 
				<< U("&key=") << utility::conversions::to_string_t(key)
				<< U("&value=") << utility::conversions::to_string_t(value);
			
			response = client.request(methods::GET, buf.str()).get();

			ucout << response.to_string() << endl;
		}
		else if (iequals(method.c_str(), "get-data"))
		{
			ucout << "Enter key:";
			cin >> key;

			utility::ostringstream_t buf;
			buf << table << U("?request=") << utility::conversions::to_string_t(method)
				<< U("&key=") << utility::conversions::to_string_t(key);

			response = client.request(methods::GET, buf.str()).get();

			ucout << response.to_string() << endl;

			json::value jdata = json::value::array();
			jdata = response.extract_json().get();
			Data data = Data::FromJSON(jdata.as_object());
		}
		else
        {
            ucout << utility::conversions::to_string_t(method) << ": not understood\n";
        }
    }

    return 0;
}

