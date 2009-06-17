/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharModifyStatus.h									 */
/* 内容			:選択キャラ情報編集[ステータス]ダイアログクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/12													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoCharCli;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharModifyStatus : public CDlgAdminBase
{
public:
			CDlgAdminCharModifyStatus(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual ~CDlgAdminCharModifyStatus();						/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	Renew		(void);											/* 更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* メッセージハンドラ(WM_MAINFRAME) */


protected:
	CInfoCharCli	*m_pInfoChar;					/* 編集中のキャラ情報 */



public:
	//{{AFX_DATA(CDlgAdminCharModifyStatus)
	enum { IDD = IDD_CHAR_MODIFY_STATUS };
	CString	m_strCharName;
	CString	m_strCharID;
	DWORD	m_dwMoveWait;
	DWORD	m_dwMoveWaitBattle;
	DWORD	m_dwHP;
	DWORD	m_dwMaxHP;
	DWORD	m_dwSP;
	DWORD	m_dwMaxSP;
	DWORD	m_dwLightTime;
	int	m_nDropItemAverage;
	int	m_nSearchX;
	int	m_nSearchY;
	int	m_nMoveAverage;
	int	m_nMoveAverageBattle;
	int	m_nLightLevel;
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

	//{{AFX_VIRTUAL(CDlgAdminCharModifyStatus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharModifyStatus)
	virtual BOOL OnInitDialog();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharModifyStatus, *PCDlgAdminCharModifyStatus;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
