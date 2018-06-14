#pragma once
#include "MySharedStuff.h"

class MYSHAREDSTUFF_API CLogger
{
public:
	CLogger();
	virtual ~CLogger();

public:
	void Init(LPTSTR lpszName);
	void WriteLog(LPCTSTR message);

private:
	HANDLE _hFile;
	TCHAR _szName[255];
	TCHAR _szPath[255];
	static CCriticalSection _cs;
};

