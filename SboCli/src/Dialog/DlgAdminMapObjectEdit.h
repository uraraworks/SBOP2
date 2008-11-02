/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapObjectEdit.h									 */
/* 内容			:マップオブジェクト編集ダイアログクラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "StaticMapObjectGrp.h"
#include "afxcmn.h"

class CMgrData;
class CWndMapPartsGrp;
class CInfoMapObject;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CDlgAdminMapObjectEdit : public CDlgAdminBase
{
public:
			CDlgAdminMapObjectEdit(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual	~CDlgAdminMapObjectEdit();						/* デストラクタ */

	void	SetData	(CInfoMapObject *pSrc);							/* 編集用にデータを設定 */
	void	GetData	(CInfoMapObject *pDst);							/* 編集したデータを取得 */


protected:
	void	Set				(void);				/* 画面に反映 */
	void	Get				(void);				/* 画面から取得 */
	void	OnSelectAnime	(void);				/* コマ選択処理 */


public:
	CWndMapPartsGrp		*m_pWndMapPartsGrp;		/* マップパーツ画像一覧 */
	CInfoMapObject		*m_pInfoMapObject;		/* 編集中のマップオブジェクト情報 */



public:
	//{{AFX_DATA(CDlgAdminMapObjectEdit)
	enum { IDD = IDD_MAP_OBJECT_EDIT };
	CComboBox	m_cbGrpNo;
	int		m_nGrpNo;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapObjectEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapObjectEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeGrpNo();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl	m_Slider;
	CStaticMapObjectGrp m_ctlObject;
	CString m_strName;
	int m_nCx;
	int m_nCy;
	afx_msg void OnEnChangeCx();
	afx_msg void OnEnChangeCy();
	afx_msg void OnBnClickedAdd();
	int m_nCount;
	CSpinButtonCtrl m_ctlSpinX;
	CSpinButtonCtrl m_ctlSpinY;
	int m_nNow;
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
