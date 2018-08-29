#pragma once
#include "MySharedStuff.h"

class MYSHAREDSTUFF_API BasicLogger
{
public:
	BasicLogger();
	virtual ~BasicLogger();

public:
	bool Init(LPTSTR lpszName);
	void WriteLog(LPCTSTR message);
	void WriteLog(std::wstring message);

private:
	HANDLE _hFile;
	TCHAR _szName[255];
	TCHAR _szPath[255];
	static CCriticalSection _cs;

public:
	static BasicLogger InternalLogger;
};


void MYSHAREDSTUFF_API InitLogger();
