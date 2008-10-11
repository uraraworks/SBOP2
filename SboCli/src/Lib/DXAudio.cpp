/* Copyright(C)URARA-works 2003 */
/* ========================================================================= */
/* ファイル名：	DXAudio.cpp													 */
/* 内容：		DirectX Audioを使うためのクラス								 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2003/03/16													 */
/* ========================================================================= */

#include "stdafx.h"
#include <windowsx.h>

#include "DXAudio.h"

#pragma comment (lib, "dxguid.lib")

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

#define SAFE_RELEASE(x)	{if(x){(x)->Release();x=NULL;}}


/* ========================================================================= */
/* 関数名：	CDXAudio::CDXAudio												 */
/* 内容：	コンストラクタ													 */
/* 日付：	2003/03/16														 */
/* ========================================================================= */

CDXAudio::CDXAudio()
{
	m_pPerformance		= NULL;
	m_pDefAudioPath		= NULL;
	m_pDefAudioPath2	= NULL;
	m_pLoader			= NULL;
	m_hResource			= NULL;
}


/* ========================================================================= */
/* 関数名：	CDXAudio::~CDXAudio												 */
/* 内容：	デストラクタ													 */
/* 日付：	2003/03/16														 */
/* ========================================================================= */

CDXAudio::~CDXAudio()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名：	CDXAudio::Create												 */
/* 内容：	初期化															 */
/* 日付：	2003/03/16														 */
/* ========================================================================= */

BOOL CDXAudio::Create(void)
{
	HRESULT hr;

	/* パフォーマンスの作成 */
	hr = CoCreateInstance (
				CLSID_DirectMusicPerformance,
				NULL,
				CLSCTX_INPROC,
				IID_IDirectMusicPerformance8,
				(PVOID *)&m_pPerformance);
	if (FAILED (hr)) {
		return FALSE;
	}

	/* 初期化 */
	m_pPerformance->InitAudio (
			NULL, NULL, NULL,
			DMUS_APATH_DYNAMIC_STEREO,
			64,
			DMUS_AUDIOF_ALL,
			NULL);
	if (FAILED (hr)) {
		return FALSE;
	}

	/* デフォルトのオーディオパスを取得 */
	m_pPerformance->GetDefaultAudioPath (&m_pDefAudioPath);
	if (FAILED (hr)) {
		return FALSE;
	}
	m_pPerformance->CreateStandardAudioPath (DMUS_APATH_DYNAMIC_STEREO, 64, TRUE, &m_pDefAudioPath2);

	hr = CoCreateInstance (
				CLSID_DirectMusicLoader,
				NULL,
				CLSCTX_INPROC,
				IID_IDirectMusicLoader8,
				(PVOID *)&m_pLoader);
	if (FAILED (hr)) {
		return FALSE;
	}

	return TRUE;
}


/* ========================================================================= */
/* 関数名：	CDXAudio::Destroy												 */
/* 内容：	終了処理														 */
/* 日付：	2003/03/16														 */
/* ========================================================================= */

void CDXAudio::Destroy(void)
{
	if (m_pPerformance) {
		m_pPerformance->Stop (NULL, NULL, 0, 0);
		m_pPerformance->CloseDown ();
	}
	SAFE_RELEASE (m_pDefAudioPath);
	SAFE_RELEASE (m_pDefAudioPath2);
	SAFE_RELEASE (m_pPerformance);
	SAFE_RELEASE (m_pLoader);
}


/* ========================================================================= */
/* 関数名：	CDXAudio::SetResourceHandle										 */
/* 内容：	リソースハンドルの設定											 */
/* 日付：	2003/09/01														 */
/* ========================================================================= */

void CDXAudio::SetResourceHandle(HMODULE hResource)
{
	m_hResource = hResource;
}


/* ========================================================================= */
/* 関数名：	CDXAudio::GetSegFromRes											 */
/* 内容：	リソースからのセグメント読み込み								 */
/* 日付：	2003/03/16														 */
/* ========================================================================= */

BOOL CDXAudio::GetSegFromRes(HRSRC hSrc, IDirectMusicSegment8 **pSeg, BOOL bMidi)
{
	HRESULT hr;
	DMUS_OBJECTDESC ObjDesc;
	IDirectMusicSegment8 *pTmp;

	/* リソースを読み込み */
	HGLOBAL hRes = LoadResource (m_hResource, hSrc);

	/* 構造体を設定 */
	ObjDesc.dwSize		= sizeof (DMUS_OBJECTDESC);
	ObjDesc.guidClass	= CLSID_DirectMusicSegment;
	ObjDesc.dwValidData	= DMUS_OBJ_CLASS | DMUS_OBJ_MEMORY;
	ObjDesc.pbMemData	= (BYTE *)LockResource (hRes);
	ObjDesc.llMemLength	= SizeofResource (m_hResource, hSrc);

	/* セグメントを読み込み */
	pTmp = *pSeg;

	hr = m_pLoader->GetObject (&ObjDesc, IID_IDirectMusicSegment8, (void **)&pTmp);
	if (FAILED (hr)) {
		return FALSE;
	}

	if (bMidi) {
		pTmp->SetParam (GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL);
	}

	/* バンドをダウンロード */
	hr = pTmp->Download (m_pPerformance);
	if (FAILED (hr)) {
		return FALSE;
	}

	*pSeg = pTmp;
	return TRUE;
}


