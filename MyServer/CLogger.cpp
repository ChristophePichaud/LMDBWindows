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
	std::string p(_path.begin(), _path.end());
	std::string m(message.begin(), message.end());

	HANDLE hFile = ::CreateFileA(p.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	LONG l = 0;
	::SetFilePointer(hFile, 0, &l, FILE_END);
	DWORD dwLen = 0;
	::WriteFile(hFile, m.c_str(), message.length(), &dwLen, NULL);
	dwLen = 0;
	::WriteFile(hFile, "\r\n", 2, &dwLen, NULL);
	::CloseHandle(hFile);
}
