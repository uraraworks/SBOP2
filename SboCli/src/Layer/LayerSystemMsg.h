/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LayerSystemMsg.h											 */
/* 内容			:レイヤー描画クラス(システムメッセージ) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/30													 */
/* ========================================================================= */

#pragma once

#include "LayerCloud.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* 構造体宣言																 */
/* ========================================================================= */

typedef struct _SYSTEMMSGINFO {
	int			nPosY;						/* Y座標 */
	CImg32		*pImg;						/* イメージ */
} SYSTEMMSGINFO, *PSYSTEMMSGINFO;


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLayerSystemMsg : public CLayerCloud
{
public:
			CLayerSystemMsg();						/* コンストラクタ */
	virtual ~CLayerSystemMsg();						/* デストラクタ */

	void	Draw		(CImg32 *pDst);						/* 描画 */
	BOOL	TimerProc	(void);								/* 時間処理 */
	void	AddMsg		(LPCSTR pszMsg, COLORREF cl);		/* メッセージを追加 */


private:
	void	DeleteMsg		(int nNo);						/* メッセージを削除 */
	void	DeleteAllMsg	(void);							/* 全てのメッセージを削除 */


private:
	DWORD		m_dwLastTimeProc;				/* 最終処理時間 */
	CImg32		*m_pDibBack;					/* 背景画像 */

	CmyArray<PSYSTEMMSGINFO, PSYSTEMMSGINFO>	m_aSystemMsgInfo;	/* システムメッセージ情報 */
} CLayerSystemMsg, *PCLayerSystemMsg;

/* Copyright(C)URARA-works 2007 */
