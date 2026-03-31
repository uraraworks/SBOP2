/// @file DlgAdminCharDisableNew.h
/// @brief 拒否MACアドレスの追加ダイアログクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/04/11
/// @copyright Copyright(C)URARA-works 2009

#pragma once

#include "DlgAdminBase.h"

// クラス宣言

typedef class CDlgAdminCharDisableNew : public CDlgAdminBase
{
public:
			CDlgAdminCharDisableNew(CWnd* pParent = NULL); // コンストラクタ
	virtual ~CDlgAdminCharDisableNew(); // デストラクタ



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminCharDisableNew)
	enum { IDD = IDD_CHAR_DISABLE_NEW };
	CString	m_strMacAddress;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharDisableNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharDisableNew)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharDisableNew, *PCDlgAdminCharDisableNew;

//{{AFX_INSERT_LOCATION}}