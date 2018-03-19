// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef STATIC
#include "lmdb.h"
#pragma comment (lib, "LMDBWindows.lib")
#pragma comment (lib, "ntdll.lib")
#endif

#include "..\Include\LMDBWindowsDll.h" 
#include "..\Include\lmdb-dll.h"
#include "..\Include\midl-dll.h"

#pragma comment (lib, "..\\Lib\\LMDBWindowsDll.lib")

// TODO: reference additional headers your program requires here
