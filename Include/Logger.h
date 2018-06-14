#pragma once
#include "MySharedStuff.h"

class MYSHAREDSTUFF_API CLogger
{
public:
	CLogger();
	virtual ~CLogger();

public:
	void Init(std::wstring name);
	void WriteLog(std::wstring message);

private:
	std::wstring _name;
	std::wstring _path;
	static CCriticalSection _cs;
};

