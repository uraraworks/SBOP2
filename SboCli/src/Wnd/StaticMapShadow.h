/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:StaticMapShadow.h											 */
/* 内容			:マップ影一覧スタティックコントロールクラス 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/06													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;
class CLibInfoMapShadow;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CStaticMapShadow : public CStatic
{
public:
			CStaticMapShadow();						/* コンストラクタ */
	virtual ~CStaticMapShadow();					/* デストラクタ */


	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData);	/* 作成 */
	void	Destroy	(void);									/* 破棄 */

	void	Renew			(void);							/* 影一覧画像を作成して再描画 */
	void	SetScrollPos	(int nPos);						/* 縦スクロール位置を設定 */

	void	OnLButtonDown	(CPoint point);					/* メッセージハンドラ(WM_LBUTTONDOWN) */
	void	OnRButtonDown	(CPoint point);					/* メッセージハンドラ(WM_RBUTTONDOWN) */
	void	OnMouseWheel	(short zDelta, CPoint pt);		/* メッセージハンドラ(WM_MOUSEWHEEL) */
	void	OnMouseMove		(CPoint point);					/* メッセージハンドラ(WM_MOUSEMOVE) */


protected:
	void	MakeShadowImage		(void);				/* 影一覧画像を作成 */


protected:
	int			m_nPos;							/* 縦スクロール位置 */
	CWnd		*m_pWndParent;					/* 親ウィンドウ */
	CImg32		*m_pImgShadow;					/* バックバッファ */
	CPoint		m_ptCursor,						/* カーソルのある位置 */
				m_ptMoveSrc,					/* 移動元の位置 */
				m_ptMoveDst;					/* 移動先の位置 */

	CMgrData			*m_pMgrData;			/* データ管理 */
	CMgrGrpData			*m_pMgrGrpData;			/* グラフィックデータ管理 */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;	/* マップ影情報 */



public:
	//{{AFX_VIRTUAL(CStaticMapShadow)
	public:
	protected:
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStaticMapShadow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CStaticMapShadow, *PCStaticMapShadow;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
