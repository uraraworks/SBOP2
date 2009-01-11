/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharSkillMOVEATACK.h								 */
/* 内容			:スキル(移動して攻撃)ダイアログクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/01/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminCharSkillNONE.h"

class CInfoSkillBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharSkillMOVEATACK : public CDlgAdminCharSkillNONE
{
public:
			CDlgAdminCharSkillMOVEATACK(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual ~CDlgAdminCharSkillMOVEATACK();						/* デストラクタ */

	virtual void	Init	(CMgrData *pMgrData);						/* 初期化 */
	virtual void	Set		(CInfoSkillBase *pSrc);						/* 設定 */
	virtual void	Get		(CInfoSkillBase *pDst);						/* 取得 */


protected:



public:
	//{{AFX_DATA(CDlgAdminCharSkillMOVEATACK)
	enum { IDD = IDD_CHAR_SKILL_MOVEATACK };
	CComboBox	m_ctlTarget;
	CComboBox	m_ctlUp;
	CComboBox	m_ctlDown;
	CComboBox	m_ctlLeft;
	CComboBox	m_ctlRight;
	CComboBox	m_ctlPutType;
	DWORD	m_dwAliveTime;
	DWORD	m_dwWaitTime;
	DWORD	m_dwValue;
	DWORD	m_dwDistance;
	BOOL	m_bHitQuit;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharSkillMOVEATACK)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharSkillMOVEATACK)
	virtual BOOL OnInitDialog();
	virtual afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharSkillMOVEATACK, *PCDlgAdminCharSkillMOVEATACK;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2009 */
