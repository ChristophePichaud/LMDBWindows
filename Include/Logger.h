#pragma once
#include "MySharedStuff.h"

class MYSHAREDSTUFF_API CLogger
{
public:
	CLogger();
	virtual ~CLogger();

public:
	bool Init(LPTSTR lpszName);
	void WriteLog(LPCTSTR message);
	void WriteLog(std::wstring message);

private:
	HANDLE _hFile;
	TCHAR _szName[255];
	TCHAR _szPath[255];
	static CCriticalSection _cs;
};

