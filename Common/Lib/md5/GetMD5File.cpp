/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名	:GetMD5File.cpp												 */
/* 内容			:MD5計算クラス 実装ファイル									 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/28													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "GetMD5File.h"

/* ========================================================================= */
/* 関数名	:CGetMD5File::CGetMD5File										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/08/28														 */
/* ========================================================================= */

CGetMD5File::CGetMD5File()
{
}


/* ========================================================================= */
/* 関数名	:CGetMD5File::~CGetMD5File										 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/08/28														 */
/* ========================================================================= */

CGetMD5File::~CGetMD5File()
{
}


/* ========================================================================= */
/* 関数名	:CGetMD5File::Update											 */
/* 内容		:更新															 */
/* 日付		:2007/08/28														 */
/* ========================================================================= */

void CGetMD5File::Update(LPCSTR pszFileName)
{
	PBYTE pTmp;
	DWORD dwFileSize, dwBytes;
	HANDLE hFile;

	pTmp = NULL;

	hFile = CreateFile (pszFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) {
		goto Exit;
	}
	dwFileSize = GetFileSize (hFile, NULL);
	pTmp = new BYTE[dwFileSize];

	/* ファイルを読み込む */
	ReadFile (hFile, pTmp, dwFileSize, &dwBytes, NULL);
	CloseHandle (hFile);

	CGetMD5::Update (pTmp, dwFileSize);

Exit:
	SAFE_DELETE_ARRAY (pTmp);
}

/* Copyright(C)URARA-works 2007 */
