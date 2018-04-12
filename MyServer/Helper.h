#pragma once

class CHelper
{
public:
	CHelper();
	~CHelper();

	static CString GetPrivateProfileString(CString section, CString key, CString fileName);
	static CString GetEnvironmentVariable(CString name);
};

