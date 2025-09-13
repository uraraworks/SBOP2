/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapObjectDataEdit.h								 */
/* 内容			:マップオブジェクト配置データ編集ダイアログクラス 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/03													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoMapObject;
class CInfoMapObjectData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminMapObjectDataEdit : public CDlgAdminBase
{
public:
			CDlgAdminMapObjectDataEdit(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual ~CDlgAdminMapObjectDataEdit();						/* デストラクタ */

	void	Init		(CMgrData *pMgrData, CWnd **pWndNotify);		/* 初期化 */
	void	Get			(CInfoMapObjectData *&pDst);					/* 編集内容を取得 */
	void	SetModify	(CInfoMapObjectData *pSrc);						/* 編集モードとして設定 */


protected:
	BOOL					m_bModeModify;			/* 編集モード判定 */
	CWnd					**m_ppWndNotify;		/* 通知先ウィンドウ */
	CLibInfoMapObject		*m_pLibInfoMapObject;	/* マップオブジェクト情報 */
	CInfoMapObjectData		*m_pInfo;				/* 編集中の配置データ */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminMapObjectDataEdit)
	enum { IDD = IDD_MAP_OBJECTDATA_EDIT };
	CComboBox	m_ctlType;
	int		m_nPosX;
	int		m_nPosY;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapObjectDataEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapObjectDataEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeType();
	afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminMapObjectDataEdit, *PCDlgAdminMapObjectDataEdit;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
