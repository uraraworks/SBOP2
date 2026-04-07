/// @file DlgAdminMapEventNONE.h
/// @brief マップイベント(未設定)ダイアログクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/26
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "DlgAdminBase.h"

class CInfoMapEventBase;

// クラス宣言

typedef class CDlgAdminMapEventNONE : public CDlgAdminBase
{
public:
			CDlgAdminMapEventNONE(CWnd* pParent = NULL); // コンストラクタ
	virtual ~CDlgAdminMapEventNONE(); // デストラクタ

	virtual void	Init(CMgrData *pMgrData); // 初期化
	virtual void	Set(CInfoMapEventBase *pSrc); // 設定
	virtual void	Get(CInfoMapEventBase *pDst); // 取得


protected:
	int	m_nResourceID; // ダイアログリソースID



public:
	//{{AFX_DATA(CDlgAdminMapEventNONE)
	enum { IDD = IDD_MAP_EVENT_NONE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEventNONE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEventNONE)
	virtual BOOL OnInitDialog();
	virtual afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminMapEventNONE, *PCDlgAdminMapEventNONE;

//{{AFX_INSERT_LOCATION}}