/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemWeaponList.h									 */
/* 内容			:アイテム武器情報一覧ダイアログクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/10													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoItemWeapon;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminItemWeaponList : public CDlgAdminBase
{
public:
			CDlgAdminItemWeaponList(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminItemWeaponList();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	Renew		(void);											/* 一覧を更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* メッセージハンドラ(WM_MAINFRAME) */


protected:


protected:
	CWnd				*m_pWndNotify;				/* 通知先ウィンドウ */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;		/* アイテム武器情報 */



public:
	//{{AFX_DATA(CDlgAdminItemWeaponList)
	enum { IDD = IDD_ITEMWEAPONINFO_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemWeaponList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemWeaponList)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnCopy();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemWeaponList, *PCDlgAdminItemWeaponList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
