/// @file LibInfoBase.h
/// @brief 情報ライブラリ 基底クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "InfoBase.h"
#include "mySection.h"

typedef class CLibInfoBase : public CmySection
{
public:
			CLibInfoBase();	// コンストラクタ
	virtual ~CLibInfoBase();	// デストラクタ

	virtual void Create(void);	// 作成
	virtual void Destroy(void);	// 破棄
	virtual BOOL Proc(void);	// 処理

	virtual int	GetCount(void);	// データ数を取得
	virtual PCInfoBase	GetNew(void);	// 新規データを取得
	virtual PCInfoBase	RenewInfo(int nNo);	// 内容に応じて更新
	virtual void	GetSaveNo(ARRAYINT &anDst);	// 保存する情報のインデックスを取得
	virtual void	Add(PCInfoBase pInfo);	// 追加
	virtual void	RenewIDPtr(void);	// ID検索用マップを更新
	virtual PCInfoBase	GetPtr(int nNo);	// 情報を取得

public:
	int	m_nProcNo;	// 処理中のインデックス
	MapIDPtr	m_mapIDPtr;	// ID検索用マップ
} CLibInfoBase, *PCLibInfoBase;

