/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminSystemSetInitCharStatus.h							 */
/* 内容			:キャラステータス初期値の設定ダイアログクラス 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/22													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminSystemSetInitCharStatus : public CDlgAdminBase
{
public:
			CDlgAdminSystemSetInitCharStatus(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual ~CDlgAdminSystemSetInitCharStatus();					/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	Renew		(void);											/* 更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* メッセージハンドラ(WM_MAINFRAME) */


protected:



public:
	//{{AFX_DATA(CDlgAdminSystemSetInitCharStatus)
	enum { IDD = IDD_SYSTEM_SET_INITCHARSTATUS };
	DWORD	m_dwMapID;
	int	m_nPosX;
	int	m_nPosY;
	DWORD	m_dwMaxHP;
	DWORD	m_dwMaxSP;
	DWORD	m_dwStamina;
	DWORD	m_dwPower;
	DWORD	m_dwStrength;
	DWORD	m_dwMagic;
	DWORD	m_dwSkillful;
	DWORD	m_dwAbillityAT;
	DWORD	m_dwAbillityDF;
	DWORD	m_dwPAtack;
	DWORD	m_dwPDefense;
	DWORD	m_dwPMagic;
	DWORD	m_dwPMagicDefense;
	DWORD	m_dwPHitAverage;
	DWORD	m_dwPAvoidAverage;
	DWORD	m_dwPCriticalAverage;
	DWORD	m_dwAttrFire;
	DWORD	m_dwAttrWind;
	DWORD	m_dwAttrWater;
	DWORD	m_dwAttrEarth;
	DWORD	m_dwAttrLight;
	DWORD	m_dwAttrDark;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminSystemSetInitCharStatus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminSystemSetInitCharStatus)
	virtual BOOL OnInitDialog();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminSystemSetInitCharStatus, *PCDlgAdminSystemSetInitCharStatus;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
