// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <tchar.h>

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <random>
#include <memory>
#include <map>

#include <atlstr.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlcomcli.h>
#include <atlsync.h>

#include "cpprest/json.h"
#include "cpprest/http_listener.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/http_client.h"
#include "cpprest/http_msg.h"

//#include <afxwin.h>

//#include <Windows.h>
#include <winsvc.h>

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

#include "..\Include\LMDBWrapper.h"
#include "..\Include\CWrapper.h"

#include "CSWMRGuard.h"
extern CSWMRGuard g_Guard;

#include "Logger.h"
#include "BasicLogger.h"
//extern CLogger g_Logger;
extern BasicLogger g_Logger;

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace http::client;
using namespace web::http;
using namespace web::http::experimental::listener;

// TODO: reference additional headers your program requires here

#ifdef _WIN64
	#ifdef _DEBUG
		//#pragma comment (lib, "..\\Lib\\LMDBWindowsDllD64.lib")
		#pragma comment (lib, "..\\Lib\\cpprest141d_2_10.lib")
		#pragma comment (lib, "..\\Lib\\MySharedStuffD64.lib")
		#pragma comment (lib, "..\\Lib\\LMDBWrapperD64.lib")
#else
		#pragma comment (lib, "..\\Lib\\LMDBWindowsDll64.lib")
		#pragma comment (lib, "..\\Lib\\cpprest141_2_10.lib")
		#pragma comment (lib, "..\\Lib\\MySharedStuff64.lib")
		#pragma comment (lib, "..\\Lib\\LMDBWrapper64.lib")
#endif
#else
	#ifdef _DEBUG
		#pragma comment (lib, "..\\Lib\\LMDBWindowsDllD.lib")
		#pragma comment (lib, "..\\Lib\\cpprest141d_2_10.lib")
	#else
		#pragma comment (lib, "..\\Lib\\LMDBWindowsDll.lib")
		#pragma comment (lib, "..\\Lib\\cpprest141_2_10.lib")
	#endif
#endif
