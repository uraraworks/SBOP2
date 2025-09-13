/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminEfcBalloon.h										 */
/* 内容			:噴出し情報設定ダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/25													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "StaticGrp.h"

class CMgrGrpData;
class CWndSelectGrp;
class CLibInfoEfcBalloon;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminEfcBalloon : public CDlgAdminBase
{
public:
			CDlgAdminEfcBalloon(CWnd* pParent = NULL);			/* コンストラクタ */
	virtual ~CDlgAdminEfcBalloon();								/* デストラクタ */

	void	Init		(CMgrData *pMgrData, DWORD dwListID, BOOL bModify = FALSE);	/* 初期化 */
	void	GetList		(CLibInfoEfcBalloon *pDst);						/* 取得 */
	void	SetList		(CLibInfoEfcBalloon *pSrc);						/* 設定 */
	DWORD	GetListID	(void)	{ return m_dwListID; }					/* 種別IDを取得 */


protected:
	void	RenewSlider		(void);							/* スライダーを更新 */
	void	OnSelectAnime	(void);							/* コマ選択処理 */
	void	Enable			(BOOL bEnable);					/* 入力禁止設定 */
	void	Set				(int nNo);						/* 噴出し情報へ設定 */
	void	Get				(int nNo);						/* 噴出し情報から取得 */


protected:
	BOOL				m_bModify;					/* 編集モード */
	int					m_nSelect;					/* 選択中の番号 */
	DWORD				m_dwLastTimeAnime;			/* 前回のアニメーション時間 */
	CWndSelectGrp		*m_pWndSelectGrp;			/* 画像選択ウィンドウ */
	CLibInfoEfcBalloon	*m_pLibInfoEfcBalloon;		/* 編集中の噴出し情報 */



public:
	void OnOK ();
	//{{AFX_DATA(CDlgAdminEfcBalloon)
	enum { IDD = IDD_EFC_BALLOON };
	CSliderCtrl	m_ctlSlider;
	CComboBox	m_cmbSoundID;
	CStaticGrp	m_ctlGrp;
	BOOL	m_bLoop;
	int		m_nViewTime;
	DWORD	m_dwListID;
	CString	m_strCount;
	CString	m_strNow;
	CString	m_strName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminEfcBalloon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminEfcBalloon)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
} CDlgAdminEfcBalloon, *PCDlgAdminEfcBalloon;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
