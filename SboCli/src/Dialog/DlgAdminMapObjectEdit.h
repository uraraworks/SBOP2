/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapObjectEdit.h									 */
/* 内容			:マップオブジェクト編集ダイアログクラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CDlgAdminMapObjectEdit : public CDlgAdminBase
{
public:
			CDlgAdminMapObjectEdit(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual	~CDlgAdminMapObjectEdit();						/* デストラクタ */

	int DoModal (CMgrData *pMgrData);								/* モーダルダイアログ表示 */


protected:


public:



public:
	//{{AFX_DATA(CDlgAdminMapObjectEdit)
	enum { IDD = IDD_MAP_OBJECT_EDIT };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapObjectEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapObjectEdit)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
