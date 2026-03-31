/// @file LibSboSoundLoader.cpp
/// @brief SBOサウンドデータライブラリ読み込みクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/03
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "LibSboSoundLoader.h"

CLibSboSoundLoader::CLibSboSoundLoader()
{
	m_hLib = NULL;
	m_pGetSoundCount		= NULL;
	m_pGetSoundResourceID	= NULL;
	m_pGetSoundID			= NULL;
	m_pGetSoundNo			= NULL;
	m_pGetSoundName			= NULL;
	Load();
}


CLibSboSoundLoader::~CLibSboSoundLoader()
{
	Free();
}


void CLibSboSoundLoader::Load(void)
{
	if (m_hLib) {
		return;
	}
	m_hLib = LoadLibrary(_T("SboSoundData.dll"));
	if (m_hLib == NULL) {
		return;
	}

	// 各関数のアドレスを取得
	m_pGetSoundCount		= (LIBSBOSOUNDGetSoundCount)		GetProcAddress(m_hLib, "GetSoundCount");
	m_pGetSoundResourceID	= (LIBSBOSOUNDGetSoundResourceID)	GetProcAddress(m_hLib, "GetSoundResourceID");
	m_pGetSoundID			= (LIBSBOSOUNDGetSoundID)			GetProcAddress(m_hLib, "GetSoundID");
	m_pGetSoundNo			= (LIBSBOSOUNDGetSoundNo)			GetProcAddress(m_hLib, "GetSoundNo");
	m_pGetSoundName			= (LIBSBOSOUNDGetSoundName)			GetProcAddress(m_hLib, "GetSoundName");

	if ((m_pGetSoundCount		== NULL) ||
		(m_pGetSoundResourceID	== NULL) ||
		(m_pGetSoundID			== NULL) ||
		(m_pGetSoundNo			== NULL) ||
		(m_pGetSoundName		== NULL)) {
		Free();
	}
}


void CLibSboSoundLoader::Free(void)
{
	if (m_hLib) {
		FreeLibrary(m_hLib);
		m_pGetSoundCount		= NULL;
		m_pGetSoundResourceID	= NULL;
		m_pGetSoundID			= NULL;
		m_pGetSoundNo			= NULL;
		m_pGetSoundName			= NULL;
	}
}


int CLibSboSoundLoader::GetSoundCount(void)
{
	if (m_pGetSoundCount == NULL) {
		return 0;
	}
	return m_pGetSoundCount();
}


int CLibSboSoundLoader::GetSoundResourceID(int nNo)
{
	if (m_pGetSoundResourceID == NULL) {
		return 0;
	}
	return m_pGetSoundResourceID(nNo);
}


DWORD CLibSboSoundLoader::GetSoundID(int nNo)
{
	if (m_pGetSoundID == NULL) {
		return 0;
	}
	return m_pGetSoundID(nNo);
}


int CLibSboSoundLoader::GetSoundNo(DWORD dwSoundID)
{
	if (m_pGetSoundNo == NULL) {
		return -1;
	}
	return m_pGetSoundNo(dwSoundID);
}


LPCSTR CLibSboSoundLoader::GetSoundName(DWORD dwSoundID)
{
	if (m_pGetSoundName == NULL) {
		return "";
	}
	return m_pGetSoundName(dwSoundID);
}
