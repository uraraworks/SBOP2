/// @file DlgAdminTalkEventADDSKILL.h
/// @brief 会話イベント設定(スキル追加)ダイアログクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/31
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "DlgAdminTalkEventNONE.h"

// クラス宣言

typedef class CDlgAdminTalkEventADDSKILL : public CDlgAdminTalkEventNONE
{
public:
			CDlgAdminTalkEventADDSKILL(CWnd* pParent = NULL);	// コンストラクタ
	virtual ~CDlgAdminTalkEventADDSKILL();	// デストラクタ

	void	Set	(CInfoTalkEventBase *pSrc);	// 設定から画面に反映
	void	Get	(CInfoTalkEventBase *pDst);	// 画面から設定に反映

protected:

protected:

public:
	//{{AFX_DATA(CDlgAdminTalkEventADDSKILL)
	enum { IDD = IDD_TALKEVENT_ADDSKILL };
	CComboBox m_Combo;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventADDSKILL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventADDSKILL)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminTalkEventADDSKILL, *PCDlgAdminTalkEventADDSKILL;

//{{AFX_INSERT_LOCATION}}

