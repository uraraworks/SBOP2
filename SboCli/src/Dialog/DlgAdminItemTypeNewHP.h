/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemTypeNewHP.h									 */
/* 内容			:アイテム種別(HP増減)設定ダイアログクラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/15													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoItemTypeBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminItemTypeNewHP : public CDlgAdminBase
{
public:
			CDlgAdminItemTypeNewHP(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminItemTypeNewHP();							/* デストラクタ */

	void	Set		(CInfoItemTypeBase *pSrc);							/* 編集内容を設定 */
	void	Get		(CInfoItemTypeBase *&pDst);							/* 編集内容を取得 */


protected:
	void	SetTarget	(BYTE byTarget);			/* 使用対象を設定 */
	BYTE	GetTarget	(void);						/* 使用対象を取得 */
	void	SetArea		(BYTE byArea);				/* 使用範囲を設定 */
	BYTE	GetArea		(void);						/* 使用範囲を取得 */


protected:
	BYTE	m_byTarget;				/* 使用対象 */
	BYTE	m_byArea;				/* 使用範囲 */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminItemTypeNewHP)
	enum { IDD = IDD_ITEMTYPE_NEW_HP };
	CComboBox	m_ctlTarget;
	CComboBox	m_ctlArea;
	int m_nValue1;
	int m_nValue2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemTypeNewHP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemTypeNewHP)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemTypeNewHP, *PCDlgAdminItemTypeNewHP;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
