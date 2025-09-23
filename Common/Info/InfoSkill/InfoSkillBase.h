/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoSkillBase.h											 */
/* 内容			:スキル情報基底クラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/03													 */
/* ========================================================================= */

#pragma once
#include <vector>
#include "myArray.h"

#include "InfoBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* スキル種別(メイン) */
enum {
	SKILLTYPEMAIN_NONE = 0,			/* 能力 */
	SKILLTYPEMAIN_BATTLE,			/* 戦闘 */
	SKILLTYPEMAIN_LIFE,				/* 生活 */
	SKILLTYPEMAIN_MAX
};

/* スキル種別(サブ:能力) */
enum {
	SKILLTYPESUB_NONE_NONE = 0,
};

/* スキル種別(サブ:戦闘) */
enum {
	SKILLTYPESUB_BATTLE_NONE = 0,
	SKILLTYPESUB_BATTLE_MOVEATACK,	/* 移動して攻撃 */
	SKILLTYPESUB_BATTLE_HEAL,		/* 回復 */
	SKILLTYPESUB_BATTLE_MAX
};

/* スキル種別(サブ:生活) */
enum {
	SKILLTYPESUB_LIFE_NONE = 0,
	SKILLTYPESUB_LIFE_FISHING,		/* 釣り */
	SKILLTYPESUB_LIFE_MAX
};

/* 使用制限 */
enum {
	SKILLUSE_ANY = 0,			/* 制限無し */
	SKILLUSE_NORMAL,			/* 通常時 */
	SKILLUSE_BATTLE,			/* 戦闘モード時 */
	SKILLUSE_MAX
};

/* クラス種別 */
enum {
	INFOSKILLTYPE_BASE = 0,		/* 基底 */
	INFOSKILLTYPE_MOVEATACK,	/* 移動して攻撃 */
	INFOSKILLTYPE_HEAL,			/* 回復 */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoSkillBase : public CInfoBase
{
public:
			CInfoSkillBase();									/* コンストラクタ */
	virtual ~CInfoSkillBase();									/* デストラクタ */

	virtual int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	virtual DWORD	GetDataSize			(void);								/* データサイズを取得 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	virtual LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	virtual DWORD	GetDerivationSize		(void);							/* 派生データサイズを取得 */
	virtual PBYTE	GetDerivationWriteData	(PDWORD pdwSize);				/* 派生データの保存用データを取得 */
	virtual DWORD	ReadDerivationData		(PBYTE pSrc);					/* 派生データを読み込み */

	virtual DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	virtual PBYTE	GetSendData			(void);								/* 送信データを取得 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	virtual void	Copy				(CInfoSkillBase *pSrc);			/* コピー */


public:
	int			m_nElementCountBase,			/* 基底クラスの要素数 */
				m_nClassType;					/* クラス種別 */

	DWORD		m_dwSkillID,					/* スキルID */
				m_dwSP,							/* 消費SP */
				m_dwIconID;						/* アイコン画像ID */
	int			m_nTypeMain,					/* スキル種別(メイン) */
				m_nTypeSub,						/* スキル種別(サブ) */
				m_nUse;							/* 使用制限 */
	CmyString	m_strName;						/* スキル名 */
} CInfoSkillBase, *PCInfoSkillBase;
using ARRAYSKILLBASEINFO = CStdArray<PCInfoSkillBase>;
using PARRAYSKILLBASEINFO = ARRAYSKILLBASEINFO *;

/* Copyright(C)URARA-works 2008 */
