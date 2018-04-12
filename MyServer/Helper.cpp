#include "stdafx.h"
#include "Helper.h"


CHelper::CHelper()
{
}


CHelper::~CHelper()
{
}

CString CHelper::GetPrivateProfileString(CString section, CString key, CString fileName)
{
	TCHAR lpszReturn[255];
	memset(lpszReturn, 0, 255);

	DWORD dw = ::GetPrivateProfileString((LPCTSTR)section, (LPCTSTR)key, NULL, lpszReturn, 255, (LPCTSTR)fileName);
	CString retVal = lpszReturn;
	return retVal;
}

CString CHelper::GetEnvironmentVariable(CString name)
{
	TCHAR lpszType[255];
	memset(lpszType, 0, 255);
	DWORD c = 255;
	::GetEnvironmentVariable((LPCTSTR)name, lpszType, c);

	CString retVal = lpszType;
	return retVal;
}