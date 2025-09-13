/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:StaticMapParts.h											 */
/* 内容			:マップパーツ一覧スタティックコントロールクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/10													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;
class CLibInfoMapParts;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CStaticMapParts : public CStatic
{
public:
			CStaticMapParts();						/* コンストラクタ */
	virtual ~CStaticMapParts();						/* デストラクタ */


	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData);	/* 作成 */
	void	Destroy	(void);									/* 破棄 */

	void	Renew			(void);							/* パーツ一覧画像を作成して再描画 */
	void	SetScrollPos	(int nPos);						/* 縦スクロール位置を設定 */

	void	OnLButtonDown	(CPoint point);					/* メッセージハンドラ(WM_LBUTTONDOWN) */
	void	OnRButtonDown	(CPoint point);					/* メッセージハンドラ(WM_RBUTTONDOWN) */
	void	OnMouseWheel	(short zDelta, CPoint pt);		/* メッセージハンドラ(WM_MOUSEWHEEL) */
	void	OnMouseMove		(CPoint point);					/* メッセージハンドラ(WM_MOUSEMOVE) */


protected:
	void	MakePartsImage		(void);				/* パーツ一覧画像を作成 */


protected:
	int			m_nPos;							/* 縦スクロール位置 */
	CWnd		*m_pWndParent;					/* 親ウィンドウ */
	CImg32		*m_pImgParts;					/* バックバッファ */
	CPoint		m_ptCursor,						/* カーソルのあるパーツ位置 */
				m_ptMoveSrc,					/* 移動元の位置 */
				m_ptMoveDst;					/* 移動先の位置 */

	CMgrData			*m_pMgrData;			/* データ管理 */
	CMgrGrpData			*m_pMgrGrpData;			/* グラフィックデータ管理 */
	CLibInfoMapParts	*m_pLibInfoMapParts;	/* マップパーツ情報 */



public:
	//{{AFX_VIRTUAL(CStaticMapParts)
	public:
	protected:
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStaticMapParts)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CStaticMapParts, *PCStaticMapParts;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
