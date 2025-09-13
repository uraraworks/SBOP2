/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:LibInfoCharBase.h											 */
/* 内容			:キャラ情報ライブラリ基底クラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/01													 */
/* ========================================================================= */

#pragma once

#include "InfoCharBase.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoCharBase : public CLibInfoBase
{
public:
			CLibInfoCharBase();							/* コンストラクタ */
	virtual ~CLibInfoCharBase();						/* デストラクタ */

	virtual void	Create		(void);							/* 作成 */
	virtual void	Destroy		(void);							/* 破棄 */
	virtual BOOL	Proc		(void);							/* 処理 */
	virtual void	RenewSize	(DWORD dwMapID, int nDirection, int nSize);	/* マップサイズ更新 */

			PCInfoBase	GetNew	(void);									/* 新規データを取得 */
	virtual PCInfoBase	GetNew	(int nType);							/* 新規データを取得 */

	void	GetSaveNo			(ARRAYINT &anDst);						/* 保存する情報のインデックスを取得 */
	int		GetCount			(void);									/* データ数を取得 */
	int		GetCountScreen		(PCInfoCharBase pCharBase);				/* データ数を取得(画面内のキャラ) */
	void	Add					(PCInfoBase pInfo);						/* 追加 */
	DWORD	Add					(PCInfoCharBase pChar);					/* 追加 */
	void	CopyAdd				(PCInfoCharBase pChar);					/* コピーして追加 */
	void	Delete				(int nNo);								/* 削除 */
	void	Delete				(DWORD dwCharID);						/* 削除 */
	void	DeleteAll			(void);									/* 全て削除 */
	void	SortY				(void);									/* Y座標順にソート */
	BOOL	IsBlockChar			(PCInfoCharBase pChar, int nDirection, BOOL bNoBlockFlg=TRUE, BOOL bHitCheck=FALSE);	/* 一歩前でぶつかるかチェック */
	BOOL	IsUseName			(LPCSTR pszName);						/* 名前が使用されているかチェック */
	BOOL	NameCheck			(LPCSTR pszName);						/* 名前に使用できない名前があるかチェック */
	int		GetTurnDirection	(int nDirection);						/* 逆向きを取得 */
	void	GetDistance			(SIZE &sizeDst, PCInfoCharBase pInfoCharSrc, PCInfoCharBase pInfoCharDst, BOOL bFrontPos = FALSE);/* キャラ座標で距離を取得 */

	BOOL	IsScreenInside	(PCInfoCharBase pCharBase, PCInfoCharBase pCharTarget);	/* 画面内にいるかチェック */

	virtual	DWORD	GetFrontCharID		(DWORD dwCharID, int nDirection = -1);	/* 一歩前のキャラIDを取得 */
	virtual	DWORD	GetFrontCharIDPush	(DWORD dwCharID, int nDirection = -1);	/* 一歩前の押せるキャラIDを取得 */
	virtual	DWORD	GetHitCharID		(DWORD dwCharIDBase, int x, int y);		/* 指定座標に当たるキャラIDを取得 */
	virtual	void	SetPtr				(DWORD dwCharID, PCInfoCharBase pChar);	/* キャラ情報を更新 */
			PCInfoBase	GetPtr			(int nNo);								/* キャラ情報を取得 */
			PCInfoBase	GetPtr			(DWORD dwCharID);						/* キャラ情報を取得 */
			PCInfoBase	GetPtrAccountID	(DWORD dwAccountID);					/* キャラ情報を取得 */
			PCInfoBase	GetPtrFront		(PCInfoCharBase pChar, int nDirection);	/* 一歩前のキャラ情報を取得 */

	DWORD	GetSendDataSize		(void);							/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);							/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);					/* 送信データから取り込み */

	DWORD	GetDataSizeScreen	(PCInfoCharBase pCharBase);		/* データサイズを取得(画面内のキャラ) */
	PBYTE	GetDataScreen		(PCInfoCharBase pCharBase);		/* データを取得(画面内のキャラ) */


protected:
	DWORD	GetNewID		(void);								/* 新しいキャラIDを取得 */


protected:
	DWORD	m_dwNewIDTmp;						/* 新規ID作成用 */
	PARRAYINFOCHARBASE	m_paInfo;				/* キャラ情報 */
} CLibInfoCharBase, *PCLibInfoCharBase;

/* Copyright(C)URARA-works 2006 */
