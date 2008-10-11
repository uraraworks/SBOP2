/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DataSlot.h													 */
/* 内容			:データスロットクラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/07													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* 構造体の定義																 */
/* ========================================================================= */

/* データスロット情報 */
typedef struct _DATASLOTINFO {
	DWORD		dwID;					/* データID */
	DWORD		dwSize;					/* データサイズ */
	PBYTE		pData;					/* データバッファ */
} DATASLOTINFO, *PDATASLOTINFO;


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDataSlot
{
public:
			CDataSlot();					/* コンストラクタ */
	virtual ~CDataSlot();					/* デストラクタ */

	virtual void Destroy(void);						/* 破棄 */

	/* ユーザデータのアクセス */
	DWORD	Add			(DWORD dwSize, PBYTE pSrc);					/* データ追加 */
	DWORD	AddDelete	(DWORD dwSize, PBYTE *pSrc);				/* データ追加してバッファを削除 */
	DWORD	GetSize		(DWORD dwID);								/* データサイズ取得 */
	BOOL	Copy		(DWORD dwID, LPBYTE pDst, BOOL bDelete);	/* データ取得 */
	PBYTE	GetPtr		(DWORD dwID);								/* データアドレス取得 */
	void	Delete		(DWORD dwID);								/* データ削除 */
	void	Lock		(void);										/* データのロック */
	void	UnLock		(void);										/* データのロック解除 */


protected:
	int GetID(DWORD dwID);							/* テーブルID取得 */


protected:
	CmySection		m_CritData,						/* データアクセス時のクリティカルセクション */
					m_CritDataLock;					/* データロック用のクリティカルセクション */
	DWORD			m_dwNewID;						/* データIDテンポラリ */

	CmyArray<DATASLOTINFO, DATASLOTINFO> m_DataSlot;	/* データスロット */
} CDataSlot, *PCDataSlot;

/* Copyright(C)URARA-works 2008 */
