// MySharedStuff.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MySharedStuff.h"


// This is an example of an exported variable
MYSHAREDSTUFF_API int nMySharedStuff=0;

// This is an example of an exported function.
MYSHAREDSTUFF_API int fnMySharedStuff(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see MySharedStuff.h for the class definition
CMySharedStuff::CMySharedStuff()
{
    return;
}
