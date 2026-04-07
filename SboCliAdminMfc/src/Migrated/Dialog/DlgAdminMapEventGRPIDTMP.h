/// @file DlgAdminMapEventGRPIDTMP.h
/// @brief マップイベント(一時画像設定)ダイアログクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/10/12
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "DlgAdminMapEventNONE.h"

// クラス宣言

typedef class CDlgAdminMapEventGRPIDTMP : public CDlgAdminMapEventNONE
{
public:
			CDlgAdminMapEventGRPIDTMP(CWnd* pParent = NULL); // コンストラクタ
	virtual ~CDlgAdminMapEventGRPIDTMP(); // デストラクタ

	void	Set(CInfoMapEventBase *pSrc); // 設定
	void	Get(CInfoMapEventBase *pDst); // 取得



public:
	//{{AFX_DATA(CDlgAdminMapEventGRPIDTMP)
	enum { IDD = IDD_MAP_EVENT_GRPIDTMP };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEventGRPIDTMP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEventGRPIDTMP)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox	m_ctlIDMain;
	int m_nSelect;
	DWORD m_dwIDSub;
} CDlgAdminMapEventGRPIDTMP, *PCDlgAdminMapEventGRPIDTMP;

//{{AFX_INSERT_LOCATION}}