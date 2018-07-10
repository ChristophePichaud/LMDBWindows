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

public:
	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

private:
	static void PrintRequest(http_request request);
	static void handle_get(http_request message);

private:
	http_listener m_listener;

public:
	static std::wstring _server;
	static std::wstring _port;
	static std::wstring _name;

public:
	std::unique_ptr<TheServer> _http;
	std::wstring _url;
};
