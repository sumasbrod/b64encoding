// b64EncodingDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#pragma comment(linker, "/ENTRY:DllMain") 
//#pragma comment(linker, "/SECTION:.text,EWR") 
#pragma comment(linker, "/MERGE:.rdata=.data") 
#pragma comment(linker, "/MERGE:.text=.data")

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

