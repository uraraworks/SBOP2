/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:InfoAccount.cpp											 */
/* 内容			:アカウント情報基底クラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/04													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoAccount.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_bDisable",			/* ログイン拒否 */
	"dwAccountID",			/* アカウントID */
	"dwCharID",				/* キャラID */
	"dwTimeLastLogin",		/* 前回のログイン日時 */
	"dwTimeMakeAccount",	/* アカウント作成日時 */
	"dwLoginCount",			/* ログイン回数 */
	"adwCharID",			/* キャラIDテーブル */
	"nAdminLevel",			/* 管理者レベル */
	"strAccount",			/* アカウント */
	"strPassword",			/* パスワード */
	"m_strMacAddr",			/* アカウント登録MACアドレス */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoAccount::CInfoAccount										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/11/04														 */
/* ========================================================================= */

CInfoAccount::CInfoAccount()
{
	m_bDisable			= FALSE;
	m_dwLastKeepalive	= 0;
	m_dwIP				= 0;
	m_dwAccountID		= 0;
	m_dwCharID			= 0;
	m_dwTimeLastLogin	= 0;
	m_dwTimeMakeAccount	= 0;
	m_dwLoginCount		= 0;

	m_nAdminLevel		= ADMINLEVEL_NONE;
	m_dwSessionID		= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoAccount::~CInfoAccount									 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/11/04														 */
/* ========================================================================= */

CInfoAccount::~CInfoAccount()
{
}


/* ========================================================================= */
/* 関数名	:CInfoAccount::GetElementNo										 */
/* 内容		:要素番号を取得													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

int CInfoAccount::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = -1;

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoAccount::GetDataSize										 */
/* 内容		:データサイズを取得												 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

DWORD CInfoAccount::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_bDisable);
	dwRet += sizeof (m_dwAccountID);
	dwRet += sizeof (m_dwCharID);
	dwRet += sizeof (m_dwTimeLastLogin);
	dwRet += sizeof (m_dwTimeMakeAccount);
	dwRet += sizeof (m_dwLoginCount);
	dwRet += ((m_adwCharID.GetSize () + 1) * sizeof (DWORD));
	dwRet += sizeof (m_nAdminLevel);
	dwRet += (m_strAccount.GetLength () + 1);
	dwRet += (m_strPassword.GetLength () + 1);
	dwRet += (m_strMacAddr.GetLength () + 1);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoAccount::GetDataSizeNo									 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

DWORD CInfoAccount::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof (m_bDisable);								break;
	case 1:	dwRet = sizeof (m_dwAccountID);								break;
	case 2:	dwRet = sizeof (m_dwCharID);								break;
	case 3:	dwRet = sizeof (m_dwTimeLastLogin);							break;
	case 4:	dwRet = sizeof (m_dwTimeMakeAccount);						break;
	case 5:	dwRet = sizeof (m_dwLoginCount);							break;
	case 6:	dwRet = ((m_adwCharID.GetSize () + 1) * sizeof (DWORD));	break;
	case 7:	dwRet = sizeof (m_nAdminLevel);								break;
	case 8:	dwRet = (m_strAccount.GetLength () + 1);					break;
	case 9:	dwRet = (m_strPassword.GetLength () + 1);					break;
	case 10:dwRet = (m_strMacAddr.GetLength () + 1);					break;		/* アカウント登録MACアドレス */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoAccount::GetName											 */
/* 内容		:要素名を取得													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

LPCSTR CInfoAccount::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoAccount::GetWriteData										 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

PBYTE CInfoAccount::GetWriteData(int nNo, PDWORD pdwSize)
{
	PBYTE pRet, pSrc;
	DWORD dwSize;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:	pSrc = (PBYTE)&m_bDisable;			break;
	case 1:	pSrc = (PBYTE)&m_dwAccountID;		break;
	case 2:	pSrc = (PBYTE)&m_dwCharID;			break;
	case 3:	pSrc = (PBYTE)&m_dwTimeLastLogin;	break;
	case 4:	pSrc = (PBYTE)&m_dwTimeMakeAccount;	break;
	case 5:	pSrc = (PBYTE)&m_dwLoginCount;		break;
	case 6:
		{
			int i, nCount;
			DWORD dwTmp;
			PBYTE pDataTmp;

			pDataTmp = pRet;
			ZeroMemory (pRet, dwSize);
			nCount = m_adwCharID.GetSize ();
			for (i = 0; i < nCount; i ++) {
				dwTmp = m_adwCharID[i];
				CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);
			}
		}
		break;
	case 7:	pSrc = (PBYTE)&m_nAdminLevel;			break;
	case 8:	pSrc = (PBYTE)(LPCSTR)m_strAccount;		break;
	case 9:	pSrc = (PBYTE)(LPCSTR)m_strPassword;	break;
	case 10:pSrc = (PBYTE)(LPCSTR)m_strMacAddr;		break;
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoAccount::ReadElementData									 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

DWORD CInfoAccount::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:	pDst = (PBYTE)&m_bDisable;			dwSize = sizeof (m_bDisable);			break;
	case 1:	pDst = (PBYTE)&m_dwAccountID;		dwSize = sizeof (m_dwAccountID);		break;
	case 2:	pDst = (PBYTE)&m_dwCharID;			dwSize = sizeof (m_dwCharID);			break;
	case 3:	pDst = (PBYTE)&m_dwTimeLastLogin;	dwSize = sizeof (m_dwTimeLastLogin);	break;
	case 4:	pDst = (PBYTE)&m_dwTimeMakeAccount;	dwSize = sizeof (m_dwTimeMakeAccount);	break;
	case 5:	pDst = (PBYTE)&m_dwLoginCount;		dwSize = sizeof (m_dwLoginCount);		break;
	case 6:
		{
			DWORD dwTmp;
			PBYTE pDataTmp;

			pDataTmp = pSrc;
			m_adwCharID.RemoveAll ();
			while (1) {
				dwSize += sizeof (DWORD);
				CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);
				if (dwTmp == 0) {
					break;
				}
				m_adwCharID.Add (dwTmp);
			}
		}
		break;
	case 7:	pDst = (PBYTE)&m_nAdminLevel;		dwSize = sizeof (m_nAdminLevel);		break;
	case 8:
		m_strAccount = (LPCSTR)pSrc;
		dwSize = m_strAccount.GetLength () + 1;
		break;
	case 9:
		m_strPassword = (LPCSTR)pSrc;
		dwSize = m_strPassword.GetLength () + 1;
		break;
	case 10:
		m_strMacAddr = (LPCSTR)pSrc;
		dwSize = m_strMacAddr.GetLength () + 1;
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoAccount::GetSendDataSize									 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2006/11/06														 */
/* ========================================================================= */

DWORD CInfoAccount::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_bDisable);
	dwRet += sizeof (m_dwAccountID);
	dwRet += sizeof (m_dwCharID);
	dwRet += sizeof (m_dwTimeLastLogin);
	dwRet += sizeof (m_dwTimeMakeAccount);
	dwRet += sizeof (m_dwLoginCount);
	dwRet += (m_adwCharID.GetSize () * sizeof (DWORD));
	dwRet += sizeof (DWORD);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoAccount::GetSendData										 */
