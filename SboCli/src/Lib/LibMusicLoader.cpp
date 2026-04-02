/// @file LibMusicLoader.cpp
/// @brief AflMusicライブラリ読み込みクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/10/18
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#include "LibMusicLoader.h"

#if defined(__EMSCRIPTEN__)

CLibMusicLoader::CLibMusicLoader()
{
	m_hLib = NULL;
	m_pOpen1 = NULL;
	m_pOpen2 = NULL;
	m_pClose = NULL;
	m_pFadeIn = NULL;
	m_pFadeOut = NULL;
	m_pPlayTime = NULL;
	m_pPlay1 = NULL;
	m_pPlay2 = NULL;
	m_pCont = NULL;
	m_pStop = NULL;
	m_pIsPlay = NULL;
	m_pSetLoop = NULL;
	m_pSetRelativeVolume = NULL;
	m_pGetTitle = NULL;
	m_pGetAllTime = NULL;
	m_pGetPlayTime = NULL;
}

CLibMusicLoader::~CLibMusicLoader()
{
}

void CLibMusicLoader::Load(void)
{
}

void CLibMusicLoader::Free(void)
{
}

BOOL CLibMusicLoader::Open(LPCSTR pFileName, LPCSTR pType)
{
	(void)pFileName;
	(void)pType;
	return FALSE;
}

BOOL CLibMusicLoader::Open(HMODULE hResource, HRSRC hSrc, LPCSTR pType)
{
	(void)hResource;
	(void)hSrc;
	(void)pType;
	return FALSE;
}

BOOL CLibMusicLoader::Close(void)
{
	return FALSE;
}

BOOL CLibMusicLoader::FadeIn(INT nTime, INT nVolume)
{
	(void)nTime;
	(void)nVolume;
	return FALSE;
}

BOOL CLibMusicLoader::FadeOut(INT nTime)
{
	(void)nTime;
	return FALSE;
}

BOOL CLibMusicLoader::PlayTime(DWORD dwTime)
{
	(void)dwTime;
	return FALSE;
}

BOOL CLibMusicLoader::Play(DWORDLONG dwlCount)
{
	(void)dwlCount;
	return FALSE;
}

BOOL CLibMusicLoader::Play(void)
{
	return FALSE;
}

BOOL CLibMusicLoader::Cont(void)
{
	return FALSE;
}

BOOL CLibMusicLoader::Stop(void)
{
	return FALSE;
}

BOOL CLibMusicLoader::IsPlay(void)
{
	return FALSE;
}

void CLibMusicLoader::SetLoop(BOOL bLoop)
{
	(void)bLoop;
}

void CLibMusicLoader::SetRelativeVolume(INT nVolume)
{
	(void)nVolume;
}

LPCSTR CLibMusicLoader::GetTitle(void)
{
	return NULL;
}

DWORDLONG CLibMusicLoader::GetAllTime(void)
{
	return 0;
}

DWORDLONG CLibMusicLoader::GetPlayTime(void)
{
	return 0;
}

#else

CLibMusicLoader::CLibMusicLoader()
{
	m_hLib = NULL;
	Load();
}

CLibMusicLoader::~CLibMusicLoader()
{
	Free();
}

void CLibMusicLoader::Load(void)
{
	if (m_hLib) {
		return;
	}
	m_hLib = LoadLibrary(_T("LibMusic.dll"));
	if (m_hLib == NULL) {
		return;
	}

	// 各関数のアドレスを取得
	m_pOpen1             = (LIBMUSICOpen1)             GetProcAddress(m_hLib, "Open1");
	m_pOpen2             = (LIBMUSICOpen2)             GetProcAddress(m_hLib, "Open2");
	m_pClose             = (LIBMUSICClose)             GetProcAddress(m_hLib, "Close");
	m_pFadeIn            = (LIBMUSICFadeIn)            GetProcAddress(m_hLib, "FadeIn");
	m_pFadeOut           = (LIBMUSICFadeOut)           GetProcAddress(m_hLib, "FadeOut");
	m_pPlayTime          = (LIBMUSICPlayTime)          GetProcAddress(m_hLib, "PlayTime");
	m_pPlay1             = (LIBMUSICPlay1)             GetProcAddress(m_hLib, "Play1");
	m_pPlay2             = (LIBMUSICPlay2)             GetProcAddress(m_hLib, "Play2");
	m_pCont              = (LIBMUSICCont)              GetProcAddress(m_hLib, "Cont");
	m_pStop              = (LIBMUSICStop)              GetProcAddress(m_hLib, "Stop");
	m_pIsPlay            = (LIBMUSICIsPlay)            GetProcAddress(m_hLib, "IsPlay");
	m_pSetLoop           = (LIBMUSICSetLoop)           GetProcAddress(m_hLib, "SetLoop");
	m_pSetRelativeVolume = (LIBMUSICSetRelativeVolume) GetProcAddress(m_hLib, "SetRelativeVolume");
	m_pGetTitle          = (LIBMUSICGetTitle)          GetProcAddress(m_hLib, "GetTitle");
	m_pGetAllTime        = (LIBMUSICGetAllTime)        GetProcAddress(m_hLib, "GetAllTime");
	m_pGetPlayTime       = (LIBMUSICGetPlayTime)       GetProcAddress(m_hLib, "GetPlayTime");

	if (m_pOpen1 == NULL) {
		Free();
	}
}

