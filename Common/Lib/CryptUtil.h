/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:CryptUtil.h												 */
/* 内容			:暗号復号クラス 定義ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/02													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */
class CCryptUtil
{
public:
			CCryptUtil();											/* コンストラクタ */
	virtual ~CCryptUtil();											/* デストラクタ */

	void	Crypt		(PBYTE pSrc, PBYTE pDst, DWORD dwSize, int nKeyNo);	/* 暗号化 */
	void	UnCrypt		(PBYTE pSrc, PBYTE pDst, DWORD dwSize, int nKeyNo);	/* 復号化 */
	void	CryptStr	(LPCSTR pszSrc, LPSTR pszDst, int nKeyNo);			/* 暗号化(文字列) */
	void	UnCryptStr	(LPCSTR pszSrc, LPSTR pszDst, int nKeyNo);			/* 復号化(文字列) */


protected:
	void	Init	(void);				/* 初期化 */


protected:
	PBYTE	m_pCryptData;				/* 暗号用データ */
};

/* Copyright(C)URARA-works 2007 */
