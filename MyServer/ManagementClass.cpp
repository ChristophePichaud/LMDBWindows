#include "stdafx.h"
#include "ManagementClass.h"


ManagementClass::ManagementClass(const CString& adr)
{
	m_wmiAdr.Format(_TEXT("SELECT * From %s"), adr);
	m_pLoc = nullptr;
	m_pEnumerator = nullptr;
	m_pclsObj = nullptr;
}


ManagementClass::~ManagementClass()
{
	if (m_pLoc)
	{
		m_pLoc->Release();
		m_pLoc = nullptr;
	}
}

bool ManagementClass::GetInstances()
{
	static bool bSecurity = false;

	HRESULT hres;

	if (bSecurity == false)
	{
		bSecurity = true;

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
	}

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&m_pLoc);


	if (FAILED(hres))
	{
		return false;
	}

	// Connect to WMI through the IWbemLocator::ConnectServer method
	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = m_pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (e.g. Kerberos)
		0,                       // Context object 
		&m_pSvc                    // pointer to IWbemServices proxy
	);

	if (FAILED(hres))
	{
		m_pLoc->Release();
		return false;                // Program has failed.
	}

	hres = CoSetProxyBlanket(
		m_pSvc,                        // Indicates the proxy to set
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
		m_pLoc->Release();
		return false;
	}

	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	//IEnumWbemClassObject* pEnumerator = NULL;
	//CComPtr<IEnumWbemClassObject> pEnumerator = NULL;
	hres = m_pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t((LPTSTR)(LPCTSTR)m_wmiAdr),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&m_pEnumerator);

	if (m_pEnumerator == NULL)
	{
		return false;
	}

	if (FAILED(hres))
	{
		m_pLoc->Release();
		return false;
	}

	return true;
}

bool ManagementClass::MoveNext()
{
	ULONG uReturn = 0;
	if (m_pEnumerator)
	{
		CComPtr<IWbemClassObject> pclsObj = nullptr;
		HRESULT hr = m_pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
		if (hr == S_FALSE)
		{
			return false;
		}

		m_pclsObj = pclsObj;
	}
	else
	{
		return false;
	}

	return true;
}

CString ManagementClass::GetStringProperty(CString propName)
{
	ASSERT(m_pclsObj != NULL);
	CComVariant vtProp;
	HRESULT hr = m_pclsObj->Get(propName, 0, &vtProp, 0, 0);
	CString data;
	AfxBSTR2CString(&data, vtProp.bstrVal);
	return data;
}

bool ManagementClass::GetSafeArrayProperty(CString propName, COleSafeArray & saReturned)
{
	ASSERT(m_pclsObj != NULL);
	//CComVariant vtProp;
	COleVariant vtProp;
	HRESULT hr = m_pclsObj->Get(propName, 0, &vtProp, 0, 0);
	if (vtProp.parray == NULL)
	{
		return false;
	}

	COleSafeArray sa;
	sa = vtProp;
	saReturned = sa;
	return true;
}

bool ManagementClass::GetBoolProperty(CString propName)
{
	ASSERT(m_pclsObj != NULL);
	bool result = false;
	CComVariant vtProp;
	HRESULT hr = m_pclsObj->Get(propName, 0, &vtProp, 0, 0);
	if (vtProp.boolVal == 1)
		result = true;
	return result;
}

int ManagementClass::GetIntProperty(CString propName)
{
	ASSERT(m_pclsObj != NULL);
	CComVariant vtProp;
	HRESULT hr = m_pclsObj->Get(propName, 0, &vtProp, 0, 0);
	int i = vtProp.iVal;
	return i;
}

long ManagementClass::GetLongProperty(CString propName)
{
	ASSERT(m_pclsObj != NULL);
	CComVariant vtProp;
	HRESULT hr = m_pclsObj->Get(propName, 0, &vtProp, 0, 0);
	long l = vtProp.lVal;
	return l;
}

