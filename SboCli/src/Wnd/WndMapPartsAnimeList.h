/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WndMapPartsAnimeList.h										 */
/* 内容			:マップパーツアニメーション一覧スタティックコントロールクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/22													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;
class CInfoMapParts;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWndMapPartsAnimeList : public CWnd
{
public:
			CWndMapPartsAnimeList();						/* コンストラクタ */
	virtual ~CWndMapPartsAnimeList();						/* デストラクタ */

	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData, int nResourceID, CInfoMapParts *pInfoMapParts);	/* 作成 */
	void	Destroy	(void);									/* 破棄 */
	void	Renew	(void);									/* 更新 */


protected:
	void	MakeImage		(void);				/* 画像を作成 */


protected:
	CWnd		*m_pWndParent;					/* 親ウィンドウ */
	CImg32		*m_pImgParts;					/* バックバッファ */

	CMgrData		*m_pMgrData;				/* データ管理 */
	CMgrGrpData		*m_pMgrGrpData;				/* グラフィックデータ管理 */
	CInfoMapParts	*m_pInfoMapParts;			/* マップパーツ情報 */



public:
	//{{AFX_VIRTUAL(CWndMapPartsAnimeList)
	public:
	protected:
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndMapPartsAnimeList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CWndMapPartsAnimeList, *PCWndMapPartsAnimeList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
