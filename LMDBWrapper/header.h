// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // remove support for MFC controls in dialogs

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include <map>
#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <streambuf>
#include <sstream>
#include <fstream>

// Windows Header Files:
#include <tchar.h>
#include <windows.h>

#include "..\Include\LMDBWindowsDll.h" 
#include "..\Include\lmdb-dll.h"
#include "..\Include\midl-dll.h"

#include "cpprest/json.h"
#include "cpprest/http_listener.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/http_client.h"
#include "cpprest/http_msg.h"

#include <atlsync.h>

#include "Logger.h"
#include "BasicLogger.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace http::client;
using namespace web::http::experimental::listener;


#ifdef _WIN64
	#ifdef _DEBUG
		#pragma comment (lib, "..\\Lib\\LMDBWindowsDllD64.lib")
		#pragma comment (lib, "..\\Lib\\cpprest141d_2_10.lib")
		#pragma comment (lib, "..\\Lib\\MySharedStuffD64.lib")
	#else
		#pragma comment (lib, "..\\Lib\\LMDBWindowsDll64.lib")
		#pragma comment (lib, "..\\Lib\\cpprest141_2_10.lib")
		#pragma comment (lib, "..\\Lib\\MySharedStuff64.lib")
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
