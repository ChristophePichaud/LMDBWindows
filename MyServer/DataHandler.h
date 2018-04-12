#pragma once

class CQFE
{
public:
	CString m_Caption;
	CString m_Description;
	CString m_HotFixID;
	CString m_InstalledOn;
};

class CDiskPartition
{
public:
	CString m_Antecedent;
	CString m_Dependent;
};

class CDiskDrive
{
public:
	CString m_Name;
	int m_Partitions;
	CString m_Model;
	CString m_FirmwareRevision;
	CString m_SerialNumber;
	long m_Size;
	CString m_InterfaceType;
	CString m_MediaType;
};

class CVideo
{
public:
	CString m_Description;
	CString m_DriverVersion;
	long m_ErrorCode;
};

class CUsb
{
public:
	CString m_DeviceID;
};

class CPrinter
{
public:
	CString m_Name;
	bool m_bDefault;
	CString m_DriverName;
	bool m_EnableBIDI;
	bool m_bNetwork;
};

class CNetworkCard
{
public:
	CString m_Name;
	CString m_MACAddress;
	CString m_Speed;
};

class CRegKeyValue
{
public:
	CString m_Abonne;
	CString m_DateAbonne;
	CString m_DateInst;
	CString m_DateSocle;
	CString m_InstPlateforme;
	CString m_LCLAppABOPDT;
	CString m_LCLAppMDTABOPDT;
	CString m_LCLAppMetierPDT;
	CString m_LCLMediaVersion;
	CString m_LCLMIG;
	CString m_PDT;
	CString m_Type;
	CString m_TypeMaster;
	CString m_TypePoste;
	CString m_VersionAbonne;
	CString m_VersionProc;
	CString m_VersionSocle;
	CString m_VersionVia;
};

class CValueNameValue
{
public:
	CString m_NameValue;
	CString m_Value;
};

class CWorkstation
{
public:
	CString m_AdmMachine;
	CString m_AgenceStation;
	CString m_Idmsr;

	CString m_VersionOS;
	CString m_VersionOSString;

	CString m_TypePoste;
	CString m_ComputerName;
	CString m_CurrentUser;
	CString m_UserDomain;

	CRegKeyValue m_Registry;
	CArray<CValueNameValue> m_RegistryLCL;

	CString m_Planet;
	CString m_Identifiant;
	CString m_Type;
	CString m_NomAbonne;
	CString m_NomAbonneR;
	CString m_VersionAbonne;
	CString m_Metier;
	CString m_Entite;
	CString m_NumPDTQualiparc;
	CString m_NumUCQualiparc;
};

class CInternetExplorer
{
public:
	CString m_ClearCache;
	CString m_ProxyExceptions;
	CString m_ProxyPac;
	CString m_ProxyServer;
	CString m_ProxyServerEnabled;
	CString m_Version;
	CString m_SiteList1;
	CString m_SiteList2;
	CString m_CurrentVersion;
	CString m_UpdateVersion;
	CString m_KBNumber;
};

class CAltirisPackage
{
public:
	CString m_Name;
	bool m_b64bits;
	CTime m_Date;
};

class CApplication
{
public:
	CString m_Name;
	CString m_Version;
	CString m_InstallDate;
};

class DataHandler
{
public:
	DataHandler();
	~DataHandler();

	void CopySimple(const DataHandler & dh);
	void CopyQFE(const DataHandler & dh);

public:
	DWORD m_dwTimeForCollectingAllData;
	CString m_ComputerName;
	CString m_ComputerDescription;
	CArray<CString> m_Env;
	int m_NumOfProcessors;
	CString m_ProcessorArchitecture;
	CString m_Version;
	CString m_OperatingSystem;
	CString m_BiosDescription;
	CString m_BiosManufacturer;
	CString m_BiosSerialNumber;
	CString m_BiosVersion;
	CArray<CPrinter> m_Printers;
	CArray<CUsb> m_Usbs;
	CArray<CVideo> m_Videos;
	CArray<CDiskDrive> m_Disks;
	CArray<CDiskPartition> m_DiskDrive;
	CArray<CDiskPartition> m_DiskLogical;
	CString m_Model;
	CString m_TotalMemory;
	CString m_Processor;
	CArray<CQFE> m_Qfes;
	CArray<CNetworkCard> m_NetworkCards;
	CString m_IPV4;
	CString m_IPV6;
	CArray<CAltirisPackage> m_AltirisPackages;
	CArray<CApplication> m_Applications;

	//
	// Business Data
	//
	CWorkstation m_Workstation;
	CInternetExplorer m_InternetExplorer;
};

