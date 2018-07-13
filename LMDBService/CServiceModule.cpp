#include "stdafx.h"
#include "CServiceModule.h"
#include "TheServer.h"
#include "..\Include\messagetypes.h"
#include "..\Include\ServerHelper.h"
#include "..\Include\Constants.h"

CServiceModule::CServiceModule()
{
	m_bService = TRUE;
	m_bStop = FALSE;

	// set up the initial service status 
	m_hServiceStatus = NULL;
	m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_status.dwCurrentState = SERVICE_STOPPED;
	m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	m_status.dwWin32ExitCode = 0;
	m_status.dwServiceSpecificExitCode = 0;
	m_status.dwCheckPoint = 0;
	m_status.dwWaitHint = 0;

}


CServiceModule::~CServiceModule()
{
}

bool CServiceModule::IsInstalled()
{
	bool bResult = false;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		SC_HANDLE hService = ::OpenService(hSCM,
			SERVICE_NAME,
			SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = true;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}

bool CServiceModule::Install()
{
	if (IsInstalled())
	{
		std::wcout << _T("Service is already installed") << std::endl;
		return TRUE;
	}

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		std::wcout << _T("Couldn't open service manager") << std::endl;
		return FALSE;
	}

	// Get the executable file path
	TCHAR szFilePath[_MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, _MAX_PATH);

	SC_HANDLE hService = ::CreateService(
		hSCM,
		SERVICE_NAME,
		SERVICE_NAME,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, NULL, NULL, NULL);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		std::wcout << _T("Couldn't create service ") << SERVICE_NAME << std::endl;
		return FALSE;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return TRUE;
}

void CServiceModule::Start()
{
	SERVICE_TABLE_ENTRY st[] =
	{
		{ (LPTSTR)SERVICE_NAME, _ServiceMain },
		{ NULL, NULL }
	};

	if (m_bService && !::StartServiceCtrlDispatcher(st))
	{
		m_bService = FALSE;
	}
	if (m_bService == FALSE)
	{
		Run();
	}
}

void CServiceModule::_ServiceMain(DWORD dwArgc, LPTSTR * lpszArgv)
{
	_Module.ServiceMain(dwArgc, lpszArgv);
}

void CServiceModule::ServiceMain(DWORD dwArgc, LPTSTR * lpszArgv)
{
	m_status.dwCurrentState = SERVICE_START_PENDING;
	m_hServiceStatus = RegisterServiceCtrlHandler(SERVICE_NAME, _Handler);
	if (m_hServiceStatus == NULL)
	{
		//LogEvent(_T("Handler not installed"));
		return;
	}
	SetServiceStatus(SERVICE_START_PENDING);

	m_status.dwWin32ExitCode = S_OK;
	m_status.dwCheckPoint = 0;
	m_status.dwWaitHint = 0;

	// When the Run function returns, the service has stopped.
	Run();

	SetServiceStatus(SERVICE_STOPPED);
	//LogEvent(_T("Service stopped"));
	//_Trace.LogDebug("CServiceModule::ServiceMain", "Service stopped");

}

void WINAPI CServiceModule::_Handler(DWORD dwOpcode)
{
	_Module.Handler(dwOpcode);
}

void CServiceModule::SetServiceStatus(DWORD dwState)
{
	m_status.dwCurrentState = dwState;
	::SetServiceStatus(m_hServiceStatus, &m_status);
}

void CServiceModule::Handler(DWORD dwOpcode)
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		m_bStop = TRUE;
		SetServiceStatus(SERVICE_STOPPED);
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		break;
	default:
		//LogEvent(_T("Bad service request"));
		break;
	}
}

void CServiceModule::Run()
{
	_Module.m_dwThreadId = GetCurrentThreadId();

	//LogEvent(_T("Service started"));
	//_Trace.LogError("CServiceModule::Run", "Service started");

	if (m_bService)
		SetServiceStatus(SERVICE_RUNNING);

	m_hThread = ::CreateThread(NULL, 0, AutomateThread, NULL, CREATE_SUSPENDED, &m_dwThreadId);
	if (m_hThread == NULL)
	{
		//LogEvent("Creation du thread AutomateThread impossible");
		SetServiceStatus(SERVICE_STOPPED);
		goto quit_now;
		return;
	}
	::ResumeThread(m_hThread);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
		DispatchMessage(&msg);

quit_now:
	return;
}

DWORD AutomateThread(LPVOID pParam)
{
	CString strLog;
	try
	{
		CoInitialize(NULL);

		std::wcout << _T("Running in console mode... Entering while()...") << std::endl;
		std::wstring port = Constants::MasterNodePort;
		std::wstring ip = ServerHelper::GetIP();
		std::wstring url = ServerHelper::BuildURL(ip, port);
		http::uri uri = http::uri(url);
		std::wstring address = uri.to_string();
		std::wstring name = _T("Master");

		TCHAR sz[255];
		_stprintf_s(sz, _T("Node_%s_%s.log"), port.c_str(), name.c_str());
		g_Logger.Init(sz);
		g_Logger.WriteLog(_T("Init Node..."));

		//
		// Create the server instance
		//

		_stprintf_s(sz, _T("IP : %s"), ip.c_str());
		g_Logger.WriteLog(sz);

		TheServer client(address);
		client._server = ip;
		client._port = port;
		client._name = name;
		client.Init();
		_stprintf_s(sz, _T("Worker node %s"), address.c_str());
		g_Logger.WriteLog(sz);
		//g_Logger.WriteLog(_T("Press ENTER to exit."));
		//std::string line;
		//std::getline(std::cin, line);
		while (TRUE)
		{
			if (_Module.m_bStop)
			{
				std::wcout << _T("Running in console mode.") << std::endl;
				goto stop_service;
				break;
			}
			Sleep(200);

		} // Main loop

	stop_service:
		client.Close();

		strLog.Format(_T("Arrêt du service"));
		//_Module.LogEvent(strLog);
		_Module.SetServiceStatus(SERVICE_STOP_PENDING);
		PostThreadMessage(_Module.m_dwThreadId, WM_QUIT, 0, 0);
	}
	catch (...)
	{
		strLog.Format(_T("EXCEPTION: Arrêt du service"));
		//_Module.LogEvent(strLog);
		//_Trace.LogDebug("AutomateThread", strLog);
		_Module.SetServiceStatus(SERVICE_STOP_PENDING);
		PostThreadMessage(_Module.m_dwThreadId, WM_QUIT, 0, 0);
	}	

	CoUninitialize();
	return 0;
}


