/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WndMapShadowAnimeList.h									 */
/* 内容			:マップ影アニメーション一覧スタティックコントロールクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/06													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;
class CInfoMapShadow;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWndMapShadowAnimeList : public CWnd
{
public:
			CWndMapShadowAnimeList();						/* コンストラクタ */
	virtual ~CWndMapShadowAnimeList();						/* デストラクタ */

	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData, int nResourceID, CInfoMapShadow *pInfoMapShadow);	/* 作成 */
	void	Destroy	(void);									/* 破棄 */
	void	Renew	(void);									/* 更新 */


protected:
	void	MakeImage		(void);				/* 画像を作成 */


protected:
	CWnd		*m_pWndParent;					/* 親ウィンドウ */
	CImg32		*m_pImgShadow;					/* バックバッファ */

	CMgrData		*m_pMgrData;				/* データ管理 */
	CMgrGrpData		*m_pMgrGrpData;				/* グラフィックデータ管理 */
	CInfoMapShadow	*m_pInfoMapShadow;			/* マップ影情報 */



public:
	//{{AFX_VIRTUAL(CWndMapShadowAnimeList)
	public:
	protected:
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndMapShadowAnimeList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CWndMapShadowAnimeList, *PCWndMapShadowAnimeList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
