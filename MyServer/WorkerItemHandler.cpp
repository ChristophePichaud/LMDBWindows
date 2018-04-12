#include "stdafx.h"
#include "DataHandler.h"
#include "WorkerItemHandler.h"
#include "ManagementClass.h"
#include "Helper.h"

WorkerItemHandler::WorkerItemHandler()
{
}


WorkerItemHandler::~WorkerItemHandler()
{
}

bool WorkerItemHandler::GetComputerName()
{
	//
	// Computer Name
	//
	TCHAR lpszComputerName[255];
	DWORD dwSize = 255;
	::GetComputerName(lpszComputerName, &dwSize);
	m_data.m_ComputerName = lpszComputerName;

	return true;
}

bool WorkerItemHandler::GetEnvironmentStrings()
{
	LPWCH lpvEnv = ::GetEnvironmentStrings();
	// Variable strings are separated by NULL byte, and the block is terminated by a NULL byte.
	LPTSTR lpszVariable;
	for (lpszVariable = (LPTSTR)lpvEnv; *lpszVariable; lpszVariable++)
	{
		CString env = CString(lpszVariable);
		m_data.m_Env.Add(env);

		while (*lpszVariable)
		{
			lpszVariable++;
		}
	}
	::FreeEnvironmentStrings(lpvEnv);

	return true;
}

bool WorkerItemHandler::GetSystemInformation()
{
	SYSTEM_INFO SystemInfo;
	memset(&SystemInfo, 0, sizeof(SYSTEM_INFO));
	::GetSystemInfo(&SystemInfo);
	m_data.m_NumOfProcessors = SystemInfo.dwNumberOfProcessors;
	switch (SystemInfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		m_data.m_ProcessorArchitecture = "x64";
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		m_data.m_ProcessorArchitecture = "x86";
		break;
	}

	OSVERSIONINFOEX VersionInfo;
	memset(&VersionInfo, 0, sizeof(OSVERSIONINFOEX));
	VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	BOOL result = ::GetVersionEx((OSVERSIONINFO*)&VersionInfo);
	CString version;
	version.Format(_TEXT("%d.%d.%d"), VersionInfo.dwMajorVersion, VersionInfo.dwMinorVersion, VersionInfo.dwBuildNumber);
	m_data.m_Version = version;
	return true;
}

/*
bool WorkerItemHandler::GetWMIInformations()
{
HRESULT hres;
hres = CoInitializeEx(0, COINIT_MULTITHREADED);
if (FAILED(hres))
{
return false;
}

hres = CoInitializeSecurity(
NULL,
-1,                          // COM authentication
NULL,                        // Authentication services
NULL,                        // Reserved
RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
NULL,                        // Authentication info
EOAC_NONE,                   // Additional capabilities
NULL                         // Reserved
);


if (FAILED(hres))
{
CoUninitialize();
return false;
}

IWbemLocator *pLoc = NULL;

hres = CoCreateInstance(
CLSID_WbemLocator,
0,
CLSCTX_INPROC_SERVER,
IID_IWbemLocator, (LPVOID *)&pLoc);


if (FAILED(hres))
{
CoUninitialize();
return false;
}

// Connect to WMI through the IWbemLocator::ConnectServer method

CComPtr<IWbemServices> pSvc = NULL;

// Connect to the root\cimv2 namespace with
// the current user and obtain pointer pSvc
// to make IWbemServices calls.
hres = pLoc->ConnectServer(
_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
NULL,                    // User name. NULL = current user
NULL,                    // User password. NULL = current
0,                       // Locale. NULL indicates current
NULL,                    // Security flags.
0,                       // Authority (e.g. Kerberos)
0,                       // Context object
&pSvc                    // pointer to IWbemServices proxy
);

if (FAILED(hres))
{
pLoc->Release();
CoUninitialize();
return false;                // Program has failed.
}

hres = CoSetProxyBlanket(
pSvc,                        // Indicates the proxy to set
RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
NULL,                        // Server principal name
RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
NULL,                        // client identity
EOAC_NONE                    // proxy capabilities
);

if (FAILED(hres))
{
pLoc->Release();
CoUninitialize();
return false;
}

// Use the IWbemServices pointer to make requests of WMI ----

// For example, get the name of the operating system
//IEnumWbemClassObject* pEnumerator = NULL;
CComPtr<IEnumWbemClassObject> pEnumerator = NULL;
hres = pSvc->ExecQuery(
bstr_t("WQL"),
bstr_t("SELECT * FROM Win32_OperatingSystem"),
WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
NULL,
&pEnumerator);

if (FAILED(hres))
{
pLoc->Release();
CoUninitialize();
return false;
}

ULONG uReturn = 0;
while (pEnumerator)
{
CComPtr<IWbemClassObject> pclsObj;
HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
if (hr == S_FALSE)
{
break;
}

CComVariant vtProp;
hr = pclsObj->Get(_TEXT("Name"), 0, &vtProp, 0, 0);
AfxBSTR2CString(&m_data.m_ComputerDescription, vtProp.bstrVal);
}

pLoc->Release();
CoUninitialize();

return true;
}
*/

