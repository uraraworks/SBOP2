/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:CryptUtil.cpp												 */
/* 内容			:暗号復号クラス 実装ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/02													 */
/* ========================================================================= */

#include "stdafx.h"
#include <stdlib.h>
#include "CryptUtil.h"


/* ========================================================================= */
/* 関数名	:CCryptUtil::CCryptUtil											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/02														 */
/* ========================================================================= */

CCryptUtil::CCryptUtil()
{
	m_pCryptData = NULL;
	Init ();
}


/* ========================================================================= */
/* 関数名	:CCryptUtil::~CCryptUtil										 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/02														 */
/* ========================================================================= */

CCryptUtil::~CCryptUtil()
{
	if (m_pCryptData) {
		delete [] m_pCryptData;
		m_pCryptData = NULL;
	}
}


/* ========================================================================= */
/* 関数名	:CCryptUtil::Crypt												 */
/* 内容		:暗号化															 */
/* 日付		:2007/07/02														 */
/* ========================================================================= */

void CCryptUtil::Crypt(
	PBYTE pSrc,			/* [in]  暗号化するデータ */
	PBYTE pDst,			/* [out] 暗号化したデータ */
	DWORD dwSize,		/* [in]  データサイズ */
	int nKeyNo)			/* [in]  初期化位置 */
{
	DWORD i;

	for (i = 0; i < dwSize; i ++) {
		pDst[i] = pSrc[i] ^ m_pCryptData[nKeyNo % 256];
	}
}


/* ========================================================================= */
/* 関数名	:CCryptUtil::UnCrypt											 */
/* 内容		:復号化															 */
/* 日付		:2007/07/02														 */
/* ========================================================================= */

void CCryptUtil::UnCrypt(
	PBYTE pSrc,			/* [in]  復号化するデータ */
	PBYTE pDst,			/* [out] 復号化したデータ */
	DWORD dwSize,		/* [in] データサイズ */
	int nKeyNo)			/* [in] 初期化位置 */
{
	Crypt (pSrc, pDst, dwSize, nKeyNo);
}


/* ========================================================================= */
/* 関数名	:CCryptUtil::CryptStr											 */
/* 内容		:暗号化(文字列)													 */
/* 日付		:2007/07/02														 */
/* メモ		:pDstは dwSize * 3 + 1バイト以上必要							 */
/* ========================================================================= */

void CCryptUtil::CryptStr(
	LPCSTR pszSrc,		/* [in]  暗号化するデータ */
	LPSTR pszDst,		/* [out] 暗号化したデータ */
	int nKeyNo)			/* [in]  初期化位置 */
{
	int i, nLen;

	strcpy ((LPSTR)pszDst, "");

	nLen = strlen (pszSrc);
	for (i = 0; i < nLen; i ++) {
		wsprintf (&pszDst[i * 2], "%02X", (BYTE)pszSrc[i] ^ m_pCryptData[nKeyNo % 256]);
	}
}


/* ========================================================================= */
/* 関数名	:CCryptUtil::UnCryptStr											 */
/* 内容		:復号化(文字列)													 */
/* 日付		:2007/07/02														 */
/* ========================================================================= */

void CCryptUtil::UnCryptStr(
	LPCSTR pszSrc,		/* [in]  復号化するデータ */
	LPSTR pszDst,		/* [out] 復号化したデータ */
	int nKeyNo)			/* [in] 初期化位置 */
{
	int i, nLen, nPos;
	char szTmp[3];

	nPos = 0;
	strcpy ((LPSTR)pszDst, "");
	szTmp[2] = 0;

	nLen = strlen (pszSrc);
	for (i = 0; i < nLen; i += 2, nPos ++) {
		CopyMemory (szTmp, &pszSrc[i], 2);
		pszDst[nPos] = ((BYTE)strtol (szTmp, NULL, 16) ^ m_pCryptData[nKeyNo % 256]);
	}
	pszDst[nPos] = 0;
}


/* ========================================================================= */
/* 関数名	:CCryptUtil::Init												 */
/* 内容		:初期化															 */
/* 日付		:2007/07/02														 */
/* ========================================================================= */

