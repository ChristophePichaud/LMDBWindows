/***
* Copyright (C) Microsoft. All rights reserved.
* Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
*
* =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
*
* stdafx.h : include file for standard system include files,
* or project specific include files that are used frequently,
* but are changed infrequently
*
* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
****/


#pragma once
#include <tchar.h>

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <random>

#include "cpprest/json.h"
#include "cpprest/http_listener.h"
#include "cpprest/uri.h"
#include "cpprest/asyncrt_utils.h"

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#else
# include <sys/time.h>
#endif

#include "..\Include\LMDBWindowsDll.h" 
#include "..\Include\lmdb-dll.h"
#include "..\Include\midl-dll.h"

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
