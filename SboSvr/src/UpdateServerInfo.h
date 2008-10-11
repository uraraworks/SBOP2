/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:UpdateServerInfo.h											 */
/* 内容			:サーバー情報更新クラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/05/22													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CUpdateServerInfo
{
public:
			CUpdateServerInfo();					/* コンストラクタ */
	virtual ~CUpdateServerInfo();					/* デストラクタ */

	BOOL	Create		(LPCSTR pszAccount, LPCSTR pszPassword, LPCSTR pszServerAddr, LPCSTR pszUploadPath);	/* 作成 */
	void	Destroy		(void);								/* 破棄 */
	void	Update		(int nCount, LPCSTR pszPlace, BOOL bUpload = FALSE);		/* 更新 */


protected:
	static UINT __stdcall ThreadEntry(LPVOID lpParam);	/* スレッドメイン処理 */
	void	ThreadMain		(void);							/* スレッドメイン処理 */
	void	MakeServerInfo	(void);							/* サーバー情報作成 */
	void	Upload			(void);							/* アップロード */
	void	GetFileName		(CmyString &strDst);			/* サーバー情報ファイル名を取得 */


protected:
	HANDLE		m_hThread,			/* スレッドハンドル */
				m_hInitEvent,		/* 開始待ちイベント */
				m_hExitEvent,		/* 終了待ちイベント */
				m_hUpdate;			/* 更新イベント */
	CmyString	m_strAccount,		/* FTPアカウント */
				m_strPassword,		/* FTPパスワード */
				m_strServerAddr,	/* サーバーアドレス */
				m_strUploadPath;	/* アップロード先 */
	CmySection	m_csUpdate;			/* 情報更新用クリティカルセクション */

	int			m_nCount;			/* ログイン人数 */
	CmyString	m_strPlace;			/* 最も集まっている場所 */
};

/* Copyright(C)URARA-works 2008 */
