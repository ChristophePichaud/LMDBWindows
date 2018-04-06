#include "stdafx.h"

#ifdef _WIN32
# define iequals(x, y) (_stricmp((x), (y))==0)
#else
# define iequals(x, y) boost::iequals((x), (y))
#endif

int wmain(int argc, wchar_t *argv[])
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

