#pragma once

class TheServer
{
public:
	TheServer(std::wstring url);
	virtual ~TheServer();

	void Init();
	void Close();
	static void RequestVerbPing(http_request message);
	static void RequestVerbGetData(http_request message);
	static void RequestVerbSetData(http_request message);
	static void RequestVerbGetData64(http_request message);
	static void RequestVerbSetData64(http_request message);
	static void RequestUsage(http_request message);

public:
	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

private:
	static void PrintRequest(http_request request);
	static void handle_get(http_request message);
	static void handle_post(http_request message);
	static void handle_del(http_request message);
	static void handle_put(http_request message);
	static std::shared_ptr<CLMDBWrapperEx> GetLMDBWrapper(std::wstring dbName);


private:
	http_listener m_listener;

public:
	static std::wstring _server;
	static std::wstring _port;
	static std::wstring _name;

public:
	static std::map<std::wstring, std::shared_ptr<CLMDBWrapperEx>> _mapWrapper;
	std::unique_ptr<TheServer> _http;
	std::wstring _url;
};
