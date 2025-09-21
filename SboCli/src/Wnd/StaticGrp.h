/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:StaticGrp.h												 */
/* 内容			:画像表示スタティックコントロールクラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/11/10													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CStaticGrp : public CStatic
{
public:
			CStaticGrp();							/* コンストラクタ */
	virtual ~CStaticGrp();							/* デストラクタ */


	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData);	/* 作成 */
	void	Destroy	(void);									/* 破棄 */

	void	Init		(CImg32 *pSrc);						/* 初期化 */
	void	Init		(DWORD dwGrpIDMain);				/* 初期化 */
	void	Set			(DWORD dwGrpIDSub);					/* 画像設定 */
	void	SetParam	(DWORD dwGrpIDParam);				/* 画像パラメータ設定 */
	DWORD	GetIDMain	(void)	{ return m_dwGrpIDMain;	}	/* 画像IDメインを取得 */
	DWORD	GetIDSub	(void)	{ return m_dwGrpIDSub;	}	/* 画像IDサブを取得 */


protected:
	void	RenewGrp	(DWORD dwGrpIDMain, DWORD dwGrpIDSub);	/* 画像更新 */


protected:
	DWORD			m_dwGrpIDMain,				/* 画像IDメイン */
					m_dwGrpIDSub,				/* 画像IDサブ */
					m_dwGrpIDParam;				/* 画像パラメータ */
	CWnd			*m_pWndParent;				/* 親ウィンドウ */
	CImg32			*m_pImgBack;				/* バックバッファ */
	CMgrData		*m_pMgrData;				/* データ管理 */
	CMgrGrpData		*m_pMgrGrpData;				/* グラフィックデータ管理 */



public:
	//{{AFX_VIRTUAL(CStaticGrp)
	public:
	protected:
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStaticGrp)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CStaticGrp, *PCStaticGrp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
