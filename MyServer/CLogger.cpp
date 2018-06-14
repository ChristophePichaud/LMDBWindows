#include "stdafx.h"
#include "Logger.h"


CLogger::CLogger()
{
}


CLogger::~CLogger()
{
}

void CLogger::Init(std::wstring name)
{
	_name = name;

	TCHAR szTemp[255];
	_stprintf_s(szTemp, _T("C:\\TEMP\\LOGS"));
	::CreateDirectory(szTemp, NULL);

	TCHAR szPath[255];
	_stprintf_s(szPath, _T("%s\\%s"), szTemp, name.c_str());

	_path = szPath;
}

void CLogger::WriteLog(std::wstring message)
{
	std::string path(_path.begin(), _path.end());

	SYSTEMTIME st;
	memset(&st, 0, sizeof(SYSTEMTIME));
	::GetSystemTime(&st);

	TCHAR sz[1024];
	_stprintf_s(sz, 
		_T("%02d:%02d:%02d.%03d - INFO - %s\r\n"), 
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, 
		message.c_str());

	std::wstring wsz = sz;
	std::string msgToWrite(wsz.begin(), wsz.end());

	HANDLE hFile = ::CreateFileA(path.c_str(), 
		GENERIC_WRITE, FILE_SHARE_WRITE, 
		NULL, OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);

	LONG l = 0;
	::SetFilePointer(hFile, 0, &l, FILE_END);
	
	DWORD dwLen = 0;
	::WriteFile(hFile, 
		msgToWrite.c_str(), msgToWrite.length(), 
		&dwLen, NULL);
	
	::CloseHandle(hFile);

	printf_s(msgToWrite.c_str());
}
