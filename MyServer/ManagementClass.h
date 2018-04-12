#pragma once

class ManagementClass
{
public:
	ManagementClass(const CString& adr);
	virtual ~ManagementClass();

	bool GetInstances();
	bool MoveNext();
	CString GetStringProperty(CString propName);
	bool GetBoolProperty(CString propName);
	int GetIntProperty(CString propName);
	long GetLongProperty(CString propName);
	bool GetSafeArrayProperty(CString propName, COleSafeArray & saReturned);

protected:
	CString m_wmiAdr;
	IWbemLocator *m_pLoc;
	CComPtr<IWbemServices> m_pSvc;
	CComPtr<IEnumWbemClassObject> m_pEnumerator;
	CComPtr<IWbemClassObject> m_pclsObj;
};
