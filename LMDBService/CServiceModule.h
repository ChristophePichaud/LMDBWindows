#pragma once

#define SERVICE_NAME _T("LMDBService")

class CServiceModule
{
public:
	CServiceModule();
	~CServiceModule();

public:
	bool IsInstalled();
	bool Install();
	void Start();

public:
	void ServiceMain(DWORD dwArgc, LPTSTR * lpszArgv);
	void SetServiceStatus(DWORD dwState);
	void Handler(DWORD dwOpcode);
	void Run();

public:
	static void WINAPI _Handler(DWORD dwOpcode);
	static void WINAPI _ServiceMain(DWORD dwArgc, LPTSTR * lpszArgv);

public:
	SERVICE_STATUS_HANDLE m_hServiceStatus;
	SERVICE_STATUS m_status;
	bool m_bService;
	bool m_bStop;
	DWORD m_dwThreadId;
	HANDLE m_hThread;
};

extern CServiceModule _Module;
DWORD AutomateThread(LPVOID pParam);
