/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:InfoAccount.h												 */
/* 内容			:アカウント情報クラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/04													 */
/* ========================================================================= */

#pragma once
#include <vector>
#include "myArray.h"

#include "InfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoAccount : public CInfoBase
{
public:
			CInfoAccount();							/* コンストラクタ */
	virtual ~CInfoAccount();						/* デストラクタ */

	int		GetElementNo		(LPCSTR pszName);			/* 要素番号を取得 */
	DWORD	GetDataSize			(void);						/* データサイズを取得 */
	DWORD	GetDataSizeNo		(int nNo);					/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName				(int nNo);					/* 要素名を取得 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);	/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);		/* 指定要素データを読み込み */

	DWORD	GetSendDataSize	(void);							/* 送信データサイズを取得 */
	PBYTE	GetSendData		(void);							/* 送信データを取得 */
	PBYTE	SetSendData		(PBYTE pSrc);					/* 送信データを設定 */

	int		GetCharCount	(void);							/* キャラ数を取得 */
	void	Copy			(CInfoAccount *pSrc);			/* コピー */

	PBYTE	GetTmpData		(DWORD &dwDataSize);			/* データキャッシュ用に取得 */
	void	SetTmpData		(PBYTE pSrc);					/* データキャッシュから設定 */


public:
	/* 保存しない情報 */
	DWORD		m_dwLastKeepalive,			/* 最後に受信した生存確認通知の時間 */
				m_dwIP;						/* IPアドレス */
	CmyString	m_strLastMacAddr;			/* ログイン時のMACアドレス */

	/* 送受信する情報 */
	DWORD		m_dwAccountID,				/* アカウントID */
				m_dwCharID,					/* 使用中のキャラID */
				m_dwTimeLastLogin,			/* 前回のログイン日時 */
				m_dwTimeMakeAccount,		/* アカウント作成日時 */
				m_dwLoginCount;				/* ログイン回数 */
	ARRAYDWORD	m_adwCharID;				/* キャラIDテーブル */

	/* 送受信せずにサーバー側だけで扱う情報 */
	BOOL		m_bDisable;					/* ログイン拒否 */
	int			m_nAdminLevel;				/* 管理者レベル */
	DWORD		m_dwSessionID;				/* 使用者のセッションID */
	CmyString	m_strAccount,				/* アカウント */
				m_strPassword,				/* パスワード */
				m_strMacAddr;				/* アカウント登録MACアドレス */
} CInfoAccount, *PCInfoAccount;
using ARRAYINFOACCOUNT = CStdArray<PCInfoAccount>;
using PARRAYINFOACCOUNT = ARRAYINFOACCOUNT *;

/* Copyright(C)URARA-works 2006 */
