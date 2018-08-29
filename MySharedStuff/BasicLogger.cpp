#include "stdafx.h"
#include <atlsync.h>
#include "..\Include\BasicLogger.h"

CCriticalSection BasicLogger::_cs;
BasicLogger BasicLogger::InternalLogger;

BasicLogger::BasicLogger()
{
}

BasicLogger::~BasicLogger()
{
	//::CloseHandle(_hFile);
}

bool BasicLogger::Init(LPTSTR lpszName)
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

	_cs.Leave();
	return true;
}

void BasicLogger::WriteLog(LPCTSTR lpszMessage)
{
	USES_CONVERSION;

	_cs.Enter();

	_hFile = ::CreateFileA(W2A(_szPath),
		GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (_hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("Init failed - INVALID_HANDLE_VALUE\n"));
		_cs.Leave();
	}

	LONG l = 0;
	::SetFilePointer(_hFile, 0, &l, FILE_END);

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

	CloseHandle(_hFile);

	_cs.Leave();
}

void BasicLogger::WriteLog(std::wstring message)
{
	WriteLog(message.c_str());
}

void InitLogger()
{
	TCHAR sz[255];
	_tcscpy(sz, _T("Internal.txt"));
	BasicLogger::InternalLogger.Init(sz);
}
