// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <tchar.h>
#include <Windows.h>

#include <stdio.h>
#include <tchar.h>

#include <string>
#include <iostream>

#include <atlconv.h>

#include "HttpLMDB.h"

#ifdef _WIN64
	#ifdef _DEBUG
		#pragma comment (lib, "..\\Lib\\HttpLMDBD64.lib")
		#pragma comment (lib, "..\\Lib\\MySharedStuffD64.lib")
	#else
		#pragma comment (lib, "..\\Lib\\HttpLMDB64.lib")
		#pragma comment (lib, "..\\Lib\\MySharedStuff64.lib")
	#endif
#endif
