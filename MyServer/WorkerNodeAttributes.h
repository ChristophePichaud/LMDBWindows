#pragma once

class WorkerNodeAttributes
{
public:
	WorkerNodeAttributes();
	virtual ~WorkerNodeAttributes();

public:
	bool _isActive = false;
	std::wstring _server;
	std::wstring _port;
	std::wstring _name;
	std::wstring _dbName;

};