void CLibMusicLoader::Free(void)
{
	if (m_hLib) {
		FreeLibrary(m_hLib);
		m_pOpen1             = NULL;
		m_pOpen2             = NULL;
		m_pClose             = NULL;
		m_pFadeIn            = NULL;
		m_pFadeOut           = NULL;
		m_pPlayTime          = NULL;
		m_pPlay1             = NULL;
		m_pPlay2             = NULL;
		m_pCont              = NULL;
		m_pStop              = NULL;
		m_pIsPlay            = NULL;
		m_pSetLoop           = NULL;
		m_pSetRelativeVolume = NULL;
		m_pGetTitle          = NULL;
		m_pGetAllTime        = NULL;
		m_pGetPlayTime       = NULL;
	}
}

BOOL CLibMusicLoader::Open(
	LPCSTR pFileName,	// [in] ファイル名
	LPCSTR pType)		// [in] リソース名
{
	if (m_pOpen1 == NULL) {
		return FALSE;
	}
	return m_pOpen1(pFileName, pType);
}

BOOL CLibMusicLoader::Open(
	HMODULE hResource,	// [in] リソースのモジュールハンドル
	HRSRC hSrc,			// [in] リソースハンドル
	LPCSTR pType)		// [in] リソース名
{
	if (m_pOpen2 == NULL) {
		return FALSE;
	}
	return m_pOpen2(hResource, hSrc, pType);
}

BOOL CLibMusicLoader::Close(void)
{
	if (m_pClose == NULL) {
		return FALSE;
	}
	return m_pClose();
}

BOOL CLibMusicLoader::FadeIn(
	INT nTime,		// [in] 通常の演奏状態になるまでの時間(ms)
	INT nVolume)	// [in] 初期ボリューム
{
	if (m_pFadeIn == NULL) {
		return FALSE;
	}
	return m_pFadeIn(nTime, nVolume);
}

BOOL CLibMusicLoader::FadeOut(
	INT nTime)	// [in] ボリュームが0になるまでの時間(ms)
{
	if (m_pFadeOut == NULL) {
		return FALSE;
	}
	return m_pFadeOut(nTime);
}

BOOL CLibMusicLoader::PlayTime(
	DWORD dwTime)	// [in] 再生開始位置(ms)
{
	if (m_pPlayTime == NULL) {
		return FALSE;
	}
	return m_pPlayTime(dwTime);
}

BOOL CLibMusicLoader::Play(
	DWORDLONG dwlCount)	// [in] 再生開始カウント位置
{
	if (m_pPlay1 == NULL) {
		return FALSE;
	}
	return m_pPlay1(dwlCount);
}

BOOL CLibMusicLoader::Play(void)
{
	if (m_pPlay2 == NULL) {
		return FALSE;
	}
	return m_pPlay2();
}

BOOL CLibMusicLoader::Cont(void)
{
	if (m_pCont == NULL) {
		return FALSE;
	}
	return m_pCont();
}

BOOL CLibMusicLoader::Stop(void)
{
	if (m_pStop == NULL) {
		return FALSE;
	}
	return m_pStop();
}

BOOL CLibMusicLoader::IsPlay(void)
{
	if (m_pIsPlay == NULL) {
		return FALSE;
	}
	return m_pIsPlay();
}

void CLibMusicLoader::SetLoop(
	BOOL bLoop)	// [in] TRUE:ループさせる
{
	bool bLoopTmp;

	if (m_pSetLoop == NULL) {
		return;
	}
	bLoopTmp = (bLoop) ? true : false;
	m_pSetLoop(bLoopTmp);
}

void CLibMusicLoader::SetRelativeVolume(
	INT nVolume)	// [in] ボリューム
{
	if (m_pSetRelativeVolume == NULL) {
		return;
	}
	m_pSetRelativeVolume(nVolume);
}

LPCSTR CLibMusicLoader::GetTitle(void)
{
	if (m_pGetTitle == NULL) {
		return NULL;
	}
	return m_pGetTitle();
}

DWORDLONG CLibMusicLoader::GetAllTime(void)
{
	if (m_pGetAllTime == NULL) {
		return 0;
	}
	return m_pGetAllTime();
}

DWORDLONG CLibMusicLoader::GetPlayTime(void)
{
	if (m_pGetPlayTime == NULL) {
		return 0;
	}
	return m_pGetPlayTime();
}

#endif
