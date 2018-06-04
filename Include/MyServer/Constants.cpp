#include <tchar.h>
#include <string>
#include "..\Include\MyServer\Constants.h"

const std::wstring Constants::MasterNodePort	= _T("7001");
const std::wstring Constants::Request			= _T("?request=");
const std::wstring Constants::VerbPing			= _T("ping");
const std::wstring Constants::VerbGetData		= _T("get-data");
const std::wstring Constants::VerbSetData		= _T("set-data");
const std::wstring Constants::VerbRegisterNode	= _T("register-node");
const std::wstring Constants::VerbShowNodes		= _T("show-nodes");
const std::wstring Constants::VerbGetNode		= _T("get-node");
const std::wstring Constants::VerbSetNode		= _T("set-node");
const std::wstring Constants::VerbReleaseNode	= _T("release-node");
const std::wstring Constants::VerbReleaseDB		= _T("release-db");
const std::string Constants::LMDBRootPath		= "c:\\temp";
const std::wstring Constants::StatusOK			= _T("OK");
const std::wstring Constants::ModeMasterNode	= _T("master");
const std::wstring Constants::ModeWorkerNode	= _T("node");

