/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharMotion.h										 */
/* 内容			:キャラモーション設定ダイアログクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/27													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "StaticGrp.h"
#include "afxcmn.h"
#include "afxwin.h"

class CMgrGrpData;
class CWndSelectGrp;
class CLibInfoMotion;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharMotion : public CDlgAdminBase
{
public:
			CDlgAdminCharMotion(CWnd* pParent = NULL);			/* コンストラクタ */
	virtual ~CDlgAdminCharMotion();							/* デストラクタ */

	void	Init	(CMgrData *pMgrData, DWORD dwMotionTypeID, DWORD dwMotionListID, int nGrpIDSub);	/* 初期化 */
	void	GetList	(CLibInfoMotion *pDst);								/* 取得 */
	void	SetList	(CLibInfoMotion *pSrc);								/* 設定 */


protected:
	void	RenewSlider		(void);							/* スライダーを更新 */
	void	RenewPreview	(void);							/* プレビュー画像を更新 */
	void	OnSelectAnime	(void);							/* コマ選択処理 */
	void	Enable			(BOOL bEnable);					/* 入力禁止設定 */
	void	Set				(int nNo);						/* モーション情報へ設定 */
	void	Get				(int nNo);						/* モーション情報から取得 */


protected:
	int				m_nSelect,				/* 選択中の番号 */
					m_nGrpIDSub;			/* グラフィックサブID */
	DWORD			m_dwMotionTypeID,		/* 編集中のモーション種別ID */
					m_dwMotionListID,		/* 編集中のモーションリストID */
					m_dwLastTimeAnime;		/* 前回のアニメーション時間 */
	CWndSelectGrp	*m_pWndSelectGrp;		/* 画像選択ウィンドウ */
	CLibInfoMotion	*m_pLibInfoMotion;		/* 編集中のモーション情報 */



public:
	void OnOK ();
	//{{AFX_DATA(CDlgAdminCharMotion)
	enum { IDD = IDD_CHAR_MOTION };
	CSliderCtrl	m_ctlSlider;
	CComboBox	m_cmbTypePile3;
	CComboBox	m_cmbTypePile2;
	CComboBox	m_cmbTypePile1;
	CComboBox	m_cmbTypeChar;
	CComboBox	m_cmbSoundID;
	CComboBox	m_cmbProcID;
	CStaticGrp	m_ctlGrpPreview;
	CStaticGrp	m_ctlGrpPile3;
	CStaticGrp	m_ctlGrpPile2;
	CStaticGrp	m_ctlGrpPile1;
	CStaticGrp	m_ctlGrpChar;
	BOOL	m_bLoop;
	BOOL	m_bPile;
	BOOL	m_bRedrawHand;
	int		m_nViewTime;
	int		m_nLevel1;
	int		m_nLevel2;
	int		m_nLevel3;
	int m_nViewPosX0;
	int m_nViewPosY0;
	int m_nViewPosX1;
	int m_nViewPosY1;
	int m_nViewPosX2;
	int m_nViewPosY2;
	int m_nViewPosX3;
	int m_nViewPosY3;
	CString	m_strCount;
	CString	m_strNow;
	CSpinButtonCtrl m_ctlSpinX0;
	CSpinButtonCtrl m_ctlSpinY0;
	CSpinButtonCtrl m_ctlSpinX1;
	CSpinButtonCtrl m_ctlSpinY1;
	CSpinButtonCtrl m_ctlSpinX2;
	CSpinButtonCtrl m_ctlSpinY2;
	CSpinButtonCtrl m_ctlSpinX3;
	CSpinButtonCtrl m_ctlSpinY3;
	CListBox m_ctlDrawList;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharMotion)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharMotion)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeTypeChar();
	afx_msg void OnSelchangeTypePile1();
	afx_msg void OnSelchangeTypePile2();
	afx_msg void OnSelchangeTypePile3();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnChangeViewpos();
	afx_msg void OnBnClickedDrawUp();
	afx_msg void OnBnClickedDrawDown();
	//}}AFX_MSG
	LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharMotion, *PCDlgAdminCharMotion;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
