/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WndMapShadowGrp.h											 */
/* 内容			:マップ影画像一覧スタティックコントロールクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/06													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWndMapShadowGrp : public CWnd
{
public:
			CWndMapShadowGrp();						/* コンストラクタ */
	virtual ~CWndMapShadowGrp();					/* デストラクタ */

	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData, int nResourceID);	/* 作成 */
	void	Destroy	(void);									/* 破棄 */


protected:
	void	MakeImage	(void);			/* 画像を作成 */


protected:
	int			m_nPos;							/* 縦スクロール位置 */
	CWnd		*m_pWndParent;					/* 親ウィンドウ */
	CImg32		*m_pImgShadow;					/* バックバッファ */
	CPoint		m_ptCursor;						/* カーソルのあるパーツ位置 */

	CMgrData		*m_pMgrData;				/* データ管理 */
	CMgrGrpData		*m_pMgrGrpData;				/* グラフィックデータ管理 */



public:
	//{{AFX_VIRTUAL(CWndMapShadowGrp)
	public:
	protected:
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndMapShadowGrp)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CWndMapShadowGrp, *PCWndMapShadowGrp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