bool WorkerItemHandler::GetWMIInformation2()
{
	ManagementClass mc(_TEXT("Win32_OperatingSystem"));
	bool result = mc.GetInstances();
	while (mc.MoveNext())
	{
		m_data.m_OperatingSystem = mc.GetStringProperty(_TEXT("Name"));
	}
	return true;
}

bool WorkerItemHandler::GetBiosInformation()
{
	ManagementClass mc(_TEXT("Win32_BIOS"));
	bool result = mc.GetInstances();
	while (mc.MoveNext())
	{
		CString data;
		m_data.m_BiosDescription = mc.GetStringProperty(_TEXT("Description"));
		m_data.m_BiosManufacturer = mc.GetStringProperty(_TEXT("Manufacturer"));
		m_data.m_BiosSerialNumber = mc.GetStringProperty(_TEXT("SerialNumber"));
		m_data.m_BiosVersion = mc.GetStringProperty(_TEXT("SMBIOSBIOSVersion"));
	}
	return true;
}

bool WorkerItemHandler::GetPrinterInformation()
{
	ManagementClass mc(_TEXT("Win32_Printer"));
	bool result = mc.GetInstances();
	while (mc.MoveNext())
	{
		CPrinter printer;
		printer.m_Name = mc.GetStringProperty(_TEXT("Name"));
		printer.m_bDefault = mc.GetBoolProperty(_TEXT("Default"));
		printer.m_DriverName = mc.GetStringProperty(_TEXT("DriverName"));
		printer.m_EnableBIDI = mc.GetBoolProperty(_TEXT("EnableBIDI"));
		printer.m_bNetwork = mc.GetBoolProperty(_TEXT("Network"));
		m_data.m_Printers.Add(printer);
	}
	return true;
}

bool WorkerItemHandler::GetPrinterInformation2()
{
	ManagementClass mc(_TEXT("Win32_USBHub"));
	bool result = mc.GetInstances();
	while (mc.MoveNext())
	{
		CUsb usb;
		usb.m_DeviceID = mc.GetStringProperty(_TEXT("DeviceID"));
		m_data.m_Usbs.Add(usb);
	}
	return true;
}

bool WorkerItemHandler::GetVideoInformation()
{
	ManagementClass mc(_TEXT("Win32_VideoController"));
	bool result = mc.GetInstances();
	while (mc.MoveNext())
	{
		CVideo video;
		video.m_Description = mc.GetStringProperty(_TEXT("Description"));
		video.m_DriverVersion = mc.GetStringProperty(_TEXT("DriverVersion"));
		video.m_ErrorCode = mc.GetIntProperty(_TEXT("ConfigManagerErrorCode"));
		m_data.m_Videos.Add(video);
	}
	return true;
}

