/***
* Copyright (C) Microsoft. All rights reserved.
* Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
*
* =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
*
* stdafx.h : include file for standard system include files,
* or project specific include files that are used frequently, but
* are changed infrequently
*
* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
****/

#pragma once

#ifdef _WIN32
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define NOMINMAX
#include <windows.h>
#include <objbase.h>
#include <winsock2.h>

// ws2tcpip.h - isn't warning clean.
#pragma warning(push)
#pragma warning(disable : 6386)
#include <ws2tcpip.h>
#pragma warning(pop)

#include <iphlpapi.h>
#endif

#include <map>
#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <streambuf>
#include <sstream>
#include <fstream>

#include "cpprest/http_client.h"
#include "cpprest/http_msg.h"

#include "MyServer/messagetypes.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace http::client;

#ifdef _WIN64
	#ifdef _DEBUG
		#pragma comment (lib, "..\\Lib\\LMDBWindowsDllD64.lib")
		#pragma comment (lib, "..\\Lib\\cpprest141d_2_10.lib")
	#else
		#pragma comment (lib, "..\\Lib\\LMDBWindowsDll64.lib")
		#pragma comment (lib, "..\\Lib\\cpprest141_2_10.lib")
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