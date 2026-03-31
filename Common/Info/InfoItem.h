/// @file InfoItem.h
/// @brief アイテム情報クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/05
/// @copyright Copyright(C)URARA-works 2007

#pragma once
#include <vector>
#include "myArray.h"

#include "InfoBase.h"

typedef class CInfoItem : public CInfoBase
{
public:
			CInfoItem();	// コンストラクタ
	virtual ~CInfoItem();	// デストラクタ

	int	GetElementNo(LPCSTR pszName);	// 要素番号を取得
	DWORD	GetDataSize(void);	// データサイズを取得
	DWORD	GetDataSizeNo(int nNo);	// 指定要素のデータサイズを取得
	LPCSTR	GetName(int nNo);	// 要素名を取得
	PBYTE	GetWriteData(int nNo, PDWORD pdwSize);	// 指定要素の保存用データを取得
	DWORD	ReadElementData(PBYTE pSrc, int nNo);	// 指定要素データを読み込み

	DWORD	GetSendDataSize(void);	// 送信データサイズを取得
	PBYTE	GetSendData(void);	// 送信データを取得
	PBYTE	SetSendData(PBYTE pSrc);	// 送信データから取り込み

	void	Copy(CInfoItem *pSrc);	// コピー

public:
	// 保存しないデータ

	// 保存するデータ
	BOOL	m_bPutOn;	// 上に置けるか判定
	DWORD	m_dwItemID,	// アイテムID
				m_dwItemTypeID,	// アイテム種別ID
				m_dwGrpID,	// 地面にある時の画像ID
				m_dwIconGrpID,	// バックパック内の画像ID
				m_dwMapID,	// 落ちているマップID
				m_dwCharID,	// 所有者キャラID
				m_dwDropSoundID;	// 落ちたときの効果音ID
	int	m_nPosZ;	// 落ちている高さ(0が地面)
	POINT	m_ptPos,	// 落ちている座標
				m_ptBackPack;	// バックパック内の位置
	CmyString	m_strName;	// アイテム名
} CInfoItem, *PCInfoItem;
using ARRAYITEMINFO = CStdArray<PCInfoItem>;
using PARRAYITEMINFO = ARRAYITEMINFO *;

