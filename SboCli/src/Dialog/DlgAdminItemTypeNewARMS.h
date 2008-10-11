/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemTypeNewARMS.h									 */
/* 内容			:アイテム種別(持ち物)設定ダイアログクラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/08													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoItemTypeBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminItemTypeNewARMS : public CDlgAdminBase
{
public:
			CDlgAdminItemTypeNewARMS(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminItemTypeNewARMS();						/* デストラクタ */

	void	Set		(CInfoItemTypeBase *pSrc);							/* 編集内容を設定 */
	void	Get		(CInfoItemTypeBase *&pDst);							/* 編集内容を取得 */


protected:
	void	SetWeaponType	(DWORD dwWeaponInfoID);		/* 武器種別を設定 */
	DWORD	GetWeaponType	(void);						/* 武器種別を取得 */


protected:
	DWORD	m_dwWeaponInfoID;				/* 武器情報ID */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminItemTypeNewARMS)
	enum { IDD = IDD_ITEMTYPE_NEW_ARMS };
	CComboBox	m_ctlWeaponType;
	DWORD m_dwValue;
	DWORD m_dwMoveWait;
	DWORD m_dwMoveCount;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemTypeNewARMS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemTypeNewARMS)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemTypeNewARMS, *PCDlgAdminItemTypeNewARMS;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
