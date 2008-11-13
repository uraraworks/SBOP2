/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:StaticMapObjectGrp.h										 */
/* 内容			:マップオブジェクト表示スタティックコントロールクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/02													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;
class CInfoMapObject;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CStaticMapObjectGrp : public CStatic
{
public:
			CStaticMapObjectGrp();					/* コンストラクタ */
	virtual ~CStaticMapObjectGrp();					/* デストラクタ */


	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData);	/* 作成 */
	void	Destroy	(void);									/* 破棄 */
	void	Init	(CInfoMapObject *pInfoMapObject);		/* 初期化 */
	void	Set		(int nAnimeNo);							/* 更新 */
	void	IncPosX	(void);									/* 選択位置を右に移動 */


protected:
	void	RenewGrp(void);									/* 画像更新 */


public:
	int				m_nAnimeNo;					/* 表示するアニメ番号 */
	POINT			m_ptSelect;					/* 選択位置 */
	CWnd			*m_pWndParent;				/* 親ウィンドウ */
	CImg32			*m_pImgBack;				/* バックバッファ */
	CMgrData		*m_pMgrData;				/* データ管理 */
	CMgrGrpData		*m_pMgrGrpData;				/* グラフィックデータ管理 */
	CInfoMapObject	*m_pInfoMapObject;			/* 表示するマップオブジェクト情報 */



public:
	//{{AFX_VIRTUAL(CStaticMapObjectGrp)
	public:
	protected:
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStaticMapObjectGrp)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CStaticMapObjectGrp, *PCStaticMapObjectGrp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
