#include "stdafx.h"
#include "b64encode.h"

__DLLEXPORT int WINAPI b64EncodeMessage(BYTE *bSrc, DWORD dwSrcSize, BYTE *bRes, DWORD *dwResSize)
{
	BOOL bFlag = 0;
	DWORD dwTmpSrcSize = 0;
	DWORD i = 0;
	BYTE *bTmpRes;
	BYTE *b64Table = (BYTE*)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	
	*dwResSize = 0;
	
	bTmpRes = (LPBYTE)LocalAlloc(LPTR,dwSrcSize*2);
	if (bTmpRes == NULL) return 0;
	
	do {
		dwTmpSrcSize += 3;
		*dwResSize += 4;
		if (dwTmpSrcSize>=dwSrcSize) bFlag = TRUE;
		b64EncodeBlock(bSrc+i*3,(DWORD*)bTmpRes+i,b64Table,bFlag);
		i++;
	} while (dwTmpSrcSize<=dwSrcSize);
	CopyMemory(bRes,&bTmpRes,4);
	return 1;
}

__DLLEXPORT int WINAPI b64EncodeFile(TCHAR *lptSourceFile,TCHAR *lptDestFile)
{
	HANDLE hSource, hDest;
	BOOL bFlag = 0;
	DWORD dwTmpSize = 0;
	DWORD dwResSize = 0;
	DWORD i = 0;
	DWORD dwFileSize = 0;
	DWORD dwTmpFileSize = 0;
	BYTE *bOutputBuffer, *bInputBuffer;
	DWORD dwRead, dwWrite;
	BYTE *b64Table = (BYTE*)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	bOutputBuffer = (LPBYTE)LocalAlloc(LPTR,0x2000);
	if (bOutputBuffer == NULL) return 0;

	bInputBuffer = (LPBYTE)LocalAlloc(LPTR,0x2000);
	if (bInputBuffer == NULL) return 0;

	hSource = CreateFile(lptSourceFile,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	if (hSource == INVALID_HANDLE_VALUE) return 0;
	hDest = CreateFile(lptDestFile,GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);
	if (hDest == INVALID_HANDLE_VALUE) return 0;

	dwFileSize = GetFileSize(hSource,0);

	while (ReadFile(hSource,bInputBuffer,0xfff,&dwRead,0)!=0 && dwRead!=0) {
		dwTmpSize = 0;
		dwResSize = 0;
		i=0;
		do {
			dwTmpSize += 3;
			dwResSize += 4;
			dwTmpFileSize +=3;

			if (dwTmpFileSize>=dwFileSize) bFlag = TRUE;

			b64EncodeBlock(bInputBuffer+i*3,(DWORD*)bOutputBuffer+i,b64Table,bFlag);
			if (bFlag) {
				if (!(bOutputBuffer[i*4-1] | 0x00)) dwResSize-=1;
				if (!(bOutputBuffer[i*4-2] | 0x00)) dwResSize-=1;
				if (!(bOutputBuffer[i*4-3] | 0x00)) dwResSize-=1;
			}
			
			i++;
		} while(dwTmpSize<dwRead);
		WriteFile(hDest,bOutputBuffer,dwResSize,&dwWrite,0);
	}
	ZeroMemory(bOutputBuffer,0x2000);
	ZeroMemory(bInputBuffer,0x2000);		
	LocalFree(bInputBuffer);
	LocalFree(bOutputBuffer);
	CloseHandle(hDest);
	CloseHandle(hSource);
	return 1;
}

__DLLEXPORT int WINAPI b64EncodeBlock(BYTE *bIn, DWORD *dwout,BYTE *b64Table, BOOL bFlag)
{
	__asm {
		push eax
		push ebx
		push ecx
		push edx
		
		mov eax,bIn
		xor edx,edx
		mov dl,[eax]
		shl edx,8
		or dl,[eax+1]
		shl edx,8
		or dl,[eax+2]

		mov ebx,b64Table

		mov eax,bFlag
		cmp eax,1
			je last

		mov eax,edx
		shr eax,18
		and eax,0x3f
		xlat
		mov ecx,eax
		
		mov eax,edx
		shr eax,12
		and eax,0x3f
		xlat
		shl eax,8
		or ecx,eax
		
		mov eax,edx
		shr eax,6
		and eax,0x3f
		xlat
		shl eax,16
		or ecx,eax	

		mov eax,edx
		and eax,0x3f
		xlat
		shl eax,24
		or ecx,eax
		jmp end
		
	last:
		mov eax,edx
		
		and eax,0x0000FF
		cmp eax,0
			jne bit24
		
		mov eax,edx
		and eax,0x00FF00
		cmp eax,0
			jne bit16

		mov eax,edx
		shr eax,18
		and eax,0x3f
		xlat
		mov ecx,eax
		
		mov eax,edx
		shr eax,12
		and eax,0x3f
		xlat
		shl eax,8
		or ecx,eax
		
		or ecx, 0x3d3d0000
				
		jmp end
	
	bit24:
		mov eax,edx
		shr eax,18
		and eax,0x3f
		xlat
		mov ecx,eax
		
		mov eax,edx
		shr eax,12
		and eax,0x3f
		xlat
		shl eax,8
		or ecx,eax
		
		mov eax,edx
		shr eax,6
		and eax,0x3f
		xlat
		shl eax,16
		or ecx,eax	

		mov eax,edx
		and eax,0x3f
		xlat
		shl eax,24
		or ecx,eax
		jmp end

	bit16:
		mov eax,edx
		shr eax,18
		and eax,0x3f
		xlat
		mov ecx,eax
		
		mov eax,edx
		shr eax,12
		and eax,0x3f
		xlat
		shl eax,8
		or ecx,eax
		
		mov eax,edx
		shr eax,6
		and eax,0x3f
		xlat
		shl eax,16
		or ecx,eax	
		
		or ecx,0x3d000000
				
	end:
		mov eax,dwout
		mov [eax],ecx

		pop edx
		pop ecx
		pop ebx
		pop eax
	}
}

__DLLEXPORT int WINAPI b64DecodeBlock(DWORD *dwIn, BYTE *bout,BYTE *b64Table, BOOL *bFlag)
{
	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esi
		push edi
		
		mov eax,dwIn
		mov edi,[eax]
		
		mov esi,b64Table
		xor edx,edx

		mov eax,edi
		and eax,0xffff0000
		cmp eax,0x3d3d0000
			je bit8

		mov eax,edi
		and eax,0xff000000
		cmp eax,0x3d000000
			je bit16

		mov ecx,0
		mov eax,edi
		mov dl,[esi+ecx]
		cmp al,dl
			je match1
		
	circle1:
		inc	ecx	
		mov dl,[esi+ecx]
		cmp al,dl
			jne circle1


	match1:
		shl ecx,18
		mov ebx,ecx

		shr eax,8
		mov ecx,0
		mov dl,[esi+ecx]
		cmp al,dl
			je match2

	circle2:
		inc	ecx	
		mov dl,[esi+ecx]
		cmp al,dl
			jne circle2

	match2:
		shl ecx,12
		or ebx,ecx

		shr eax,8
		mov ecx,0
		mov dl,[esi+ecx]
		cmp al,dl
			je match3

	circle3:
		inc	ecx	
		mov dl,[esi+ecx]
		cmp al,dl
			jne circle3
	
	match3:
		shl ecx,6
		or ebx,ecx
		shr eax,8

		mov ecx,0
		mov dl,[esi+ecx]
		cmp al,dl
			je match4

	circle4:
		inc	ecx	
		mov dl,[esi+ecx]
		cmp al,dl
			jne circle4

	match4:
		or ebx,ecx

		jmp end
	
	bit8:
		
		mov eax,bFlag
		cmp eax,0
			je notFlag1
		mov [eax],1
	notFlag1:


		mov ecx,0
		mov eax,edi
		mov dl,[esi+ecx]
		cmp al,dl
			je match5

	circle5:
		inc	ecx	
		mov dl,[esi+ecx]
		cmp al,dl
			jne circle5
	
	match5:
		shl ecx,18
		mov ebx,ecx

		shr eax,8
		mov ecx,0
		mov dl,[esi+ecx]
		cmp al,dl
			je match6

	circle6:
		inc	ecx	
		mov dl,[esi+ecx]
		cmp al,dl
			jne circle6

	match6:

		shl ecx,12
		or ebx,ecx

		jmp end

	bit16:

		mov eax,bFlag
		cmp eax,0
			je notFlag2
		mov [eax],1
	notFlag2:

		mov ecx,0
		mov eax,edi
		mov dl,[esi+ecx]
		cmp al,dl
			je match7
	circle7:
		inc	ecx	
		mov dl,[esi+ecx]
		cmp al,dl
		jne circle7

	match7:
	
		shl ecx,18
		mov ebx,ecx
		shr eax,8

		mov ecx,0
		mov dl,[esi+ecx]
		cmp al,dl
			je match8
	circle8:
		inc	ecx	
		mov dl,[esi+ecx]
		cmp al,dl
		jne circle8

	match8:

		shl ecx,12
		or ebx,ecx
		shr eax,8

		mov ecx,0
		mov dl,[esi+ecx]
		cmp al,dl
			je match9
	circle9:
		inc	ecx	
		mov dl,[esi+ecx]
		cmp al,dl
		jne circle9

	match9:

		shl ecx,6
		or ebx,ecx

		jmp end

	end:

		mov eax,bout
		mov [eax+2],bl
		mov [eax+1],bh
		shr ebx,8
		mov [eax],bh

		pop edi
		pop esi
		pop edx
		pop ecx
		pop ebx
		pop eax
	}
}


