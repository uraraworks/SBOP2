/// @file LibInfoMapEvent.h
/// @brief マップイベント情報ライブラリクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/22
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "InfoMapEventBase.h"
#include "LibInfoBase.h"

typedef class CLibInfoMapEvent : public CLibInfoBase
{
public:
			CLibInfoMapEvent();	// コンストラクタ
	virtual ~CLibInfoMapEvent();	// デストラクタ

	void Create(void);	// 作成
	void Destroy(void);	// 破棄

	PCInfoBase GetNew(int nType);	// 新規データを取得

	int	GetCount(void);	// データ数を取得
	void	Add(PCInfoBase pInfo);	// 追加
	void	Delete(int nNo);	// 削除
	void	Delete(DWORD dwMapEventID);	// 削除
	void	DeleteAll(void);	// 全て削除
	void	Merge(CLibInfoMapEvent *pSrc);	// 取り込み

	CInfoMapEventBase	*Renew(DWORD dwMapEventID, CInfoMapEventBase *pSrc);	// 更新
	PCInfoBase	GetPtr(int nNo);	// マップイベント情報を取得
	PCInfoBase	GetPtr(DWORD dwMapEventID);	// マップイベント情報を取得

	virtual DWORD	GetDataSize(void);	// データサイズを取得
	virtual PBYTE	GetWriteData(PDWORD pdwSize);	// 指定要素の保存用データを取得
	virtual DWORD	ReadElementData(PBYTE pSrc);	// 指定要素データを読み込み

	virtual DWORD	GetSendDataSize(void);	// 送信データサイズを取得
	virtual PBYTE	GetSendData(void);	// 送信データを取得
	virtual PBYTE	SetSendData(PBYTE pSrc);	// 送信データから取り込み

protected:
	DWORD	GetNewID(void);	// 新しいIDを取得

protected:
	DWORD	m_dwNewIDTmp;	// 新規ID作成用
	PARRAYMAPEVENTBASEINFO	m_paInfo;	// マップイベント情報
} CLibInfoMapEvent, *PCLibInfoMapEvent;

