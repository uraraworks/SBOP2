/// @file DlgAdminTalkSet.h
/// @brief 会話データ設定ダイアログクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/22
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "DlgAdminBase.h"

class CInfoTalkEvent;

// クラス宣言

typedef class CDlgAdminTalkSet : public CDlgAdminBase
{
public:
			CDlgAdminTalkSet(CWnd* pParent = NULL);	// コンストラクタ
	virtual ~CDlgAdminTalkSet();	// デストラクタ

public:
	CInfoTalkEvent	*m_pInfo;	// 編集中の会話イベント

public:
	void OnOK();
	//{{AFX_DATA(CDlgAdminTalkSet)
	enum { IDD = IDD_TALK_SET };
	CString	m_strTalk;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkSet)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTalkevent();
} CDlgAdminTalkSet, *PCDlgAdminTalkSet;

//{{AFX_INSERT_LOCATION}}

