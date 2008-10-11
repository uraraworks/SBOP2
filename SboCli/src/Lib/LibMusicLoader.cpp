/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名：	LibMusicLoader.cpp											 */
/* 内容：		AflMusicライブラリ読み込みクラス 実装ファイル				 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2005/10/18													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibMusicLoader.h"

/* ========================================================================= */
/* 関数名：	CLibMusicLoader::CLibMusicLoader								 */
/* 内容：	コンストラクタ													 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

CLibMusicLoader::CLibMusicLoader()
{
	m_hLib = NULL;
	Load ();
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::~CLibMusicLoader								 */
/* 内容：	デストラクタ													 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

CLibMusicLoader::~CLibMusicLoader()
{
	Free ();
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::Load											 */
/* 内容：	読み込み														 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

void CLibMusicLoader::Load(void)
{
	if (m_hLib) {
		return;
	}
	m_hLib = LoadLibrary ("LibMusic.dll");
	if (m_hLib == NULL) {
		return;
	}

	/* 各関数のアドレスを取得 */
	m_pOpen1				= (LIBMUSICOpen1)				GetProcAddress (m_hLib, "Open1");
	m_pOpen2				= (LIBMUSICOpen2)				GetProcAddress (m_hLib, "Open2");
	m_pClose				= (LIBMUSICClose)				GetProcAddress (m_hLib, "Close");
	m_pFadeIn				= (LIBMUSICFadeIn)				GetProcAddress (m_hLib, "FadeIn");
	m_pFadeOut				= (LIBMUSICFadeOut)				GetProcAddress (m_hLib, "FadeOut");
	m_pPlayTime				= (LIBMUSICPlayTime)			GetProcAddress (m_hLib, "PlayTime");
	m_pPlay1				= (LIBMUSICPlay1)				GetProcAddress (m_hLib, "Play1");
	m_pPlay2				= (LIBMUSICPlay2)				GetProcAddress (m_hLib, "Play2");
	m_pCont					= (LIBMUSICCont)				GetProcAddress (m_hLib, "Cont");
	m_pStop					= (LIBMUSICStop)				GetProcAddress (m_hLib, "Stop");
	m_pIsPlay				= (LIBMUSICIsPlay)				GetProcAddress (m_hLib, "IsPlay");
	m_pSetLoop				= (LIBMUSICSetLoop)				GetProcAddress (m_hLib, "SetLoop");
	m_pSetRelativeVolume	= (LIBMUSICSetRelativeVolume)	GetProcAddress (m_hLib, "SetRelativeVolume");
	m_pGetTitle				= (LIBMUSICGetTitle)			GetProcAddress (m_hLib, "GetTitle");
	m_pGetAllTime			= (LIBMUSICGetAllTime)			GetProcAddress (m_hLib, "GetAllTime");
	m_pGetPlayTime			= (LIBMUSICGetPlayTime)			GetProcAddress (m_hLib, "GetPlayTime");

	if (m_pOpen1 == NULL) {
		Free ();
	}
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::Free											 */
/* 内容：	開放															 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

void CLibMusicLoader::Free(void)
{
	if (m_hLib) {
		FreeLibrary (m_hLib);
		m_pOpen1				= NULL;
		m_pOpen2				= NULL;
		m_pClose				= NULL;
		m_pFadeIn				= NULL;
		m_pFadeOut				= NULL;
		m_pPlayTime				= NULL;
		m_pPlay1				= NULL;
		m_pPlay2				= NULL;
		m_pCont					= NULL;
		m_pStop					= NULL;
		m_pIsPlay				= NULL;
		m_pSetLoop				= NULL;
		m_pSetRelativeVolume	= NULL;
		m_pGetTitle				= NULL;
		m_pGetAllTime			= NULL;
		m_pGetPlayTime			= NULL;
	}
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::Open											 */
/* 内容：	ファイルからMIDIデータを読み込む								 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::Open(
	LPCSTR pFileName,		/* [in] ファイル名 */
	LPCSTR pType)			/* [in] リソース名 */
{
	if (m_pOpen1 == NULL) {
		return FALSE;
	}
	return m_pOpen1 (pFileName, pType);
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::Open											 */
/* 内容：	リソースからMIDIデータを読み込む								 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::Open(
	HMODULE hResource,		/* [in] リソースのモジュールハンドル */
	HRSRC hSrc,				/* [in] リソースハンドル */
	LPCSTR pType)			/* [in] リソース名 */
{
	if (m_pOpen2 == NULL) {
		return FALSE;
	}
	return m_pOpen2 (hResource, hSrc, pType);
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::Close											 */
/* 内容：	データをメモリから開放											 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::Close(void)
{
	if (m_pClose == NULL) {
		return FALSE;
	}
	return m_pClose ();
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::FadeIn											 */
/* 内容：	フェードインを行う												 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::FadeIn(
	INT nTime,			/* [in] 通常の演奏状態になるまでの時間(ms) */
	INT nVolume)		/* [in] 初期ボリューム */
{
	if (m_pFadeIn == NULL) {
		return FALSE;
	}
	return m_pFadeIn (nTime, nVolume);
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::FadeOut										 */
/* 内容：	フェードアウトを行う											 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::FadeOut(
	INT nTime)		/* [in] ボリュームが0になるまでの時間(ms) */
{
	if (m_pFadeOut == NULL) {
		return FALSE;
	}
	return m_pFadeOut (nTime);
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::PlayTime										 */
/* 内容：	指定時間からから演奏開始										 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::PlayTime(
	DWORD dwTime)		/* [in] 再生開始位置(ms) */
{
	if (m_pPlayTime == NULL) {
		return FALSE;
	}
	return m_pPlayTime (dwTime);
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::Play											 */
/* 内容：	指定位置から演奏開始											 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::Play(
	DWORDLONG dwlCount)		/* [in] 再生開始カウント位置 */
{
	if (m_pPlay1 == NULL) {
		return FALSE;
	}
	return m_pPlay1 (dwlCount);
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::Play											 */
/* 内容：	演奏開始														 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::Play(void)
{
	if (m_pPlay2 == NULL) {
		return FALSE;
	}
	return m_pPlay2 ();
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::Cont											 */
/* 内容：	演奏を再開														 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::Cont(void)
{
	if (m_pCont == NULL) {
		return FALSE;
	}
	return m_pCont ();
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::Stop											 */
/* 内容：	演奏停止														 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::Stop(void)
{
	if (m_pStop == NULL) {
		return FALSE;
	}
	return m_pStop ();
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::IsPlay											 */
/* 内容：	演奏中か判定													 */
/* 日付：	2005/10/18														 */
/* 戻り値：	TRUE:演奏中														 */
/* ========================================================================= */

BOOL CLibMusicLoader::IsPlay(void)
{
	if (m_pIsPlay == NULL) {
		return FALSE;
	}
	return m_pIsPlay ();
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::SetLoop										 */
/* 内容：	ループさせるか指定												 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

void CLibMusicLoader::SetLoop(
	BOOL bLoop)		/* [in] TRUE:ループさせる */
{
	bool bLoopTmp;

	if (m_pSetLoop == NULL) {
		return;
	}
	bLoopTmp = (bLoop) ? true : false;
	m_pSetLoop (bLoopTmp);
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::SetRelativeVolume								 */
/* 内容：	ボリュームの設定												 */
/* 日付：	2005/10/23														 */
/* ========================================================================= */

void CLibMusicLoader::SetRelativeVolume(
	INT nVolume)		/* [in] ボリューム */
{
	if (m_pSetRelativeVolume == NULL) {
		return;
	}
	m_pSetRelativeVolume (nVolume);
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::GetTitle										 */
/* 内容：	タイトル名取得													 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

LPCSTR CLibMusicLoader::GetTitle(void)
{
	if (m_pGetTitle == NULL) {
		return NULL;
	}
	return m_pGetTitle ();
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::GetAllTime										 */
/* 内容：	総演奏時間を取得												 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

DWORDLONG CLibMusicLoader::GetAllTime(void)
{
	if (m_pGetAllTime == NULL) {
		return 0;
	}
	return m_pGetAllTime ();
}


/* ========================================================================= */
/* 関数名：	CLibMusicLoader::GetPlayTime									 */
/* 内容：	演奏時間を取得													 */
/* 日付：	2005/10/18														 */
/* ========================================================================= */

DWORDLONG CLibMusicLoader::GetPlayTime(void)
{
	if (m_pGetPlayTime == NULL) {
		return 0;
	}
	return m_pGetPlayTime ();
}

/* Copyright(C)URARA-works 2005 */
