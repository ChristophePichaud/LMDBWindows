// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

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

#include <fcntl.h>
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>

#include "..\Include\LMDBWrapper.h"

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment (lib, "..\\Lib\\LMDBWrapperD64.lib")
#else
#pragma comment (lib, "..\\Lib\\LMDBWrapper64.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment (lib, "..\\Lib\\LMDBWrapper.lib")
#else
#pragma comment (lib, "..\\Lib\\LMDBWrapper.lib")
#endif
#endif




// TODO: reference additional headers your program requires here
