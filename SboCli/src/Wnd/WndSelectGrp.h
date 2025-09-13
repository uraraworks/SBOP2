/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WndSelectGrp.h												 */
/* 内容			:画像選択ウィンドウクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/30													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWndSelectGrp : public CWnd
{
public:
			CWndSelectGrp();						/* コンストラクタ */
	virtual ~CWndSelectGrp();						/* デストラクタ */

	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData, int nNotify);	/* 作成 */
	void	Destroy	(void);									/* 破棄 */

	void	SetType	(int nType, int nGrpIDSub = 0);			/* 画像タイプの設定 */


protected:
	void	RenewScrollRange	(int cx, int cy);			/* スクロールバーの範囲を更新 */
	void	RenewGrp			(int nType);				/* 表示画像の更新 */


protected:
	int			m_nSelect,						/* 選択されたパーツ番号 */
				m_nNotify,						/* 通知データ */
				m_nType,						/* 画像タイプ */
				m_nGrpSizeX,					/* 1画像の横サイズ */
				m_nGrpCountX,					/* 画像１ファイルの横個数 */
				m_nGrpIDSub;					/* グラフィックサブID */
	CWnd		*m_pWndParent;					/* 親ウィンドウ */
	CImg32		*m_pImgBack,					/* バックバッファ */
				*m_pImgBase;					/* 表示画像 */

	CMgrData		*m_pMgrData;				/* データ管理 */
	CMgrGrpData		*m_pMgrGrpData;				/* グラフィックデータ管理 */



public:
	//{{AFX_VIRTUAL(CWndSelectGrp)
	public:
	protected:
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndSelectGrp)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CWndSelectGrp, *PCWndSelectGrp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
