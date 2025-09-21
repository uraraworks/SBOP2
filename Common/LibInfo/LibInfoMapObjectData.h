/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LibInfoMapObjectData.h										 */
/* 内容			:マップオブジェクト配置データライブラリクラス 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/03													 */
/* ========================================================================= */

#pragma once

#include "InfoMapObjectData.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoMapObjectData : public CLibInfoBase
{
public:
			CLibInfoMapObjectData();					/* コンストラクタ */
	virtual ~CLibInfoMapObjectData();					/* デストラクタ */

	void Create			(void);									/* 作成 */
	void Destroy		(void);									/* 破棄 */

	PCInfoBase GetNew	(void);									/* 新規データを取得 */

	int		GetCount		(void);								/* データ数を取得 */
	void	Add				(PCInfoBase pInfo);					/* 追加 */
	void	Delete			(int nNo);							/* 削除 */
	void	Delete			(DWORD dwDataID);					/* 削除 */
	void	DeleteAll		(void);								/* 全て削除 */
	void	Merge			(CLibInfoMapObjectData *pSrc);		/* 取り込み */

	CInfoMapObjectData	*Renew	(DWORD dwDataID, CInfoMapObjectData *pSrc);	/* 更新 */
	PCInfoBase			GetPtr	(int nNo);									/* 配置データを取得 */
	PCInfoBase			GetPtr	(DWORD dwDataID);							/* 配置データを取得 */

	virtual DWORD	GetDataSize		(void);						/* データサイズを取得 */
	virtual PBYTE	GetWriteData	(PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	virtual DWORD	ReadElementData	(PBYTE pSrc);				/* 指定要素データを読み込み */

	virtual DWORD	GetSendDataSize	(void);						/* 送信データサイズを取得 */
	virtual PBYTE	GetSendData		(void);						/* 送信データを取得 */
	virtual PBYTE	SetSendData		(PBYTE pSrc);				/* 送信データから取り込み */


protected:
	DWORD	GetNewID	(void);									/* 新しいIDを取得 */


protected:
	DWORD	m_dwNewIDTmp;						/* 新規ID作成用 */
	PARRAYMAPOBJECTDATAINFO	m_paInfo;			/* 配置データ */
} CLibInfoMapObjectData, *PCLibInfoMapObjectData;

/* Copyright(C)URARA-works 2008 */
