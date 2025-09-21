/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名：	WndSelectMapPartsGrp.h										 */
/* 内容：		マップパーツ画像選択ウィンドウクラス 定義ファイル			 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2005/10/07													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWndSelectMapPartsGrp : public CWnd
{
public:
			CWndSelectMapPartsGrp();							/* コンストラクタ */
	virtual ~CWndSelectMapPartsGrp();							/* デストラクタ */


	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData);	/* 作成 */
	void	Destroy	(void);									/* 破棄 */


protected:
	int			m_nSelect;						/* 選択されたパーツ番号 */
	CWnd		*m_pWndParent;					/* 親ウィンドウ */
	CImg32		*m_pImgBack;					/* バックバッファ */

	CMgrData		*m_pMgrData;				/* データ管理 */
	CMgrGrpData		*m_pMgrGrpData;				/* グラフィックデータ管理 */



public:
	//{{AFX_VIRTUAL(CWndSelectMapPartsGrp)
	public:
	protected:
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndSelectMapPartsGrp)
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CWndSelectMapPartsGrp, *PCWndSelectMapPartsGrp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2005 */
