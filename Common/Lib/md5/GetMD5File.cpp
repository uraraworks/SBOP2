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
	FILE* pFile;
	long dwFileSize;

	pTmp = NULL;

	// ファイルをバイナリ読み込みモードで開く
	pFile = fopen(pszFileName, "rb");
	if (pFile == NULL) {
		goto Exit;
	}

	// ファイルサイズを取得
	fseek(pFile, 0, SEEK_END);
	dwFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	pTmp = new BYTE[dwFileSize];

	// ファイルを読み込む
	fread(pTmp, 1, dwFileSize, pFile);
	fclose(pFile);

	CGetMD5::Update(pTmp, dwFileSize);

Exit:
	SAFE_DELETE_ARRAY(pTmp);
}

