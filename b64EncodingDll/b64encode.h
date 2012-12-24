#define __DLLEXPORT extern "C" __declspec(dllexport)

__DLLEXPORT int WINAPI b64EncodeBlock(BYTE *bIn, DWORD *out,BYTE *b64Table, BOOL bFlag);
__DLLEXPORT int WINAPI b64DecodeBlock(DWORD *dwIn, BYTE *bout,BYTE *b64Table, BOOL *bFlag);
__DLLEXPORT int WINAPI b64EncodeMessage(BYTE *bSrc,DWORD dwSrcSize,BYTE *bRes,DWORD *dwResSize);
__DLLEXPORT int WINAPI b64EncodeFile(TCHAR *lptSourceFile,TCHAR *lptDestFile);
__DLLEXPORT int WINAPI b64DecodeMessage(BYTE *bSrc, DWORD dwSrcSize, BYTE *bRes, DWORD *dwResSize);
__DLLEXPORT int WINAPI b64DecodeFile(TCHAR *lptSourceFile,TCHAR *lptDestFile);