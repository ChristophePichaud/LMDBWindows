#pragma once
#include "LMDBData.h"

class WorkerNodeClient
{
public:
	WorkerNodeClient(std::wstring url);
	virtual ~WorkerNodeClient();

	void Init();
	void Close();
	bool RegisterToMaster();
	static void Init_LMDB();

	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

private:
	static void handle_get(http_request message);

private:
	http_listener m_listener;

public:
	static std::wstring _server;
	static std::wstring _port;
	static std::wstring _name;
	static std::wstring _dbName;
	static LMDBData m_lmdb;

public:
	std::unique_ptr<WorkerNodeClient> _http;
	std::wstring _url;
};


