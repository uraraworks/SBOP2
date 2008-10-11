/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:LibInfoAccount.h											 */
/* 内容			:アカウント情報ライブラリ基底クラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "InfoAccount.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoAccount : public CLibInfoBase
{
public:
			CLibInfoAccount();								/* コンストラクタ */
	virtual ~CLibInfoAccount();								/* デストラクタ */

	void Create			(void);										/* 作成 */
	void Destroy		(void);										/* 破棄 */
	BOOL CheckPassword	(LPCSTR pszAccount, LPCSTR pszPassword);	/* パスワードのチェック */
	BOOL IsUseMacAddr	(LPCSTR pszMacAddr);						/* MACアドレスが使用済みかチェック */

	virtual PCInfoBase GetNew	(void);								/* 新規データを取得 */

	int		GetCount		(void);									/* データ数を取得 */
	void	Add				(PCInfoBase pInfo);						/* 追加 */
	void	Delete			(int nNo);								/* 削除 */
	void	Delete			(DWORD dwAccountID);					/* 削除 */
	void	DeleteAll		(void);									/* 全て削除 */
	DWORD	GetAccountID	(DWORD dwCharID);						/* キャラIDからアカウントIDを取得 */

	PCInfoBase		GetPtr			(int nNo);						/* アカウント情報を取得 */
	PCInfoAccount	GetPtr			(DWORD dwAccountID);			/* アカウント情報を取得 */
	PCInfoAccount	GetPtr			(LPCSTR pszAccount);			/* アカウント情報を取得 */
	PCInfoAccount	GetPtrSessionID	(DWORD dwSessionID);			/* アカウント情報を取得 */


protected:
	DWORD	GetNewID	(void);									/* 新しいアカウントIDを取得 */


protected:
	PARRAYINFOACCOUNT	m_paInfo;				/* アカウント情報 */
} CLibInfoAccount, *PCLibInfoAccount;

/* Copyright(C)URARA-works 2006 */
