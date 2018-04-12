#pragma once

class WorkerItemHandler
{
public:
	WorkerItemHandler();
	~WorkerItemHandler();

	bool GetComputerName();
	bool GetEnvironmentStrings();
	bool GetSystemInformation();
	bool GetWMIInformation2();
	bool GetBiosInformation();
	bool GetPrinterInformation();
	bool GetPrinterInformation2();
	bool GetVideoInformation();
	bool GetDiskDriveInformation();
	bool GetModelInformation();
	bool GetNetworkAdapterInformation();
	bool GetQFEInformation();
	bool GetWorkstationInformation();
	bool GetInternetExplorerInformation();
	bool GetAltirisPackagesInformation();
	bool GetAltirisPackagesInformation(bool b64bits);
	bool GetApplicationsInformation();
	bool GetApplicationsInformation(bool b64bits);

	CString GetNetworkAdapterInformation2();

public:
	DataHandler m_data;
};

