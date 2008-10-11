/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminEfcEffect.h										 */
/* 内容			:エフェクト情報設定ダイアログクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/06													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "StaticGrp.h"

class CMgrGrpData;
class CWndSelectGrp;
class CInfoEffect;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminEfcEffect : public CDlgAdminBase
{
public:
			CDlgAdminEfcEffect(CWnd* pParent = NULL);			/* コンストラクタ */
	virtual ~CDlgAdminEfcEffect();								/* デストラクタ */

	void	Init	(CMgrData *pMgrData, BOOL bModify = FALSE);			/* 初期化 */
	void	GetInfo	(CInfoEffect *pDst);								/* 取得 */
	void	SetInfo	(CInfoEffect *pSrc);								/* 設定 */


protected:
	void	RenewSlider		(void);							/* スライダーを更新 */
	void	OnSelectAnime	(void);							/* コマ選択処理 */
	void	Enable			(BOOL bEnable);					/* 入力禁止設定 */
	void	Set				(int nNo);						/* エフェクト情報へ設定 */
	void	Get				(int nNo);						/* エフェクト情報から取得 */


protected:
	BOOL			m_bModify;						/* 編集モード */
	int				m_nSelect;						/* 選択中の番号 */
	DWORD			m_dwLastTimeAnime;				/* 前回のアニメーション時間 */
	CWndSelectGrp	*m_pWndSelectGrp;				/* 画像選択ウィンドウ */
	CInfoEffect		*m_pInfoEffect;					/* 編集中のエフェクト情報 */



public:
	void OnOK ();
	//{{AFX_DATA(CDlgAdminEfcEffect)
	enum { IDD = IDD_EFC_EFFECT };
	CSliderCtrl	m_ctlSlider;
	CComboBox	m_cmbSoundID;
	CComboBox	m_cmbType;
	CStaticGrp	m_ctlGrp;
	BOOL	m_bLoop;
	BOOL	m_bLoopSound;
	int		m_nViewTime;
	int		m_nLevel;
	CString	m_strCount;
	CString	m_strNow;
	CString	m_strName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminEfcEffect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminEfcEffect)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeType();
	//}}AFX_MSG
	LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
} CDlgAdminEfcEffect, *PCDlgAdminEfcEffect;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
