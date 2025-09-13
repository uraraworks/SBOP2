/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:LibInfoDisable.h											 */
/* 内容			:拒否情報ライブラリ基底クラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/04/08													 */
/* ========================================================================= */

#pragma once

#include "InfoDisable.h"
#include "LibInfoBase.h"

/* IPアドレス拒否情報 */
typedef struct _STDISABLEIPADDRESS {
	ULONG	ulIPAddress;		/* IPアドレス */
	DWORD	dwLastTime;			/* 最終ログイン時間 */
} STDISABLEIPADDRESS, *PSTDISABLEIPADDRESS;
typedef CmyArray<PSTDISABLEIPADDRESS, PSTDISABLEIPADDRESS>   ARRAYDISABLEIPADDRESS;
typedef CmyArray<PSTDISABLEIPADDRESS, PSTDISABLEIPADDRESS> *PARRAYDISABLEIPADDRESS;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoDisable : public CLibInfoBase
{
public:
			CLibInfoDisable();								/* コンストラクタ */
	virtual ~CLibInfoDisable();								/* デストラクタ */

	void Create			(void);										/* 作成 */
	void Destroy		(void);										/* 破棄 */
	BOOL Proc			(void);										/* 処理 */

	PCInfoBase GetNew	(void);										/* 新規データを取得 */

	int		GetCount	(void);										/* データ数を取得 */
	void	Add			(PCInfoBase pInfo);							/* 追加 */
	void	Delete		(int nNo);									/* 削除 */
	void	Delete		(DWORD dwDisableID);						/* 削除 */
	void	DeleteAll	(void);										/* 全て削除 */
	void	CopyAll		(CLibInfoDisable *pSrc);					/* 全てコピー */
	void	Merge		(CLibInfoDisable *pSrc);					/* 取り込み */
	BOOL	IsDisable	(LPCSTR pszMacAddress);						/* 拒否情報が登録されているか判定 */

	PCInfoBase	GetPtr (int nNo);									/* 拒否情報を取得 */
	PCInfoBase	GetPtr (DWORD dwDisableID);							/* 拒否情報を取得 */
	PCInfoBase	GetPtr (LPCSTR pszMacAddress);						/* 拒否情報を取得 */

	DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);								/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	/* IPアドレス関連 */
	void	AddIP		(ULONG ulIPAddress);						/* 追加 */
	void	DeleteIP	(int nNo);									/* 削除 */
	void	DeleteAllIP	(void);										/* 全て削除 */
	BOOL	IsDisableIP	(ULONG ulIPAddress);						/* 禁止されているIPアドレスか判定 */


protected:
	DWORD	GetNewID	(void);										/* 新しい拒否IDを取得 */


protected:
	DWORD	m_dwNewIDTmp,						/* 新規ID作成用 */
			m_dwLastTimeProc;					/* 最後の処理時間 */

	PARRAYDISABLEINFO		m_paInfo;			/* 拒否情報 */
	PARRAYDISABLEIPADDRESS	m_paInfoIPADdress;	/* 拒否IPアドレス情報 */
} CLibInfoDisable, *PCLibInfoDisable;

/* Copyright(C)URARA-works 2009 */
