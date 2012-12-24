// b64Encoding.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "b64encode.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

	BYTE *b64Table = (BYTE*)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	BYTE *bSrc = (BYTE*)"ABCDEFGHIG";
	BYTE *bSrc2;
	DWORD dwRes = 0;
	LPVOID lpRes;

	LPTSTR lptSourceName = TEXT("test.txt");

	LPTSTR lptDestName = TEXT("testencoded.txt");
	
	LPTSTR lptDest2 = TEXT("testdecoded.txt");
	
	b64EncodeFile(lptSourceName,lptDestName);

	b64DecodeFile(lptDestName,lptDest2);
	
	/*b64EncodeBlock(bSrc,&dwRes, b64Table,true);
	
	

	//Base64_code(S,lpszOutString);

	
	ZeroMemory(lpszOutString,20);
	memcpy(lpszOutString,&dwRes,4);
	MessageBoxA(NULL,lpszOutString,"b64",MB_OK);
	
	BOOL bFlag=0;

	b64DecodeBlock(&dwRes,bRes,b64Table,&bFlag);
	ZeroMemory(lpszOutString,20);
	memcpy(lpszOutString,&bRes,4);*/

	/*b64EncodeMessage(bSrc,(DWORD)strlen((CHAR*)bSrc),(LPBYTE)&lpRes,&dwRes);

	MessageBoxA(NULL,(CHAR*)lpRes,"b64",MB_OK);
	
	

	bSrc2 = new BYTE[dwRes];
	ZeroMemory(bSrc2,dwRes);
	CopyMemory((LPBYTE)bSrc2,(LPBYTE)lpRes,dwRes);
	LocalFree(lpRes);


	b64DecodeMessage(bSrc2,dwRes,(LPBYTE)&lpRes,&dwRes);

	MessageBoxA(NULL,(CHAR*)lpRes,"b64",MB_OK);

	LocalFree(lpRes);*/

	
}