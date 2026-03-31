/// @file DlgAdminTmp.h
/// @brief ダイアログクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/23
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "DlgAdminBase.h"

// クラス宣言

typedef class CDlgAdminTmp : public CDlgAdminBase
{
public:
			CDlgAdminTmp(CWnd* pParent = NULL); // コンストラクタ
	virtual ~CDlgAdminTmp(); // デストラクタ

	void	Init(CMgrData *pMgrData); // 初期化
	void	OnAdminMsg(int nType, DWORD dwPara); // メッセージハンドラ(WM_ADMINMSG)


protected:


protected:



public:
	//{{AFX_DATA(CDlgAdminTmp)
	enum { IDD = IDD_ };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTmp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTmp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminTmp, *PCDlgAdminTmp;

//{{AFX_INSERT_LOCATION}}