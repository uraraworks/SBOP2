/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名：	WindowMSG.h													 */
/* 内容：		メッセージ表示ウィンドウクラス 定義ファイル					 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2006/11/03													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowMSG : public CWindowBase
{
public:
			CWindowMSG();									/* コンストラクタ */
	virtual ~CWindowMSG();									/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */

	void	Draw		(CImg32 *pDst);								/* 描画 */
	BOOL	TimerProc	(void);										/* 時間処理 */
	void	SetMsg		(LPCSTR pszMsg, DWORD dwTime, int nType=0);				/* メッセージ設定 */


private:
	int			m_nType;					/* ウィンドウ種別 */
	DWORD		m_dwDeleteTime;				/* 削除までの時間 */
	CmyString	m_strMsg;					/* メッセージ */
} CWindowMSG, *PCWindowMSG;

/* Copyright(C)URARA-works 2006 */