/* 内容		:送信データを取得												 */
/* 日付		:2006/11/06														 */
/* ========================================================================= */

PBYTE CInfoAccount::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwSize, dwTmp;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	pDataTmp	= pData;

	CopyMemoryRenew (pDataTmp, &m_dwAccountID,			sizeof (m_dwAccountID),			pDataTmp);	/* アカウントID */
	CopyMemoryRenew (pDataTmp, &m_dwCharID,				sizeof (m_dwCharID),			pDataTmp);	/* 使用中のキャラID */
	CopyMemoryRenew (pDataTmp, &m_dwTimeLastLogin,		sizeof (m_dwTimeLastLogin),		pDataTmp);	/* 前回のログイン日時 */
	CopyMemoryRenew (pDataTmp, &m_dwTimeMakeAccount,	sizeof (m_dwTimeMakeAccount),	pDataTmp);	/* アカウント作成日時 */
	CopyMemoryRenew (pDataTmp, &m_dwLoginCount,			sizeof (m_dwLoginCount),		pDataTmp);	/* ログイン数 */
	nCount = m_adwCharID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwTmp = m_adwCharID[i];
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);	/* キャラID */
	}

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoAccount::SetSendData										 */
/* 内容		:送信データを設定												 */
/* 日付		:2006/11/06														 */
/* 戻り値	:処理した後のアドレス											 */
/* ========================================================================= */

