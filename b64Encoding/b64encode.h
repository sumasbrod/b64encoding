

int b64EncodeBlock(BYTE *bIn, DWORD *out,BYTE *b64Table, BOOL bFlag);
int b64DecodeBlock(DWORD *dwIn, BYTE *bout,BYTE *b64Table, BOOL *bFlag);
int WINAPI b64EncodeMessage(BYTE *bSrc,DWORD dwSrcSize,BYTE *bRes,DWORD *dwResSize);
int WINAPI b64EncodeFile(TCHAR *lptSourceFile,TCHAR *lptDestFile);
int WINAPI b64DecodeMessage(BYTE *bSrc, DWORD dwSrcSize, BYTE *bRes, DWORD *dwResSize);
int WINAPI b64DecodeFile(TCHAR *lptSourceFile,TCHAR *lptDestFile);