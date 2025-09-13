/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibSboSoundLoader.cpp										 */
/* 内容			:SBOサウンドデータライブラリ読み込みクラス 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibSboSoundLoader.h"

/* ========================================================================= */
/* 関数名	:CLibSboSoundLoader::CLibSboSoundLoader							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/05/03														 */
/* ========================================================================= */

CLibSboSoundLoader::CLibSboSoundLoader()
{
	m_hLib = NULL;
	m_pGetSoundCount		= NULL;
	m_pGetSoundResourceID	= NULL;
	m_pGetSoundID			= NULL;
	m_pGetSoundNo			= NULL;
	m_pGetSoundName			= NULL;
	Load ();
}


/* ========================================================================= */
/* 関数名：	CLibSboSoundLoader::~CLibSboSoundLoader							 */
/* 内容：	デストラクタ													 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

CLibSboSoundLoader::~CLibSboSoundLoader()
{
	Free ();
}


/* ========================================================================= */
/* 関数名：	CLibSboSoundLoader::Load										 */
/* 内容：	読み込み														 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

void CLibSboSoundLoader::Load(void)
{
	if (m_hLib) {
		return;
	}
	m_hLib = LoadLibrary ("SboSoundData.dll");
	if (m_hLib == NULL) {
		return;
	}

	/* 各関数のアドレスを取得 */
	m_pGetSoundCount		= (LIBSBOSOUNDGetSoundCount)		GetProcAddress (m_hLib, "GetSoundCount");
	m_pGetSoundResourceID	= (LIBSBOSOUNDGetSoundResourceID)	GetProcAddress (m_hLib, "GetSoundResourceID");
	m_pGetSoundID			= (LIBSBOSOUNDGetSoundID)			GetProcAddress (m_hLib, "GetSoundID");
	m_pGetSoundNo			= (LIBSBOSOUNDGetSoundNo)			GetProcAddress (m_hLib, "GetSoundNo");
	m_pGetSoundName			= (LIBSBOSOUNDGetSoundName)			GetProcAddress (m_hLib, "GetSoundName");

	if ((m_pGetSoundCount		== NULL) ||
		(m_pGetSoundResourceID	== NULL) ||
		(m_pGetSoundID			== NULL) ||
		(m_pGetSoundNo			== NULL) ||
		(m_pGetSoundName		== NULL)) {
		Free ();
	}
}


/* ========================================================================= */
/* 関数名：	CLibSboSoundLoader::Free										 */
/* 内容：	開放															 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

void CLibSboSoundLoader::Free(void)
{
	if (m_hLib) {
		FreeLibrary (m_hLib);
		m_pGetSoundCount		= NULL;
		m_pGetSoundResourceID	= NULL;
		m_pGetSoundID			= NULL;
		m_pGetSoundNo			= NULL;
		m_pGetSoundName			= NULL;
	}
}


/* ========================================================================= */
/* 関数名	:CLibSboSoundLoader::GetSoundCount								 */
/* 内容		:効果音数を取得													 */
/* 日付		:2007/05/03														 */
/* ========================================================================= */

int CLibSboSoundLoader::GetSoundCount(void)
{
	if (m_pGetSoundCount == NULL) {
		return 0;
	}
	return m_pGetSoundCount ();
}


/* ========================================================================= */
/* 関数名	:CLibSboSoundLoader::GetSoundResourceID							 */
/* 内容		:効果音のリソースIDを取得										 */
/* 日付		:2007/05/03														 */
/* ========================================================================= */

int CLibSboSoundLoader::GetSoundResourceID(int nNo)
{
	if (m_pGetSoundResourceID == NULL) {
		return 0;
	}
	return m_pGetSoundResourceID (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibSboSoundLoader::GetSoundID									 */
/* 内容		:効果音IDを取得													 */
/* 日付		:2007/09/19														 */
/* ========================================================================= */

DWORD CLibSboSoundLoader::GetSoundID(int nNo)
{
	if (m_pGetSoundID == NULL) {
		return 0;
	}
	return m_pGetSoundID (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibSboSoundLoader::GetSoundNo									 */
/* 内容		:効果音のインデックスを取得										 */
/* 日付		:2007/05/03														 */
/* ========================================================================= */

int CLibSboSoundLoader::GetSoundNo(DWORD dwSoundID)
{
	if (m_pGetSoundNo == NULL) {
		return -1;
	}
	return m_pGetSoundNo (dwSoundID);
}


/* ========================================================================= */
/* 関数名	:CLibSboSoundLoader::GetSoundName								 */
/* 内容		:効果音名を取得													 */
/* 日付		:2007/05/03														 */
/* ========================================================================= */

LPCSTR CLibSboSoundLoader::GetSoundName(DWORD dwSoundID)
{
	if (m_pGetSoundName == NULL) {
		return "";
	}
	return m_pGetSoundName (dwSoundID);
}

/* Copyright(C)URARA-works 2007 */
