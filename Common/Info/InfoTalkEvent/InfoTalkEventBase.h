/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoTalkEventBase.h										 */
/* 内容			:会話イベント情報基底クラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/16													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* 会話イベント種別 */
enum {
	TALKEVENTTYPE_NONE = 0,
	TALKEVENTTYPE_PAGE,			/* ページ切り替え */
	TALKEVENTTYPE_MSG,			/* メッセージ表示 */
	TALKEVENTTYPE_MENU,			/* 項目選択 */
	TALKEVENTTYPE_ADDSKILL,		/* スキル追加 */
	TALKEVENTTYPE_MAX
};

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoTalkEventBase : public CInfoBase
{
public:
			CInfoTalkEventBase();									/* コンストラクタ */
	virtual ~CInfoTalkEventBase();									/* デストラクタ */

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
	int			m_nElementCountBase,			/* 基底クラスの要素数 */
				m_nEventType,					/* 会話イベント種別 */
				m_nPage;						/* 所属ページ番号 */
	DWORD		m_dwData;						/* バイナリデータ */
	CmyString	m_strText;						/* 文字列データ */
} CInfoTalkEventBase, *PCInfoTalkEventBase;
typedef CmyArray<PCInfoTalkEventBase, PCInfoTalkEventBase>	  ARRAYTALKEVENTBASEINFO;
typedef CmyArray<PCInfoTalkEventBase, PCInfoTalkEventBase>	*PARRAYTALKEVENTBASEINFO;

/* Copyright(C)URARA-works 2008 */
