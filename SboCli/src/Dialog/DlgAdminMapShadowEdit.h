/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapShadowEdit.h									 */
/* 内容			:マップ影編集ダイアログクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/06													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CMgrData;
class CWndMapShadowAnimeList;
class CWndMapPartsGrp;
class CInfoMapShadow;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CDlgAdminMapShadowEdit : public CDlgAdminBase
{
public:
			CDlgAdminMapShadowEdit(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual	~CDlgAdminMapShadowEdit();						/* デストラクタ */

	int DoModal (CMgrData *pMgrData);								/* モーダルダイアログ表示 */


protected:
	void	SetData				(void);					/* データを画面に反映 */
	void	GetData				(void);					/* 画面からデータを取得 */
	void	OnSelectAnime		(void);					/* コマ選択処理 */
	void	RenewButtonState	(void);					/* ボタン状態の設定 */


public:
	int						m_nSelectType,				/* どちらの画像を選択しているか */
							m_nAnimeNo;					/* アニメーション中の番号 */
	WORD					m_wShadowGrpID;				/* 影画像ID */
	DWORD					m_dwTimeLastAnime;			/* 最後にアニメーションした時間 */
	SIZE					m_sizeWindow;				/* 表示開始時のウィンドウサイズ */
	CMgrData				*m_pMgrData;				/* データ管理 */
	CMgrGrpData				*m_pMgrGrpData;				/* グラフィックデータ管理 */
	CWndMapShadowAnimeList	*m_pWndMapShadowAnimeList;	/* マップ影アニメーション一覧ウィンドウ */
	CInfoMapShadow			*m_pInfoMapShadow;			/* マップ影情報 */
	CWndMapPartsGrp			*m_pWndMapShadowGrp;		/* マップ影画像一覧 */



public:
	//{{AFX_DATA(CDlgAdminMapShadowEdit)
	enum { IDD = IDD_SHADOW_EDIT };
	CSliderCtrl	m_Slider;
	CString	m_strCount;
	CString	m_strNow;
	CComboBox	m_cbGrpNo;
	int		m_nGrpNo;
	int		m_nLevel;
	int		m_nViewTime;
	BOOL	m_bLight;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapShadowEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapShadowEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnChangeLevel();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	afx_msg void OnChangeViewtime();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnSelchangeGrpNo();
	//}}AFX_MSG
	afx_msg LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
