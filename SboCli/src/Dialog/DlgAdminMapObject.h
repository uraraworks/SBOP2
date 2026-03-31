/// @file DlgAdminMapObject.h
/// @brief マップオブジェクト一覧ダイアログクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/09/07
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "DlgAdminBase.h"

class CLibInfoMapObject;

// クラス宣言

typedef class CDlgAdminMapObject : public CDlgAdminBase
{
public:
			CDlgAdminMapObject(CWnd* pParent = NULL); // コンストラクタ
	virtual ~CDlgAdminMapObject(); // デストラクタ

	void	Init(CMgrData *pMgrData); // 初期化
	void	OnAdminMsg(int nType, DWORD dwPara); // メッセージハンドラ(WM_ADMINMSG)
	void	Renew(void); // 一覧を更新


protected:


protected:
	CWnd	*m_pWndNotify; // 通知先ウィンドウ
	CLibInfoMapObject	*m_pLibInfoMapObject; // マップオブジェクト情報ライブラリ



public:
	//{{AFX_DATA(CDlgAdminMapObject)
	enum { IDD = IDD_MAP_EVENT };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapObject)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapObject)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnCopy();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminMapObject, *PCDlgAdminMapObject;

//{{AFX_INSERT_LOCATION}}