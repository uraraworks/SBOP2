/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapEventBase.h										 */
/* 内容			:マップイベント編集ダイアログクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/24													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CDlgAdminMapEventNONE;
class CInfoMapEventBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminMapEventBase : public CDlgAdminBase
{
public:
			CDlgAdminMapEventBase(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual ~CDlgAdminMapEventBase();						/* デストラクタ */

	void	Init		(CMgrData *pMgrData, CWnd **pWndNotify);	/* 初期化 */
	void	Get			(CInfoMapEventBase *&pDst);					/* 編集内容を取得 */
	void	SetModify	(CInfoMapEventBase *pSrc);					/* 編集モードとして設定 */


protected:
	void SelectSound(DWORD dwSoundID);		/* 実行時の効果音IDを選択 */


protected:
	int			m_nEventType,				/* イベント種別 */
				m_nHitType,					/* 当たり判定種別 */
				m_nHitDirection;			/* 判定向き */
	DWORD		m_dwSoundID;				/* 実行時の効果音ID */
	BOOL		m_bModeModify;				/* 編集モード判定 */
	CWnd		**m_ppWndNotify;			/* 通知先ウィンドウ */
	CDlgAdminMapEventNONE	*m_pDlgType;	/* 編集中のイベント種別ダイアログ */
	CInfoMapEventBase		*m_pInfo;		/* 編集中のイベント情報 */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminMapEventBase)
	enum { IDD = IDD_MAP_EVENT_BASE };
	CComboBox	m_ctlType;
	CComboBox	m_ctlHitType;
	CComboBox	m_ctlDirection;
	CComboBox	m_ctlSoundID;
	int		m_nPosX;
	int		m_nPosY;
	int		m_nPosX2;
	int		m_nPosY2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEventBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEventBase)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeType();
	afx_msg void OnSelchangeHitType();
	afx_msg void OnSelchangeDirection();
	afx_msg void OnSelchangeSoundID();
	afx_msg void OnBnClickedPlay();
	afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminMapEventBase, *PCDlgAdminMapEventBase;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
