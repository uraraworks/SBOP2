/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoMapObjectData.h										 */
/* 内容			:マップオブジェクト配置データクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/03													 */
/* ========================================================================= */

#pragma once
#include <vector>

#include "InfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoMapObjectData : public CInfoBase
{
public:
			CInfoMapObjectData();									/* コンストラクタ */
	virtual ~CInfoMapObjectData();									/* デストラクタ */

	virtual void	RenewSize			(int nDirection, int nSize, SIZE *pSize);	/* サイズ更新 */
	virtual int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	virtual DWORD	GetDataSize			(void);								/* データサイズを取得 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	virtual LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	virtual DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	virtual PBYTE	GetSendData			(void);								/* 送信データを取得 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	virtual void	Copy				(CInfoMapObjectData *pSrc);			/* コピー */


public:
	DWORD		m_dwDataID,						/* 配置データID */
				m_dwObjectID;					/* オブジェクトID */
	POINT		m_ptPos;						/* 座標 */
} CInfoMapObjectData, *PCInfoMapObjectData;
typedef std::vector<PCInfoMapObjectData>	  ARRAYMAPOBJECTDATAINFO;
typedef std::vector<PCInfoMapObjectData>	*PARRAYMAPOBJECTDATAINFO;

/* Copyright(C)URARA-works 2008 */
