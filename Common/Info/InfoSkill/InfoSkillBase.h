/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoSkillBase.h											 */
/* 内容			:スキル情報基底クラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/03													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* スキル種別 */
enum {
	SKILLTYPE_NONE = 0,
	SKILLTYPE_FISHING,			/* 釣り */
	SKILLTYPE_MAX
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

	virtual DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	virtual PBYTE	GetSendData			(void);								/* 送信データを取得 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	virtual void	Copy				(CInfoSkillBase *pSrc);			/* コピー */


public:
	int			m_nElementCountBase;			/* 基底クラスの要素数 */

	DWORD		m_dwSkillID,					/* スキルID */
				m_dwSP;							/* 消費SP */
	int			m_nType;						/* スキル種別 */
	CmyString	m_strName;						/* スキル名 */
} CInfoSkillBase, *PCInfoSkillBase;
typedef CmyArray<PCInfoSkillBase, PCInfoSkillBase>	  ARRAYSKILLBASEINFO;
typedef CmyArray<PCInfoSkillBase, PCInfoSkillBase>	*PARRAYSKILLBASEINFO;

/* Copyright(C)URARA-works 2008 */
