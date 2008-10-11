/* Copyright(C)URARA-works 2003-2005 */
/* ========================================================================= */
/* ファイル名：	DXAudio.h													 */
/* 内容：		DirectX Audioを使うためのクラス								 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2003/03/16													 */
/* ========================================================================= */

#pragma once

#include <dmusici.h>

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDXAudio
{
public:
			CDXAudio	();										/* コンストラクタ */
	virtual	~CDXAudio	();										/* デストラクタ */

	BOOL Create				(void);										/* 初期化 */
	void Destroy			(void);										/* 終了 */
	void SetResourceHandle	(HMODULE hResource);						/* リソースハンドルの設定 */

	BOOL GetSegFromRes		(HRSRC hSrc, IDirectMusicSegment8 **pSeg, BOOL bMidi = FALSE);	/* リソースからのセグメント読み込み */
	void ReleaseSeg			(IDirectMusicSegment8 *pSeg);				/* セグメントの開放 */
	BOOL PlayPrimary		(IDirectMusicSegment8 *pSeg);				/* プライマリセグメントとして再生 */
	BOOL PlaySecoundary		(IDirectMusicSegment8 *pSeg);				/* セカンダリセグメントとして再生 */
	void SetVolPrimary		(long lVol);								/* プライマリセグメントの音量設定 */
	void SetVolSecoundary	(long lVol);								/* セカンダリセグメントの音量設定 */
	void Stop				(IDirectMusicSegment8 *pSeg, DWORD dwFlg);	/* 再生停止 */
	void SetLoopPoints		(IDirectMusicSegment8 *pSeg, DWORD dwFlg);	/* ループ範囲の設定 */
	BOOL IsPlaying			(IDirectMusicSegment8 *pSeg);				/* 再生中判定 */


private:
	IDirectMusicPerformance8	*m_pPerformance;	/* パフォーマンスオブジェクト */
	IDirectMusicAudioPath8		*m_pDefAudioPath;	/* オーディオパス */
	IDirectMusicAudioPath8		*m_pDefAudioPath2;	/* オーディオパス2 */
	IDirectMusicLoader8			*m_pLoader;			/* ローダー */
	HMODULE						m_hResource;		/* リソースハンドル */
} CDXAudio, *PCDXAudio;

/* Copyright(C)URARA-works 2003 */