__DLLEXPORT int WINAPI b64DecodeMessage(BYTE *bSrc, DWORD dwSrcSize, BYTE *bRes, DWORD *dwResSize)
{
	BOOL bFlag = 0;
	DWORD dwTmpSrcSize = 0;
	DWORD i=0;
	BYTE *bTmpRes;
	BYTE *b64Table = (BYTE*)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	
	*dwResSize = 0;
	
	bTmpRes = (LPBYTE)LocalAlloc(LPTR,dwSrcSize);
	if (bTmpRes == NULL) return 0;
	
	do {
		dwTmpSrcSize += 4;
		*dwResSize += 3;
		b64DecodeBlock((DWORD*)bSrc+i,bTmpRes+i*3,b64Table,&bFlag);
		i++;
		if (bFlag) break;
	} while (dwTmpSrcSize<=dwSrcSize);
	CopyMemory(bRes,&bTmpRes,4);
	return 1;
}

__DLLEXPORT int WINAPI b64DecodeFile(TCHAR *lptSourceFile,TCHAR *lptDestFile)
{
	HANDLE hSource, hDest;
	BOOL bFlag = 0;
	DWORD dwTmpSize = 0;
	DWORD dwResSize = 0;
	DWORD i=0;
	BYTE *bOutputBuffer, *bInputBuffer;
	DWORD dwRead, dwWrite;
	BYTE *b64Table = (BYTE*)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	bOutputBuffer = (LPBYTE)LocalAlloc(LPTR,0x2000);
	if (bOutputBuffer == NULL) return 0;

	bInputBuffer = (LPBYTE)LocalAlloc(LPTR,0x2000);
	if (bInputBuffer == NULL) return 0;

	hSource = CreateFile(lptSourceFile,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	if (hSource == INVALID_HANDLE_VALUE) return 0;
	hDest = CreateFile(lptDestFile,GENERIC_WRITE,FILE_SHARE_READ,0,CREATE_ALWAYS,0,0);
	if (hDest == INVALID_HANDLE_VALUE) return 0;

	while (ReadFile(hSource,bInputBuffer,0x1000,&dwRead,0)!=0 && dwRead!=0) {
		i=0;
		dwResSize = 0;
		dwTmpSize = 0;
		do {
			dwTmpSize += 4;
			dwResSize += 3;
			
			b64DecodeBlock((DWORD*)bInputBuffer+i,bOutputBuffer+i*3,b64Table,&bFlag);
			i++;
			if (bFlag) {
				if (!(bOutputBuffer[i*3-1] | 0x00)) dwResSize-=1;
				if (!(bOutputBuffer[i*3-2] | 0x00)) dwResSize-=1;
			}
		} while (dwTmpSize<dwRead);
		WriteFile(hDest,bOutputBuffer,dwResSize,&dwWrite,0);
		dwResSize = 0;
	}
	ZeroMemory(bOutputBuffer,0x2000);
	ZeroMemory(bInputBuffer,0x2000);
	LocalFree(bInputBuffer);
	LocalFree(bOutputBuffer);
	CloseHandle(hDest);
	CloseHandle(hSource);
	return 1;	
}