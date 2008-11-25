/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WndMap.h													 */
/* 内容			:マップウィンドウクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/12													 */
/* ========================================================================= */

#pragma once

#include "WndToolbar.h"

class CMgrData;
class CMgrGrpData;
class CImg32;
class CMgrDraw;
class CInfoMapBase;
class CUraraSockTCPSBO;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWndMap : public CWnd
{
public:
			CWndMap();									/* コンストラクタ */
	virtual ~CWndMap();									/* デストラクタ */

	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData, int nNotify);	/* 作成 */
	void	Destroy	(void);												/* 破棄 */


protected:
	void	RenewScrollRange	(int cx, int cy);			/* スクロールバーの範囲を更新 */
	void	RenewToolbar		(void);						/* ツールバーを更新 */
	void	RenewStatusText		(void);						/* ステータスバーのテキストを更新 */
	BOOL	GetCheck			(int nNo);					/* ツールバーのボタン状態を取得 */
	void	SetCheck			(int nNo, BOOL bCheck);		/* ツールバーのボタン状態を設定 */


protected:
	BOOL		m_bRClickFirst,					/* 右クリックされた */
				m_bViewGrid;					/* グリッド表示 */
	int			m_nSelect,						/* 選択されたパーツ番号 */
				m_nNotify;						/* 通知データ */
	DWORD		*m_pdwParts;					/* コピー中のマップ */
	CWnd		*m_pWndParent;					/* 親ウィンドウ */
	CImg32		*m_pImgBack,					/* バックバッファ */
				*m_pImgBase;					/* 表示画像 */
	CWndToolbar	m_wndToolBar;					/* ツールバー */
	CStatusBar	m_StatusBar;					/* ステータスバー */
	CImageList	m_ImgListToolBar;				/* ツールバー用イメージリスト */
	CRect		m_rcRange,						/* 選択範囲 */
				m_rcCopy;						/* コピーする範囲 */
	POINT		m_ptBack;						/* 前回のマウス位置 */

	CMgrData			*m_pMgrData;			/* データ管理 */
	CMgrGrpData			*m_pMgrGrpData;			/* グラフィックデータ管理 */
	CMgrDraw			*m_pMgrDraw;			/* 描画マネージャ */
	CInfoMapBase		*m_pInfoMap;			/* マップ情報 */
	CUraraSockTCPSBO	*m_pSock;				/* 通信マネージャ */



public:
	//{{AFX_VIRTUAL(CWndMap)
	public:
	protected:
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndMap)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateCommandUI(CCmdUI* pCmdUI);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnCopy();
	afx_msg void OnPaste();
	afx_msg void OnToolPaint();
	afx_msg void OnRange();
	afx_msg void OnGrid();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CWndMap, *PCWndMap;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
