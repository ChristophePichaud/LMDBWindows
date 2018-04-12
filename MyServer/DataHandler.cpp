#include "stdafx.h"
#include "DataHandler.h"


DataHandler::DataHandler()
{
}

DataHandler::~DataHandler()
{
}

void DataHandler::CopySimple(const DataHandler & dh)
{
	this->m_dwTimeForCollectingAllData = dh.m_dwTimeForCollectingAllData;
	this->m_ComputerName = dh.m_ComputerName;
	this->m_ComputerDescription = dh.m_ComputerDescription;
	this->m_Env.Copy(dh.m_Env);
	this->m_NumOfProcessors = dh.m_NumOfProcessors;
	this->m_ProcessorArchitecture = dh.m_ProcessorArchitecture;
	this->m_Version = dh.m_Version;
	this->m_OperatingSystem = dh.m_OperatingSystem;
	this->m_BiosDescription = dh.m_BiosDescription;
	this->m_BiosManufacturer = dh.m_BiosManufacturer;
	this->m_BiosSerialNumber = dh.m_BiosSerialNumber;
	this->m_BiosVersion = dh.m_BiosVersion;
	this->m_Printers.Copy(dh.m_Printers);
	this->m_Usbs.Copy(dh.m_Usbs);
	this->m_Videos.Copy(dh.m_Videos);
	this->m_Disks.Copy(dh.m_Disks);
	this->m_DiskDrive.Copy(dh.m_DiskDrive);
	this->m_DiskLogical.Copy(dh.m_DiskLogical);
	this->m_Model = dh.m_Model;
	this->m_TotalMemory = dh.m_TotalMemory;
	this->m_Processor = dh.m_Processor;
	this->m_NetworkCards.Copy(dh.m_NetworkCards);
	this->m_IPV4 = dh.m_IPV4;
	this->m_IPV6 = dh.m_IPV6;
	this->m_Workstation.m_AdmMachine = dh.m_Workstation.m_AdmMachine;
	this->m_Workstation.m_AgenceStation = dh.m_Workstation.m_AgenceStation;
	this->m_Workstation.m_Idmsr = dh.m_Workstation.m_Idmsr;
	this->m_Workstation.m_VersionOS = dh.m_Workstation.m_VersionOS;
	this->m_Workstation.m_VersionOSString = dh.m_Workstation.m_VersionOSString;
	this->m_Workstation.m_TypePoste = dh.m_Workstation.m_TypePoste;
	this->m_Workstation.m_ComputerName = dh.m_Workstation.m_ComputerName;
	this->m_Workstation.m_CurrentUser = dh.m_Workstation.m_CurrentUser;
	this->m_Workstation.m_UserDomain = dh.m_Workstation.m_UserDomain;
	this->m_Workstation.m_Registry.m_Abonne = dh.m_Workstation.m_Registry.m_Abonne;
	this->m_Workstation.m_Registry.m_DateAbonne = dh.m_Workstation.m_Registry.m_DateAbonne;
	this->m_Workstation.m_Registry.m_DateInst = dh.m_Workstation.m_Registry.m_DateInst;
	this->m_Workstation.m_Registry.m_DateSocle = dh.m_Workstation.m_Registry.m_DateSocle;
	this->m_Workstation.m_Registry.m_InstPlateforme = dh.m_Workstation.m_Registry.m_InstPlateforme;
	this->m_Workstation.m_Registry.m_LCLAppABOPDT = dh.m_Workstation.m_Registry.m_LCLAppABOPDT;
	this->m_Workstation.m_Registry.m_LCLAppMDTABOPDT = dh.m_Workstation.m_Registry.m_LCLAppMDTABOPDT;
	this->m_Workstation.m_Registry.m_LCLAppMetierPDT = dh.m_Workstation.m_Registry.m_LCLAppMetierPDT;
	this->m_Workstation.m_Registry.m_LCLMediaVersion = dh.m_Workstation.m_Registry.m_LCLMediaVersion;
	this->m_Workstation.m_Registry.m_LCLMIG = dh.m_Workstation.m_Registry.m_LCLMIG;
	this->m_Workstation.m_Registry.m_PDT = dh.m_Workstation.m_Registry.m_PDT;
	this->m_Workstation.m_Registry.m_Type = dh.m_Workstation.m_Registry.m_Type;
	this->m_Workstation.m_Registry.m_TypeMaster = dh.m_Workstation.m_Registry.m_TypeMaster;
	this->m_Workstation.m_Registry.m_TypePoste = dh.m_Workstation.m_Registry.m_TypePoste;
	this->m_Workstation.m_Registry.m_VersionAbonne = dh.m_Workstation.m_Registry.m_VersionAbonne;
	this->m_Workstation.m_Registry.m_VersionProc = dh.m_Workstation.m_Registry.m_VersionProc;
	this->m_Workstation.m_Registry.m_VersionSocle = dh.m_Workstation.m_Registry.m_VersionSocle;
	this->m_Workstation.m_Registry.m_VersionVia = dh.m_Workstation.m_Registry.m_VersionVia;
	this->m_Workstation.m_RegistryLCL.Copy(dh.m_Workstation.m_RegistryLCL);
	this->m_Workstation.m_Planet = dh.m_Workstation.m_Planet;
	this->m_Workstation.m_Identifiant = dh.m_Workstation.m_Identifiant;
	this->m_Workstation.m_Type = dh.m_Workstation.m_Type;
	this->m_Workstation.m_NomAbonne = dh.m_Workstation.m_NomAbonne;
	this->m_Workstation.m_NomAbonneR = dh.m_Workstation.m_NomAbonneR;
	this->m_Workstation.m_VersionAbonne = dh.m_Workstation.m_VersionAbonne;
	this->m_Workstation.m_Metier = dh.m_Workstation.m_Metier;
	this->m_Workstation.m_Entite = dh.m_Workstation.m_Entite;
	this->m_Workstation.m_NumPDTQualiparc = dh.m_Workstation.m_NumPDTQualiparc;
	this->m_Workstation.m_NumUCQualiparc = dh.m_Workstation.m_NumUCQualiparc;
	this->m_InternetExplorer.m_ClearCache = dh.m_InternetExplorer.m_ClearCache;
	this->m_InternetExplorer.m_ProxyExceptions = dh.m_InternetExplorer.m_ProxyExceptions;
	this->m_InternetExplorer.m_ProxyPac = dh.m_InternetExplorer.m_ProxyPac;
	this->m_InternetExplorer.m_ProxyServer = dh.m_InternetExplorer.m_ProxyServer;
	this->m_InternetExplorer.m_ProxyServerEnabled = dh.m_InternetExplorer.m_ProxyServerEnabled;
	this->m_InternetExplorer.m_Version = dh.m_InternetExplorer.m_Version;
	this->m_InternetExplorer.m_SiteList1 = dh.m_InternetExplorer.m_SiteList1;
	this->m_InternetExplorer.m_SiteList2 = dh.m_InternetExplorer.m_SiteList2;
	this->m_InternetExplorer.m_CurrentVersion = dh.m_InternetExplorer.m_CurrentVersion;
	this->m_InternetExplorer.m_UpdateVersion = dh.m_InternetExplorer.m_UpdateVersion;
	this->m_InternetExplorer.m_KBNumber = dh.m_InternetExplorer.m_KBNumber;
	this->m_AltirisPackages.Copy(dh.m_AltirisPackages);
	this->m_Applications.Copy(dh.m_Applications);
}

void DataHandler::CopyQFE(const DataHandler & dh)
{
	this->m_Qfes.Copy(dh.m_Qfes);
}
