/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoTalkEventPAGE.h										 */
/* 内容			:会話イベント情報(ページ切り替え)クラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/29													 */
/* ========================================================================= */

#pragma once

#include "InfoTalkEventBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ページ切り替え条件 */
enum {
	CHGPAGECONDITION_NONE = 0,
	CHGPAGECONDITION_ITEM,		/* アイテムあり */
	CHGPAGECONDITION_NOITEM,	/* アイテムなし */
	CHGPAGECONDITION_MAX
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoTalkEventPAGE : public CInfoTalkEventBase
{
public:
			CInfoTalkEventPAGE();									/* コンストラクタ */
	virtual ~CInfoTalkEventPAGE();									/* デストラクタ */

	virtual int		GetElementCount		(void);								/* 要素数を取得 */
	virtual int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	virtual DWORD	GetDataSize			(void);								/* データサイズを取得 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	virtual LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	virtual DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	virtual PBYTE	GetSendData			(void);								/* 送信データを取得 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	virtual void	Copy				(CInfoTalkEventBase *pSrc);			/* コピー */


public:
	int		m_nPageChgCondition,			/* ページ切り替え条件 */
			m_nPageJump;					/* ページ切り替え先 */
} CInfoTalkEventPAGE, *PCInfoTalkEventPAGE;

/* Copyright(C)URARA-works 2008 */
