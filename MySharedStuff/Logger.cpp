#include "stdafx.h"
#include <atlsync.h>
#include "..\Include\Logger.h"


CCriticalSection CLogger::_cs;

CLogger::CLogger()
{
}

CLogger::~CLogger()
{
	::CloseHandle(_hFile);
}

void CLogger::Init(LPTSTR lpszName)
{																									
	USES_CONVERSION;

	_cs.Enter();
	_tcscpy(_szName, lpszName);

	TCHAR szTemp[255];
	_stprintf_s(szTemp, _T("C:\\TEMP\\LOGS"));
	::CreateDirectory(szTemp, NULL);

	TCHAR szPath[255];
	_stprintf_s(szPath, _T("%s\\%s"), szTemp, lpszName);

	_tcscpy(_szPath, szPath);

	_hFile = ::CreateFileA(W2A(_szPath),
		GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	LONG l = 0;
	::SetFilePointer(_hFile, 0, &l, FILE_END);

	_cs.Leave();
}

void CLogger::WriteLog(LPCTSTR lpszMessage)
{
	USES_CONVERSION;

	_cs.Enter();
	
	SYSTEMTIME st;
	memset(&st, 0, sizeof(SYSTEMTIME));
	::GetSystemTime(&st);

	TCHAR sz[10240];
	_stprintf_s(sz, 
		_T("%02d:%02d:%02d.%03d - INFO - %s\r\n"), 
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, 
		lpszMessage);

	LPSTR msg = W2A(sz);

	printf_s(msg);

	DWORD dwLen = 0;
	::WriteFile(_hFile,
		msg, strlen(msg),
		&dwLen, NULL);

	/*
	HANDLE hFile = ::CreateFileA(W2A(_szPath),
		GENERIC_WRITE, FILE_SHARE_WRITE, 
		NULL, OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);

	LONG l = 0;
	::SetFilePointer(hFile, 0, &l, FILE_END);
	
	DWORD dwLen = 0;
	::WriteFile(hFile, 
		msg, strlen(msg), 
		&dwLen, NULL);
	
	::CloseHandle(hFile);
	*/
	_cs.Leave();
}