/* ========================================================================= */
/* 関数名：	CDXAudio::ReleaseSeg											 */
/* 内容：	セグメント開放													 */
/* 日付：	2003/03/16														 */
/* ========================================================================= */

void CDXAudio::ReleaseSeg(IDirectMusicSegment8 *pSeg)
{
	if (pSeg == NULL) {
		return;
	}

	pSeg->Unload (m_pPerformance);
	SAFE_RELEASE (pSeg);
}


/* ========================================================================= */
/* 関数名：	CDXAudio::PlayPrimary											 */
/* 内容：	プライマリセグメントとして再生									 */
/* 日付：	2003/07/13														 */
/* ========================================================================= */

BOOL CDXAudio::PlayPrimary(IDirectMusicSegment8 *pSeg)
{
	BOOL bRet;
	HRESULT hr;

	bRet = FALSE;
	if (m_pPerformance == NULL) {
		goto Exit;
	}

	hr = m_pPerformance->PlaySegmentEx (
				pSeg,
				NULL, NULL, 0, 0, NULL, NULL,
				m_pDefAudioPath2);
	if (FAILED (hr)) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名：	CDXAudio::PlaySecoundary										 */
/* 内容：	セカンダリセグメントとして再生									 */
/* 日付：	2003/03/16														 */
/* ========================================================================= */

BOOL CDXAudio::PlaySecoundary(IDirectMusicSegment8 *pSeg)
{
	BOOL bRet;
	HRESULT hr;

	bRet = FALSE;
	if (m_pPerformance == NULL) {
		goto Exit;
	}
	if (m_pDefAudioPath == NULL) {
		goto Exit;
	}

	hr = m_pPerformance->PlaySegmentEx (
				pSeg,
				NULL, NULL,
				DMUS_SEGF_SECONDARY,
				0, NULL, NULL,
				m_pDefAudioPath);
	if (FAILED (hr)) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名：	CDXAudio::SetVolPrimary											 */
/* 内容：	プライマリセグメントの音量設定									 */
/* 日付：	2003/11/30														 */
/* ========================================================================= */

void CDXAudio::SetVolPrimary(long lVol)
{
	if (m_pDefAudioPath2 == NULL) {
		return;
	}
	lVol = max (lVol, -96);
	lVol = min (lVol, 0);
	lVol *= 100;
	m_pDefAudioPath2->SetVolume (lVol, 0);
}


/* ========================================================================= */
/* 関数名：	CDXAudio::SetVolSecoundary										 */
/* 内容：	セカンダリセグメントの音量設定									 */
/* 日付：	2003/11/30														 */
/* ========================================================================= */

void CDXAudio::SetVolSecoundary(long lVol)
{
	if (m_pDefAudioPath == NULL) {
		return;
	}
	lVol = max (lVol, -100);
	lVol = min (lVol, 10);
	lVol *= 100;
	m_pDefAudioPath->SetVolume (lVol, 0);
}


/* ========================================================================= */
/* 関数名：	CDXAudio::Stop													 */
/* 内容：	再生停止														 */
/* 日付：	2003/03/16														 */
/* ========================================================================= */

void CDXAudio::Stop(IDirectMusicSegment8 *pSeg, DWORD dwFlg)
{
	if (m_pPerformance == NULL) {
		return;
	}

	if (dwFlg == 1) {
		m_pPerformance->StopEx (m_pDefAudioPath2, 0, 0);
		dwFlg = 0;
	}
	/* 再生停止 */
	m_pPerformance->StopEx (pSeg, 0, dwFlg);
}


/* ========================================================================= */
/* 関数名：	CDXAudio::SetLoopPoints											 */
/* 内容：	ループ範囲の設定												 */
/* 日付：	2003/03/22														 */
/* ========================================================================= */

void CDXAudio::SetLoopPoints(IDirectMusicSegment8 *pSeg, DWORD dwFlg)
{
	if (pSeg == NULL) {
		return;
	}

	pSeg->SetLoopPoints (0, 0);
	pSeg->SetRepeats (dwFlg);
}


/* ========================================================================= */
/* 関数名：	CDXAudio::IsPlaying												 */
/* 内容：	再生中判定														 */
/* 日付：	2003/05/05														 */
/* ========================================================================= */

BOOL CDXAudio::IsPlaying(IDirectMusicSegment8 *pSeg)
{
	BOOL bRet;

	bRet = FALSE;
	if (pSeg == NULL) {
		goto Exit;
	}

	if (m_pPerformance->IsPlaying (pSeg, 0) != S_OK) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2003 */
