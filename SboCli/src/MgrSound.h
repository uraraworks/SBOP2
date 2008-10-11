/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名：	MgrSound.h													 */
/* 内容：		サウンドデータ管理クラス 定義ファイル						 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2006/05/09													 */
/* ========================================================================= */

#pragma once

#include <dmusici.h>
#include "Audiere.h"
using namespace audiere;
#include "SboSoundData.h"

class CDXAudio;
class CLibMusicLoader;
class CLibSboSoundLoader;

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */
/* BGM */
enum {
	BGMID_NONE = 0,
	BGMID_DAICHI_S,					/* daichi_s.ogg */
	BGMID_HISYOU,					/* hisyou.ogg */
	BGMID_SUISHA,					/* suisha.ogg */
	BGMID_FAIRYTALE,				/* fairytale.ogg */
	BGMID_TABLA_IMAGE,				/* tabla_image.ogg */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CMgrSound
{
public:
			CMgrSound();						/* コンストラクタ */
	virtual ~CMgrSound();						/* デストラクタ */

	BOOL	Create		(void);							/* 作成 */
	void	Destroy		(void);							/* 破棄 */

	void	PlaySound		(DWORD dwSoundID);			/* 効果音の再生 */
	void	PlayBGM			(int nNo);					/* BGMの再生 */
	void	StopBGM			(void);						/* BGMの停止 */
	void	SetBGMVolume	(int nVolume);				/* BGM音量設定 */
	void	SetSEVolume		(int nVolume);				/* 効果音量設定 */


protected:
	void	ReadSoundData	(void);						/* 効果音を読み込み */


protected:
	int						m_SEVolume;					/* 効果音量 */
	float					m_fBGMVolume;				/* BGM音量 */
	HMODULE					m_hDllSoundData;			/* 音関連DLLのハンドル */
	CDXAudio				*m_pDXAudio;				/* サウンドライブラリ */
	CLibMusicLoader			*m_pLibMusicLoader;			/* MIDI再生ライブラリ */
	CLibSboSoundLoader		*m_pLibSboSoundLoader;		/* サウンドデータライブラリ */
	IDirectMusicSegment8	**m_apDMSSound;				/* 効果音 */
	AudioDevicePtr			*m_pDevicePtr;				/* BGM用デバイス */
	OutputStream			*m_pOutputStreamPtr;		/* 再生するストリーム */
} CMgrSound, *PCMgrSound;

/* Copyright(C)URARA-works 2006 */
