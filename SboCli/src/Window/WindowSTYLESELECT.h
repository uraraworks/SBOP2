/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowSTYLESELECT.h										 */
/* 内容			:容姿選択ウィンドウクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/10													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;
class CMgrWindow;
class CMgrDraw;
class CWindowSEX;
class CWindowHAIRTYPE;
class CWindowHAIRCOLOR;
class CWindowEYECOLOR;
class CInfoCharCli;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowSTYLESELECT : public CWindowBase
{
public:
			CWindowSTYLESELECT();							/* コンストラクタ */
	virtual ~CWindowSTYLESELECT();							/* デストラクタ */

	void	Create			(CMgrData *pMgrData);					/* 作成 */
	void	SetFamilyType	(WORD wFamilyID);						/* 種族を設定 */
	void	SetInfoChar		(CInfoCharCli *pInfoCharCli);			/* キャラ情報を設定 */
	void	OnWindowMsg		(int nType, DWORD dwPara);				/* メッセージハンドラ(WM_WINDOWMSG) */
	void	Draw			(CImg32 *pDst);							/* 描画 */
	BOOL	TimerProc		(void);									/* 時間処理 */


private:
	BOOL	OnUp		(void);								/* キーハンドラ(↑) */
	BOOL	OnDown		(void);								/* キーハンドラ(↓) */
	BOOL	OnLeft		(void);								/* キーハンドラ(←) */
	BOOL	OnRight		(void);								/* キーハンドラ(→) */
	BOOL	OnX			(BOOL bDown);						/* キーハンドラ(X) */
	BOOL	OnZ			(BOOL bDown);						/* キーハンドラ(Y) */


public:
	CInfoCharCli		*m_pInfoCharCli;			/* 作成中のキャラ情報 */


private:
	BOOL		m_bAnime;							/* アニメーションする判定 */
	int			m_nSex,								/* 性別 */
				m_nSexBack,							/* 性別保存用 */
				m_nDirection;						/* 向き番号 */
	WORD		m_wFamilyID,						/* 種族ID */
				m_wFamilyIDBack,					/* 種族ID保存用 */
				m_wHairTypeID,						/* 髪型ID */
				m_wHairTypeIDBack,					/* 髪型ID保存用 */
				m_wHairColorID,						/* 髪色ID */
				m_wHairColorIDBack,					/* 髪色ID保存用 */
				m_wEyeColorID,						/* 目色ID */
				m_wEyeColorIDBack;					/* 目色ID保存用 */
	CMgrWindow			*m_pMgrWindow;				/* ウィンドウマネージャ */
	CMgrDraw			*m_pMgrDraw;				/* 描画マネージャ */
	CWindowSEX			*m_pWindowSEX;				/* 性別入力ウィンドウ */
	CWindowHAIRTYPE		*m_pWindowHAIRTYPE;			/* 髪型選択ウィンドウ */
	CWindowHAIRCOLOR	*m_pWindowHAIRCOLOR;		/* 髪色選択ウィンドウ */
	CWindowEYECOLOR		*m_pWindowEYECOLOR;			/* 目色選択ウィンドウ */
} CWindowSTYLESELECT, *PCWindowSTYLESELECT;

/* Copyright(C)URARA-works 2007 */