bool WorkerItemHandler::GetDiskDriveInformation()
{
	ManagementClass mc(_TEXT("Win32_DiskDrive"));
	bool result = mc.GetInstances();
	while (mc.MoveNext())
	{
		CDiskDrive disk;
		disk.m_Name = mc.GetStringProperty(_TEXT("Name"));
		disk.m_Partitions = mc.GetIntProperty(_TEXT("Partitions"));
		disk.m_Model = mc.GetStringProperty(_TEXT("Model"));
		disk.m_FirmwareRevision = mc.GetStringProperty(_TEXT("FirmwareRevision"));
		disk.m_SerialNumber = mc.GetStringProperty(_TEXT("SerialNumber"));
		disk.m_Size = mc.GetLongProperty(_TEXT("Size"));
		disk.m_InterfaceType = mc.GetStringProperty(_TEXT("InterfaceType"));
		disk.m_MediaType = mc.GetStringProperty(_TEXT("MediaType"));
		m_data.m_Disks.Add(disk);
	}

	ManagementClass mc2(_TEXT("Win32_DiskDriveToDiskPartition"));
	result = mc2.GetInstances();
	while (mc2.MoveNext())
	{
		CDiskPartition p;
		p.m_Antecedent = mc2.GetStringProperty(_TEXT("Antecedent"));
		p.m_Dependent = mc2.GetStringProperty(_TEXT("Dependent"));
		m_data.m_DiskDrive.Add(p);
	}

	ManagementClass mc3(_TEXT("Win32_LogicalDiskToPartition"));
	result = mc3.GetInstances();
	while (mc3.MoveNext())
	{
		CDiskPartition p;
		p.m_Antecedent = mc3.GetStringProperty(_TEXT("Antecedent"));
		p.m_Dependent = mc3.GetStringProperty(_TEXT("Dependent"));
		m_data.m_DiskLogical.Add(p);
	}

	return true;
}

bool WorkerItemHandler::GetModelInformation()
{
	ManagementClass mc(_TEXT("Win32_ComputerSystem"));
	bool result = mc.GetInstances();
	while (mc.MoveNext())
	{
		m_data.m_Model = mc.GetStringProperty(_TEXT("Model"));
		m_data.m_TotalMemory = mc.GetStringProperty(_TEXT("TotalPhysicalMemory"));
	}

	ManagementClass mc2(_TEXT("Win32_Processor"));
	result = mc2.GetInstances();
	while (mc2.MoveNext())
	{
		m_data.m_Processor = mc2.GetStringProperty(_TEXT("Name"));
	}
	return true;
}

bool WorkerItemHandler::GetNetworkAdapterInformation()
{
	ManagementClass mc(_TEXT("Win32_NetworkAdapter"));
	bool result = mc.GetInstances();
	while (mc.MoveNext())
	{
		CNetworkCard card;
		card.m_Name = mc.GetStringProperty(_TEXT("Name"));
		card.m_MACAddress = mc.GetStringProperty(_TEXT("MACAddress"));
		card.m_Speed = mc.GetStringProperty(_TEXT("Speed"));
		m_data.m_NetworkCards.Add(card);
	}

	ManagementClass mc2(_TEXT("Win32_NetworkAdapterConfiguration"));
	result = mc2.GetInstances();
	while (mc2.MoveNext())
	{
		COleSafeArray sa;
		if (mc2.GetSafeArrayProperty(_TEXT("IPAddress"), sa) == false)
		{
			continue;
		}
		else
		{
			//DWORD dwDim = sa.GetDim();
			//LONG lBound = 0;
			//sa.GetLBound(1, &lBound);
			//LONG uBound = 0;
			//sa.GetUBound(1, &uBound);
			//long index[1];
			//index[0] = lBound;
			//COleVariant vData;
			//sa.GetElement(index, (void *)&vData);
			m_data.m_IPV4 = (((wchar_t* *)(*(((tagVARIANT*)(&(sa)))->parray)).pvData))[0];
			m_data.m_IPV6 = (((wchar_t* *)(*(((tagVARIANT*)(&(sa)))->parray)).pvData))[1];
		}
	}

	return true;
}

bool WorkerItemHandler::GetQFEInformation()
{
	ManagementClass mc(_TEXT("Win32_QuickFixEngineering"));
	bool result = mc.GetInstances();
	while (mc.MoveNext())
	{
		CQFE qfe;
		qfe.m_Caption = mc.GetStringProperty(_TEXT("Caption"));
		qfe.m_Description = mc.GetStringProperty(_TEXT("Description"));
		qfe.m_HotFixID = mc.GetStringProperty(_TEXT("HotFixID"));
		qfe.m_InstalledOn = mc.GetStringProperty(_TEXT("InstalledOn"));
		m_data.m_Qfes.Add(qfe);
	}
	return true;
}

