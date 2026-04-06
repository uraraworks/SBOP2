/// @file LibSboSoundLoader.cpp
/// @brief SBOサウンドデータライブラリ読み込みクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/03
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#if !defined(_WINDLL)
#include <SDL.h>
#endif
#include "LibSboSoundLoader.h"

// DLL動的ロードのプラットフォーム抽象化マクロ
// SboCliAdminMfc（_WINDLL定義あり）ではSDL未使用のためWin32 APIを使う
// DLL関数ロード・解放のプラットフォーム抽象化マクロ
// SboCliAdminMfc（_WINDLL定義あり）ではSDL未使用のためWin32 APIを使う
#if !defined(_WINDLL)
#define DYN_LOAD_FUNC(lib, name)   SDL_LoadFunction(lib, name)
#define DYN_UNLOAD_LIB(lib)        SDL_UnloadObject(lib)
#else
#define DYN_LOAD_FUNC(lib, name)   ((void*)GetProcAddress((HMODULE)(lib), name))
#define DYN_UNLOAD_LIB(lib)        FreeLibrary((HMODULE)(lib))
#endif

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

#if !defined(_WINDLL)
	// SboCli: SDL_LoadObject でクロスプラットフォーム対応
	// BuildModuleRelativePath は TCHAR* を受け取るため TCHAR 配列を使用
	TCHAR szPath[MAX_PATH];
	BuildModuleRelativePath(szPath, _countof(szPath), _T("SboSoundData.dll"));
	// SDL_LoadObject は char* を受け取るため ANSI 文字列に変換
	std::string ansiPath = TStringToAnsiStd(szPath);
	m_hLib = SDL_LoadObject(ansiPath.c_str());
#else
	// SboCliAdminMfc: Win32 API（TCHAR/Unicode対応）
	TCHAR szPath[MAX_PATH];
	BuildModuleRelativePath(szPath, _countof(szPath), _T("SboSoundData.dll"));
	m_hLib = (void*)LoadLibrary(szPath);
#endif
	if (m_hLib == NULL) {
		return;
	}

	// 各関数のアドレスを取得
	m_pGetSoundCount		= (LIBSBOSOUNDGetSoundCount)		DYN_LOAD_FUNC(m_hLib, "GetSoundCount");
	m_pGetSoundResourceID	= (LIBSBOSOUNDGetSoundResourceID)	DYN_LOAD_FUNC(m_hLib, "GetSoundResourceID");
	m_pGetSoundID			= (LIBSBOSOUNDGetSoundID)			DYN_LOAD_FUNC(m_hLib, "GetSoundID");
	m_pGetSoundNo			= (LIBSBOSOUNDGetSoundNo)			DYN_LOAD_FUNC(m_hLib, "GetSoundNo");
	m_pGetSoundName			= (LIBSBOSOUNDGetSoundName)			DYN_LOAD_FUNC(m_hLib, "GetSoundName");

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
		DYN_UNLOAD_LIB(m_hLib);
		m_hLib = NULL;
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
