/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:InfoSkillMOVEATACK.h										 */
/* 内容			:スキル情報(移動して攻撃)クラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/01/08													 */
/* ========================================================================= */

#pragma once

#include "InfoSkillBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* 攻撃対象 */
enum {
	SKILLMOVEATACKTARGETTYPE_NONE = 0,		/* 未設定 */
	SKILLMOVEATACKTARGETTYPE_PC,			/* プレイヤー */
	SKILLMOVEATACKTARGETTYPE_NPC,			/* NPC */
	SKILLMOVEATACKTARGETTYPE_ALL,			/* 全て */
	SKILLMOVEATACKTARGETTYPE_MAX
};

/* 発射種別 */
enum {
	SKILLMOVEATACKPUTTYPE_NONE = 0,			/* 未設定 */
	SKILLMOVEATACKPUTTYPE_FRONT,			/* 前方 */
	SKILLMOVEATACKPUTTYPE_CROSS,			/* 上下左右 */
	SKILLMOVEATACKPUTTYPE_MAX
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoSkillMOVEATACK : public CInfoSkillBase
{
public:
			CInfoSkillMOVEATACK();									/* コンストラクタ */
	virtual ~CInfoSkillMOVEATACK();									/* デストラクタ */

	virtual int		GetElementNoTmp		(LPCSTR pszName);					/* 要素番号を取得 */
	virtual DWORD	GetDataSizeTmp		(void);								/* データサイズを取得 */
	virtual DWORD	GetDataSizeNoTmp	(int nNo);							/* 指定要素のデータサイズを取得 */
	virtual LPCSTR	GetNameTmp			(int nNo);							/* 要素名を取得 */
	virtual PBYTE	GetWriteDataTmp		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	virtual DWORD	ReadElementDataTmp	(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	virtual DWORD	GetDerivationSize		(void);							/* 派生データサイズを取得 */
	virtual PBYTE	GetDerivationWriteData	(PDWORD pdwSize);				/* 派生データの保存用データを取得 */
	virtual DWORD	ReadDerivationData		(PBYTE pSrc);					/* 派生データを読み込み */

	virtual DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	virtual PBYTE	GetSendData			(void);								/* 送信データを取得 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	virtual void	Copy				(CInfoSkillBase *pSrc);				/* コピー */


public:
	int		m_nElementCountDerivation;	/* 要素数 */
	DWORD	m_dwTartgetType,		/* 攻撃対象 */
			m_adwEffectID[4],		/* 向きによる表示エフェクト */
			m_dwPutType,			/* 発射種別 */
			m_dwAliveTime,			/* 耐久時間 */
			m_dwWaitTime,			/* 移動速度 */
			m_dwValue,				/* 効果 */
			m_dwDistance;			/* 射程距離 */
	BOOL	m_bHitQuit;				/* ヒットすると消滅 */
} CInfoSkillMOVEATACK, *PCInfoSkillMOVEATACK;
typedef CmyArray<PCInfoSkillMOVEATACK, PCInfoSkillMOVEATACK>	  ARRAYSKILLMOVEATACKINFO;
typedef CmyArray<PCInfoSkillMOVEATACK, PCInfoSkillMOVEATACK>	*PARRAYSKILLMOVEATACKINFO;

/* Copyright(C)URARA-works 2009 */
