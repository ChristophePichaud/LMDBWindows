// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <tchar.h>

#include <string>
#include <iostream>

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

#include "cpprest/json.h"
#include "cpprest/http_listener.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/http_client.h"
#include "cpprest/http_msg.h"

#include <atlstr.h>

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace http::client;

// TODO: reference additional headers your program requires here
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