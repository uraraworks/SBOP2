/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapInfo.h											 */
/* 内容			:マップ情報編集ダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/01													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoMapBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminMapInfo : public CDlgAdminBase
{
public:
			CDlgAdminMapInfo(CWnd* pParent = NULL);			/* コンストラクタ */
	virtual ~CDlgAdminMapInfo();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);						/* 初期化 */
	void	Renew		(void);										/* 更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* メッセージハンドラ(WM_ADMINMSG) */


protected:
	CInfoMapBase	*m_pInfoMap;				/* 編集中のマップ情報 */



public:
	//{{AFX_DATA(CDlgAdminMapInfo)
	enum { IDD = IDD_MAP_INFO };
	CString	m_strMapSize;
	int		m_nValue;
	int		m_nDarkLevel;
	CString	m_strMapID;
	CString	m_strMapName;
	CComboBox	m_cmbBGMID;
	CComboBox	m_cmbWeatherType;
	BOOL m_bEnableBattle;
	BOOL m_bRecovery;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnChangemapname();
	afx_msg void OnCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminMapInfo, *PCDlgAdminMapInfo;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
