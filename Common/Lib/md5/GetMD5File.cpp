/// @file GetMD5File.cpp
/// @brief MD5計算クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/28
/// @copyright Copyright(C)URARA-works 2005

#include "StdAfx.h"
#include "GetMD5File.h"

CGetMD5File::CGetMD5File()
{
}

CGetMD5File::~CGetMD5File()
{
}

void CGetMD5File::Update(LPCSTR pszFileName)
{
	PBYTE pTmp;
	DWORD dwFileSize, dwBytes;
	HANDLE hFile;

	pTmp = NULL;

	hFile = CreateFileA(pszFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) {
		goto Exit;
	}
	dwFileSize = GetFileSize(hFile, NULL);
	pTmp = new BYTE[dwFileSize];

	// ファイルを読み込む
	ReadFile(hFile, pTmp, dwFileSize, &dwBytes, NULL);
	CloseHandle(hFile);

	CGetMD5::Update(pTmp, dwFileSize);

Exit:
	SAFE_DELETE_ARRAY(pTmp);
}

