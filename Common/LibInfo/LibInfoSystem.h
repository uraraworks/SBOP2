/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LibInfoSystem.h											 */
/* 内容			:システム情報ライブラリクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/28													 */
/* ========================================================================= */

#pragma once

#include "InfoSystem.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoSystem : public CLibInfoBase
{
public:
			CLibInfoSystem();								/* コンストラクタ */
	virtual ~CLibInfoSystem();								/* デストラクタ */

	void Create			(void);										/* 作成 */
	void Destroy		(void);										/* 破棄 */

	virtual PCInfoBase GetNew	(void);								/* 新規データを取得 */

	int		GetCount	(void);										/* データ数を取得 */
	void	Add			(PCInfoBase pInfo);							/* 追加 */
	void	Delete		(int nNo);									/* 削除 */
	void	DeleteAll	(void);										/* 全て削除 */

	PCInfoBase	GetPtr	(int nNo = 0);								/* システム情報を取得 */


protected:
	PCInfoSystem	m_pInfo;				/* システム情報 */
} CLibInfoSystem, *PCLibInfoSystem;

/* Copyright(C)URARA-works 2008 */
