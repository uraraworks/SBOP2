/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoBase.h											 */
/* 内容			:データ保存・読み込み基底クラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2005/04/26													 */
/* ========================================================================= */

#pragma once

#include "LibInfoBase.h"

class CInfoBase;

/* ========================================================================= */
/* 構造体定義																 */
/* ========================================================================= */

/* ヘッダ情報 */
typedef struct _HEADERINFO {
	char		szName[64];							/* カラム名 */
	DWORD		dwOffset;							/* オフセット */
	DWORD		dwDataSize;							/* データサイズ */
} HEADERINFO, *PHEADERINFO;

typedef CmyArray<PHEADERINFO, PHEADERINFO>	ARRAYHEADERINFO;


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoBase
{
public:
			CSaveLoadInfoBase();						/* コンストラクタ */
	virtual ~CSaveLoadInfoBase();						/* デストラクタ */

	virtual void	WriteData			(void);					/* データの書き込み */
	virtual BOOL	ReadData			(void);					/* データの読み込み */

	virtual void	Save				(PCLibInfoBase pSrc);	/* ファイルに書き込み */
	virtual void	Load				(PCLibInfoBase pDst);	/* ファイルから読み込み */


protected:
	virtual void	SetHeaderInfo		(PCInfoBase pInfo);		/* ヘッダ情報を設定 */
	void			MakeWriteData		(void);					/* 保存データの作成 */
	void			ReadHeader			(void);					/* ヘッダ情報の読み込み */
	void			SetFileName			(LPCSTR pszName);		/* ファイル名を設定 */
	void			AddHeaderInfo		(LPCSTR pszName);		/* ヘッダ情報を追加 */
	PHEADERINFO		GetHeaderInfo		(LPCSTR pszName);		/* 要素名からヘッダ情報を取得 */
	void			DelAllHeaderInfo	(void);					/* ヘッダ情報を削除 */

	void			CopyMemoryDataDst	(PBYTE &pDst, PBYTE pSrc, DWORD dwSize);			/* コピーしてコピー先ポインタを進める */
	void			CopyMemoryDataSrc	(PBYTE pDst, PBYTE &pSrc, DWORD dwSize);			/* コピーしてコピー元ポインタを進める */


protected:
	CmyString			m_strFileName;				/* ファイル名 */
	PBYTE				m_pData;					/* 保存・読み込みデータ */
	DWORD				m_dwDataSize;				/* データサイズ */
	DWORD				m_dwHeaderSize;				/* ヘッダサイズ */
	ARRAYHEADERINFO		*m_pHeaderInfo;				/* ヘッダ情報 */

	PCLibInfoBase		m_pLibInfoBase;				/* 情報クラス */
} CSaveLoadInfoBase, *PCSaveLoadInfoBase;

/* Copyright(C)URARA-works 2005 */