bool WorkerItemHandler::GetWorkstationInformation()
{
	TCHAR lpszWindowsDir[255];
	memset(lpszWindowsDir, 0, 255);
	::GetWindowsDirectory(lpszWindowsDir, 255);

	CString strIniFile;
	strIniFile.Format(_TEXT("%s\\CL.INI"), lpszWindowsDir);

	TCHAR lpszReturn[255];
	memset(lpszReturn, 0, 255);
	CString strSection = _TEXT("Poste de travail");

	m_data.m_Workstation.m_AdmMachine = CHelper::GetPrivateProfileString(strSection, _TEXT("Adm_Machine"), strIniFile);
	m_data.m_Workstation.m_AgenceStation = CHelper::GetPrivateProfileString(strSection, _TEXT("Agence_Station"), strIniFile);
	m_data.m_Workstation.m_Idmsr = CHelper::GetPrivateProfileString(strSection, _TEXT("IdmsR"), strIniFile);
	m_data.m_Workstation.m_Planet = CHelper::GetPrivateProfileString(strSection, _TEXT("Planet"), strIniFile);
	m_data.m_Workstation.m_Identifiant = CHelper::GetPrivateProfileString(strSection, _TEXT("Identifiant"), strIniFile);
	m_data.m_Workstation.m_Type = CHelper::GetPrivateProfileString(strSection, _TEXT("Type"), strIniFile);
	m_data.m_Workstation.m_NomAbonne = CHelper::GetPrivateProfileString(strSection, _TEXT("NomAbonne"), strIniFile);
	m_data.m_Workstation.m_NomAbonneR = CHelper::GetPrivateProfileString(strSection, _TEXT("NomAbonneR"), strIniFile);
	m_data.m_Workstation.m_VersionAbonne = CHelper::GetPrivateProfileString(strSection, _TEXT("VersionAbonne"), strIniFile);
	m_data.m_Workstation.m_Metier = CHelper::GetPrivateProfileString(strSection, _TEXT("Metier"), strIniFile);
	m_data.m_Workstation.m_Entite = CHelper::GetPrivateProfileString(strSection, _TEXT("Entite"), strIniFile);
	m_data.m_Workstation.m_NumPDTQualiparc = CHelper::GetPrivateProfileString(strSection, _TEXT("NumPDTQualiparc"), strIniFile);
	m_data.m_Workstation.m_NumUCQualiparc = CHelper::GetPrivateProfileString(strSection, _TEXT("NumUCQualiparc"), strIniFile);

	TCHAR lpszValue[255];
	memset(lpszValue, 0, 255);
	CRegKey key;
	LONG ret = 0;
	ULONG c = 255;
	ret = key.Open(HKEY_LOCAL_MACHINE, _TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), KEY_READ);
	if (ret == ERROR_SUCCESS)
	{
		CString strVersion;
		CString strBuild;

		c = 255;
		key.QueryStringValue(_TEXT("CurrentVersion"), lpszValue, &c);
		strVersion = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		key.QueryStringValue(_TEXT("CurrentBuild"), lpszValue, &c);
		strBuild = lpszValue;

		m_data.m_Workstation.m_VersionOS.Format(_TEXT("%s.%s"), strVersion, strBuild);

		c = 255;
		memset(lpszValue, 0, 255);
		key.QueryStringValue(_TEXT("ProductName"), lpszValue, &c);
		m_data.m_Workstation.m_VersionOSString = lpszValue;
	}

	key.Close();

	m_data.m_Workstation.m_TypePoste = CHelper::GetEnvironmentVariable(_TEXT("TYPE"));
	m_data.m_Workstation.m_ComputerName = CHelper::GetEnvironmentVariable(_TEXT("COMPUTERNAME"));
	m_data.m_Workstation.m_CurrentUser = CHelper::GetEnvironmentVariable(_TEXT("USERNAME"));
	m_data.m_Workstation.m_UserDomain = CHelper::GetEnvironmentVariable(_TEXT("USERDOMAIN"));

	CRegKey keyLCL;
	ret = 0;
	ret = keyLCL.Open(HKEY_LOCAL_MACHINE, _TEXT("SOFTWARE\\LCL"), KEY_READ);
	if (ret == ERROR_SUCCESS)
	{
		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("ABONNE"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_Abonne = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("DATE_ABONNE"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_DateAbonne = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("DATEINST"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_DateInst = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("DATESOCLE"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_DateSocle = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("INSTPLATEFORME"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_InstPlateforme = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("LCLAppABOPDT"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_LCLAppABOPDT = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("LCLAppMDTABOPDT"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_LCLAppMDTABOPDT = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("LCLAppMetierPDT"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_LCLAppMetierPDT = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("LCLMediaVersion"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_LCLMediaVersion = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("LCLMIG"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_LCLMIG = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("PDT"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_PDT = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("TYPE"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_Type = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("TYPEMASTER"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_TypeMaster = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("TYPEPOSTE"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_TypePoste = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("VERSIONABONNE"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_VersionAbonne = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("VERSIONPROC"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_VersionProc = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("VERSIONSOCLE"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_VersionSocle = lpszValue;

		c = 255;
		memset(lpszValue, 0, 255);
		keyLCL.QueryStringValue(_TEXT("VERSIONVIA"), lpszValue, &c);
		m_data.m_Workstation.m_Registry.m_VersionVia = lpszValue;
	}

	keyLCL.Close();

	ret = 0;
	ret = keyLCL.Open(HKEY_LOCAL_MACHINE, _TEXT("SOFTWARE\\LCL"), KEY_READ);
	if (ret == ERROR_SUCCESS)
	{
		DWORD dwIndex = 0;
		DWORD count = 255;
		TCHAR szValueName[255];
		LONG lRet;

		while ((lRet = ::RegEnumValue(keyLCL.m_hKey, dwIndex, szValueName, &count, NULL, NULL, NULL, NULL)) != ERROR_NO_MORE_ITEMS)
		{
			if (lRet == ERROR_SUCCESS)
			{
				ULONG ulChars = 255;
				CString strValue;
				keyLCL.QueryStringValue(szValueName, strValue.GetBuffer(255), &ulChars);
				strValue.ReleaseBuffer();

				// Add to the array
				CValueNameValue vnv;
				vnv.m_NameValue = szValueName;
				vnv.m_Value = strValue;
				m_data.m_Workstation.m_RegistryLCL.Add(vnv);
			}
			dwIndex++;
			count = 255;
		}

	}

	keyLCL.Close();
	return true;
}

bool WorkerItemHandler::GetInternetExplorerInformation()
{
	int iVersion = 0;
	TCHAR lpszValue[255];
	memset(lpszValue, 0, 255);
	CRegKey key;
	LONG ret = 0;
	ULONG count = 255;
	ret = key.Open(HKEY_LOCAL_MACHINE, _TEXT("Software\\Microsoft\\Internet Explorer"), KEY_READ);
	if (ret == ERROR_SUCCESS)
	{
		USES_CONVERSION;

		count = 255;
		memset(lpszValue, 0, 255);
		key.QueryStringValue(_TEXT("Version"), lpszValue, &count);

		count = 255;
		memset(lpszValue, 0, 255);
		key.QueryStringValue(_TEXT("svcVersion"), lpszValue, &count);

		m_data.m_InternetExplorer.m_Version = lpszValue;
		int start = 0;
		CString version = m_data.m_InternetExplorer.m_Version.Tokenize(_TEXT("% ."), start);
		iVersion = atoi(W2A((LPCTSTR)version));

	}
	key.Close();

	ret = key.Open(HKEY_CURRENT_USER, _TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), KEY_READ);
	if (ret == ERROR_SUCCESS)
	{
		count = 255;
		memset(lpszValue, 0, 255);
		key.QueryStringValue(_TEXT("AutoConfigURL"), lpszValue, &count);
		m_data.m_InternetExplorer.m_ProxyPac = lpszValue;

		count = 255;
		memset(lpszValue, 0, 255);
		DWORD proxyEnable = 0;
		key.QueryDWORDValue(_TEXT("ProxyEnable"), proxyEnable);
		if (proxyEnable == 1)
		{
			m_data.m_InternetExplorer.m_ProxyServerEnabled = "Oui";
		}
		else
		{
			m_data.m_InternetExplorer.m_ProxyServerEnabled = "Non";
		}

		count = 255;
		memset(lpszValue, 0, 255);
		key.QueryStringValue(_TEXT("ProxyServer"), lpszValue, &count);
		m_data.m_InternetExplorer.m_ProxyServer = lpszValue;

		TCHAR lpszValue2[2550];
		memset(lpszValue2, 0, 2550);
		count = 2550;
		key.QueryStringValue(_TEXT("ProxyOverride"), lpszValue2, &count);
		m_data.m_InternetExplorer.m_ProxyExceptions = lpszValue2;
	}
	key.Close();

	if (iVersion <= 9)
	{
		ret = key.Open(HKEY_CURRENT_USER, _TEXT("Software\\Policies\\Microsoft\\Internet Explorer\\Privacy"), KEY_READ);
		if (ret == ERROR_SUCCESS)
		{
			count = 255;
			memset(lpszValue, 0, 255);
			key.QueryStringValue(_TEXT("ClearBrowsingHistoryOnExit"), lpszValue, &count);
			m_data.m_InternetExplorer.m_ClearCache = lpszValue;
		}
		key.Close();
	}
	else
	{
		ret = key.Open(HKEY_CURRENT_USER, _TEXT("Software\\Microsoft\\Internet Explorer\\Privacy"), KEY_READ);
		if (ret == ERROR_SUCCESS)
		{
			count = 255;
			memset(lpszValue, 0, 255);
			key.QueryStringValue(_TEXT("ClearBrowsingHistoryOnExit"), lpszValue, &count);
			m_data.m_InternetExplorer.m_ClearCache = lpszValue;
		}
		key.Close();
	}

	ret = key.Open(HKEY_LOCAL_MACHINE, _TEXT("Software\\Microsoft\\Internet Explorer\\MAIN\\EnterpriseMode"), KEY_READ);
	if (ret == ERROR_SUCCESS)
	{
		count = 255;
		memset(lpszValue, 0, 255);
		key.QueryStringValue(_TEXT("SiteList"), lpszValue, &count);
		m_data.m_InternetExplorer.m_SiteList1 = lpszValue;
	}
	key.Close();

	ret = key.Open(HKEY_CURRENT_USER, _TEXT("Software\\Microsoft\\Internet Explorer\\MAIN\\EnterpriseMode"), KEY_READ);
	if (ret == ERROR_SUCCESS)
	{
		count = 255;
		memset(lpszValue, 0, 255);
		key.QueryStringValue(_TEXT("SiteList"), lpszValue, &count);
		m_data.m_InternetExplorer.m_SiteList2 = lpszValue;
	}
	key.Close();

	ret = key.Open(HKEY_LOCAL_MACHINE, _TEXT("Software\\Microsoft\\Internet Explorer"), KEY_READ);
	if (ret == ERROR_SUCCESS)
	{
		count = 255;
		memset(lpszValue, 0, 255);
		key.QueryStringValue(_TEXT("svcUpdateVersion"), lpszValue, &count);
		m_data.m_InternetExplorer.m_UpdateVersion = lpszValue;

		count = 255;
		memset(lpszValue, 0, 255);
		key.QueryStringValue(_TEXT("svcKBNumber"), lpszValue, &count);
		m_data.m_InternetExplorer.m_KBNumber = lpszValue;
	}
	key.Close();

	return true;
}

bool WorkerItemHandler::GetAltirisPackagesInformation()
{
	bool bRet = false;
	bRet = GetAltirisPackagesInformation(false);
	bRet = bRet && GetAltirisPackagesInformation(true);
	return bRet;
}

bool WorkerItemHandler::GetAltirisPackagesInformation(bool b64bits)
{
	TCHAR lpszValue[255];
	memset(lpszValue, 0, 255);
	CRegKey key;
	LONG ret = 0;
	ULONG c = 255;
	if (b64bits == false)
	{
		ret = key.Open(HKEY_LOCAL_MACHINE, _TEXT("SOFTWARE\\LCL_MSI_Application"), KEY_READ | KEY_WOW64_32KEY);
	}
	else
	{
		ret = key.Open(HKEY_LOCAL_MACHINE, _TEXT("SOFTWARE\\LCL_MSI_Application"), KEY_READ);
	}

	if (ret == ERROR_SUCCESS)
	{
		DWORD index = 0;
		while (TRUE)
		{
			DWORD dwLen = 255;
			TCHAR lpszKeyName[255];
			memset(lpszKeyName, 0, 255);
			FILETIME ft;
			memset(&ft, 0, sizeof(FILETIME));
			long lRet = key.EnumKey(index, lpszKeyName, &dwLen, &ft);
			index++;

			if (lRet == ERROR_SUCCESS)
			{
				CAltirisPackage package;
				package.m_b64bits = b64bits;
				package.m_Name = lpszKeyName;
				package.m_Date = ft;
				m_data.m_AltirisPackages.Add(package);
			}

			if (lRet == ERROR_NO_MORE_ITEMS)
				break;
		}
		key.Close();
	}

	return true;
}

bool WorkerItemHandler::GetApplicationsInformation()
{
	bool bRet = false;
	bRet = GetApplicationsInformation(false);
	bRet = bRet && GetApplicationsInformation(true);
	return bRet;
}

bool WorkerItemHandler::GetApplicationsInformation(bool b64bits)
{
	TCHAR lpszValue[255];
	memset(lpszValue, 0, 255);
	CRegKey key;
	LONG ret = 0;
	ULONG c = 255;
	if (b64bits == false)
	{
		ret = key.Open(HKEY_LOCAL_MACHINE, _TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), KEY_READ | KEY_WOW64_32KEY);
	}
	else
	{
		ret = key.Open(HKEY_LOCAL_MACHINE, _TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), KEY_READ);
	}

	if (ret == ERROR_SUCCESS)
	{
		DWORD index = 0;
		while (TRUE)
		{
			DWORD dwLen = 255;
			TCHAR lpszKeyName[255];
			memset(lpszKeyName, 0, 255);
			FILETIME ft;
			memset(&ft, 0, sizeof(FILETIME));
			long lRet = key.EnumKey(index, lpszKeyName, &dwLen, &ft);
			index++;

			if (lRet == ERROR_SUCCESS)
			{
				TCHAR lpszValue[255];
				CRegKey keyApp;
				LONG lRet2;
				lRet2 = keyApp.Open(key, lpszKeyName, KEY_READ);
				if (lRet2 == ERROR_SUCCESS)
				{
					CApplication app;

					memset(lpszValue, 0, 255);
					c = 255;
					lRet2 = keyApp.QueryStringValue(_TEXT("DisplayName"), lpszValue, &c);
					if (lRet2 != ERROR_SUCCESS)
					{
						keyApp.Close();
						continue;
					}

					app.m_Name = lpszValue;

					memset(lpszValue, 0, 255);
					c = 255;
					keyApp.QueryStringValue(_TEXT("DisplayVersion"), lpszValue, &c);
					app.m_Version = lpszValue;

					memset(lpszValue, 0, 255);
					c = 255;
					keyApp.QueryStringValue(_TEXT("InstallDate"), lpszValue, &c);
					app.m_InstallDate = lpszValue;

					m_data.m_Applications.Add(app);
				}
				keyApp.Close();
			}

			if (lRet == ERROR_NO_MORE_ITEMS)
				break;
		}
		key.Close();
	}

	return true;
}

CString WorkerItemHandler::GetNetworkAdapterInformation2()
{
	DataHandler data;
	CString strIP;
	strIP.Empty();

	ManagementClass mc(_TEXT("Win32_NetworkAdapter"));
	bool result = mc.GetInstances();
	while (mc.MoveNext())
	{
		CNetworkCard card;
		card.m_Name = mc.GetStringProperty(_TEXT("Name"));
		card.m_MACAddress = mc.GetStringProperty(_TEXT("MACAddress"));
		card.m_Speed = mc.GetStringProperty(_TEXT("Speed"));
		data.m_NetworkCards.Add(card);
	}

	ManagementClass mc2(_TEXT("Win32_NetworkAdapterConfiguration"));
	result = mc2.GetInstances();
	while (mc2.MoveNext())
	{
		COleSafeArray sa;
		if (mc2.GetSafeArrayProperty(_TEXT("IPAddress"), sa) == false)
		{
			continue;
		}
		else
		{
			//DWORD dwDim = sa.GetDim();
			//LONG lBound = 0;
			//sa.GetLBound(1, &lBound);
			//LONG uBound = 0;
			//sa.GetUBound(1, &uBound);
			//long index[1];
			//index[0] = lBound;
			//COleVariant vData;
			//sa.GetElement(index, (void *)&vData);
			data.m_IPV4 = (((wchar_t* *)(*(((tagVARIANT*)(&(sa)))->parray)).pvData))[0];
			data.m_IPV6 = (((wchar_t* *)(*(((tagVARIANT*)(&(sa)))->parray)).pvData))[1];

			strIP = data.m_IPV4;
			_tprintf(_T("IPV4=%s\n"), (LPCTSTR)data.m_IPV4);
			_tprintf(_T("IPV6=%s\n"), (LPCTSTR)data.m_IPV6);
		}
	}

	return strIP;
}