void CCryptUtil::Init(void)
{
	int i;
	BYTE abyData[256] = {
		0x29, 0x23, 0xBE, 0x84, 0xE1, 0x6C, 0xD6, 0xAE,
		0x52, 0x90, 0x49, 0xF1, 0xF1, 0xBB, 0xE9, 0xEB,
		0xB3, 0xA6, 0xDB, 0x3C, 0x87, 0x0C, 0x3E, 0x99,
		0x24, 0x5E, 0x0D, 0x1C, 0x06, 0xB7, 0x47, 0xDE,
		0xB3, 0x12, 0x4D, 0xC8, 0x43, 0xBB, 0x8B, 0xA6,
		0x1F, 0x03, 0x5A, 0x7D, 0x09, 0x38, 0x25, 0x1F,
		0x5D, 0xD4, 0xCB, 0xFC, 0x96, 0xF5, 0x45, 0x3B,
		0x13, 0x0D, 0x89, 0x0A, 0x1C, 0xDB, 0xAE, 0x32,
		0x20, 0x9A, 0x50, 0xEE, 0x40, 0x78, 0x36, 0xFD,
		0x12, 0x49, 0x32, 0xF6, 0x9E, 0x7D, 0x49, 0xDC,
		0xAD, 0x4F, 0x14, 0xF2, 0x44, 0x40, 0x66, 0xD0,
		0x6B, 0xC4, 0x30, 0xB7, 0x32, 0x3B, 0xA1, 0x22,
		0xF6, 0x22, 0x91, 0x9D, 0xE1, 0x8B, 0x1F, 0xDA,
		0xB0, 0xCA, 0x99, 0x02, 0xB9, 0x72, 0x9D, 0x49,
		0x2C, 0x80, 0x7E, 0xC5, 0x99, 0xD5, 0xE9, 0x80,
		0xB2, 0xEA, 0xC9, 0xCC, 0x53, 0xBF, 0x67, 0xD6,
		0xBF, 0x14, 0xD6, 0x7E, 0x2D, 0xDC, 0x8E, 0x66,
		0x83, 0xEF, 0x57, 0x49, 0x61, 0xFF, 0x69, 0x8F,
		0x61, 0xCD, 0xD1, 0x1E, 0x9D, 0x9C, 0x16, 0x72,
		0x72, 0xE6, 0x1D, 0xF0, 0x84, 0x4F, 0x4A, 0x77,
		0x02, 0xD7, 0xE8, 0x39, 0x2C, 0x53, 0xCB, 0xC9,
		0x12, 0x1E, 0x33, 0x74, 0x9E, 0x0C, 0xF4, 0xD5,
		0xD4, 0x9F, 0xD4, 0xA4, 0x59, 0x7E, 0x35, 0xCF,
		0x32, 0x22, 0xF4, 0xCC, 0xCF, 0xD3, 0x90, 0x2D,
		0x48, 0xD3, 0x8F, 0x75, 0xE6, 0xD9, 0x1D, 0x2A,
		0xE5, 0xC0, 0xF7, 0x2B, 0x78, 0x81, 0x87, 0x44,
		0x0E, 0x5F, 0x50, 0x00, 0xD4, 0x61, 0x8D, 0xBE,
		0x7B, 0x05, 0x15, 0x07, 0x3B, 0x33, 0x82, 0x1F,
		0x18, 0x70, 0x92, 0xDA, 0x64, 0x54, 0xCE, 0xB1,
		0x85, 0x3E, 0x69, 0x15, 0xF8, 0x46, 0x6A, 0x04,
		0x96, 0x73, 0x0E, 0xD9, 0x16, 0x2F, 0x67, 0x68,
		0xD4, 0xF7, 0x4A, 0x4A, 0xD0, 0x57, 0x68, 0x76
	};

	if (m_pCryptData) {
		delete [] m_pCryptData;
		m_pCryptData = NULL;
	}
	m_pCryptData = new BYTE[256];

	for (i = 0; i < 256; i ++) {
		m_pCryptData[i] = abyData[i];
	}
}

/* Copyright(C)URARA-works 2007 */
