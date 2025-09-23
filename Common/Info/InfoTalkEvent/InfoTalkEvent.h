/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoTalkEvent.h											 */
/* 内容			:会話イベント情報クラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/16													 */
/* ========================================================================= */

#pragma once
#include <vector>
#include "myArray.h"

#include "InfoTalkEventBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoTalkEvent : public CInfoBase
{
public:
			CInfoTalkEvent();										/* コンストラクタ */
	virtual ~CInfoTalkEvent();										/* デストラクタ */

	virtual int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	virtual DWORD	GetDataSize			(void);								/* データサイズを取得 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	virtual LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	virtual DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	virtual PBYTE	GetSendData			(void);								/* 送信データを取得 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	virtual void	Copy				(CInfoTalkEvent *pSrc);				/* コピー */

	PCInfoTalkEventBase	GetNew			(int nType);									/* 新しい会話イベントを取得 */
	PCInfoTalkEventBase	GetPtr			(int nPage, int nNo);							/* 会話イベントを取得 */
	void	SetPtr						(int nPage, int nNo, CInfoTalkEventBase *pInfo);/* 会話イベントを差し替え */
	void	GetEventArray				(int nPage, ARRAYTALKEVENTBASEINFO &aDst);		/* 指定ページ番号のイベント一覧を取得 */
	int		GetTalkEventCount			(int nPage);						/* 会話イベントページ数を取得 */
	int		GetPageCount				(void);								/* 会話イベントページ数を取得 */
	void	AddTalkEvent				(CInfoTalkEventBase *pInfo);		/* 会話イベントを追加 */
	void	UpTalkEvent					(int nPage, int nNo);				/* 指定イベントを1つ上に移動 */
	void	DownTalkEvent				(int nPage, int nNo);				/* 指定イベントを1つ下に移動 */
	void	DeleteTalkEvent				(int nNo);							/* 会話イベントを削除 */
	void	DeleteTalkEvent				(int nPage, int nNo);				/* 会話イベントを削除 */
	void	DeleteTalkEvent				(CInfoTalkEventBase *pInfo);		/* 会話イベントを削除 */
	void	DeleteAllTalkEvent			(void);								/* 会話イベントを全て削除 */


public:
	DWORD	m_dwTalkEventID;						/* 会話イベントID */
	ARRAYTALKEVENTBASEINFO		m_apTalkEvent;		/* 会話イベント */
} CInfoTalkEvent, *PCInfoTalkEvent;
using ARRAYTALKEVENTINFO = CStdArray<PCInfoTalkEvent>;
using PARRAYTALKEVENTINFO = ARRAYTALKEVENTINFO *;

/* Copyright(C)URARA-works 2008 */