PBYTE CInfoAccount::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwTmp;

	pRet = pSrc;

	m_adwCharID.RemoveAll ();

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwAccountID,		pDataTmp, sizeof (m_dwAccountID), 		pDataTmp);	/* アカウントID */
	CopyMemoryRenew (&m_dwCharID,			pDataTmp, sizeof (m_dwCharID),			pDataTmp);	/* 使用中のキャラID */
	CopyMemoryRenew (&m_dwTimeLastLogin,	pDataTmp, sizeof (m_dwTimeLastLogin),	pDataTmp);	/* 前回のログイン日時 */
	CopyMemoryRenew (&m_dwTimeMakeAccount,	pDataTmp, sizeof (m_dwTimeMakeAccount),	pDataTmp);	/* アカウント作成日時 */
	CopyMemoryRenew (&m_dwLoginCount,		pDataTmp, sizeof (m_dwLoginCount),		pDataTmp);	/* ログイン数 */

	while (1) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);	/* キャラID */
		if (dwTmp == 0) {
			break;
		}
		m_adwCharID.Add (dwTmp);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoAccount::GetCharCount										 */
/* 内容		:キャラ数を取得													 */
/* 日付		:2007/03/21														 */
/* ========================================================================= */

int CInfoAccount::GetCharCount(void)
{
	return m_adwCharID.GetSize ();
}


/* ========================================================================= */
/* 関数名	:CInfoAccount::Copy												 */
/* 内容		:コピー															 */
/* 日付		:2006/11/08														 */
/* ========================================================================= */

void CInfoAccount::Copy(CInfoAccount *pSrc)
{
	m_dwAccountID		= pSrc->m_dwAccountID;
	m_dwCharID			= pSrc->m_dwCharID;
	m_dwTimeLastLogin	= pSrc->m_dwTimeLastLogin;
	m_dwTimeMakeAccount	= pSrc->m_dwTimeMakeAccount;
	m_dwLoginCount		= pSrc->m_dwLoginCount;
	m_adwCharID.Copy (&pSrc->m_adwCharID);

	m_dwSessionID		= pSrc->m_dwSessionID;
	m_strAccount		= pSrc->m_strAccount;
	m_strPassword		= pSrc->m_strPassword;
	m_strMacAddr		= pSrc->m_strMacAddr;
}


/* ========================================================================= */
/* 関数名	:CInfoAccount::GetTmpData										 */
/* 内容		:データキャッシュ用に取得										 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

PBYTE CInfoAccount::GetTmpData(DWORD &dwDataSize)
{
	PBYTE pRet, pDataTmp;

	pRet = NULL;
	dwDataSize = 0;

	/* サイズを計算 */
	dwDataSize += sizeof (m_dwAccountID);
	dwDataSize += sizeof (m_dwIP);
	dwDataSize += (m_strAccount.GetLength () + 1);
	dwDataSize += (m_strPassword.GetLength () + 1);
	dwDataSize += (m_strLastMacAddr.GetLength () + 1);

	pRet = ZeroNew (dwDataSize);
	pDataTmp = pRet;
	CopyMemoryRenew (pDataTmp, &m_dwAccountID, sizeof (m_dwAccountID), pDataTmp);	/* アカウントID */
	CopyMemoryRenew (pDataTmp, &m_dwIP, sizeof (m_dwIP), pDataTmp);					/* IPアドレス */
	strcpyRenew ((LPSTR)pDataTmp, m_strAccount,  pDataTmp);							/* アカウント */
	strcpyRenew ((LPSTR)pDataTmp, m_strPassword, pDataTmp);							/* パスワード */
	strcpyRenew ((LPSTR)pDataTmp, m_strLastMacAddr, pDataTmp);						/* ログイン時のMACアドレス */

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoAccount::SetTmpData										 */
/* 内容		:データキャッシュから設定										 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CInfoAccount::SetTmpData(PBYTE pSrc)
{
	PBYTE pDataTmp;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	/* アカウントID */
	CopyMemoryRenew (&m_dwIP, pDataTmp, sizeof (m_dwIP), pDataTmp);					/* IPアドレス */
	StoreRenew (m_strAccount,	(LPCSTR)pDataTmp, pDataTmp);						/* アカウント */
	StoreRenew (m_strPassword,	(LPCSTR)pDataTmp, pDataTmp);						/* パスワード */
	StoreRenew (m_strLastMacAddr,	(LPCSTR)pDataTmp, pDataTmp);					/* ログイン時のMACアドレス */
}

/* Copyright(C)URARA-works 2006 */
