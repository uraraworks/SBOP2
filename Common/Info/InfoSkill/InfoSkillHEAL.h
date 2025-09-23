/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:InfoSkillHEAL.h											 */
/* 内容			:スキル情報(回復)クラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/01/22													 */
/* ========================================================================= */

#pragma once
#include <vector>
#include "myArray.h"

#include "InfoSkillBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* 範囲 */
enum {
	SKILLHEAL_AREA_NONE = 0,		/* 自分位置 */
	SKILLHEAL_AREA_FRONT,			/* 前方 */
	SKILLHEAL_AREA_AREA,			/* 周囲 */
	SKILLHEAL_AREA_MAX
};

/* 回復種別 */
enum {
	SKILLHEAL_HEALTYPE_NONE = 0,	/* 未設定 */
	SKILLHEAL_HEALTYPE_HP,			/* HP */
	SKILLHEAL_HEALTYPE_SP,			/* SP */
	SKILLHEAL_HEALTYPE_MAX
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoSkillHEAL : public CInfoSkillBase
{
public:
			CInfoSkillHEAL();										/* コンストラクタ */
	virtual ~CInfoSkillHEAL();										/* デストラクタ */

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
	DWORD	m_dwArea,				/* 範囲 */
			m_dwHealType,			/* 回復種別 */
			m_dwHitEffectID,		/* ヒット時の表示エフェクト */
			m_dwValue1,				/* 効果1 */
			m_dwValue2,				/* 効果2 */
			m_dwDistance;			/* 射程距離 */
} CInfoSkillHEAL, *PCInfoSkillHEAL;
using ARRAYSKILLHEALINFO = CStdArray<PCInfoSkillHEAL>;
using PARRAYSKILLHEALINFO = ARRAYSKILLHEALINFO *;

/* Copyright(C)URARA-works 2009 */
