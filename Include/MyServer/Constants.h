#pragma once

/*
#define _MASTER_NODE_PORT_		_T("7001")
#define _REQUEST_				_T("?request=")
#define _VERB_PING_				_T("ping")
#define _VERB_GET_DATA_			_T("get-data")
#define _VERB_SET_DATA_			_T("set-data")
#define _VERB_REGISTER_NODE_	_T("register-node")
#define _VERB_SHOW_NODE_		_T("show-nodes")
#define _VERB_GET_NODE_			_T("get-node")
#define _VERB_SET_NODE_			_T("set-node")
#define _LMDB_ROOT_PATH_		"c:\\temp"
#define _STATUS_OK_				_T("OK")
#define _MODE_MASTER_NODE_		_T("master")
#define _MODE_WORKER_NODE_		_T("node")
*/

class Constants
{
public:
	static const std::wstring MasterNodePort;
	static const std::wstring Request;
	static const std::wstring VerbPing;
	static const std::wstring VerbGetData;
	static const std::wstring VerbSetData;
	static const std::wstring VerbRegisterNode;
	static const std::wstring VerbShowNodes;
	static const std::wstring VerbGetNode;
	static const std::wstring VerbSetNode;
	static const std::string LMDBRootPath;
	static const std::wstring StatusOK;
	static const std::wstring ModeMasterNode;
	static const std::wstring ModeWorkerNode;
};

