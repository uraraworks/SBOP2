/// @file DlgAdminCharSkillHEAL.h
/// @brief スキル(回復)ダイアログクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/01/22
/// @copyright Copyright(C)URARA-works 2009

#pragma once

#include "DlgAdminCharSkillNONE.h"

class CInfoSkillBase;

// クラス宣言

typedef class CDlgAdminCharSkillHEAL : public CDlgAdminCharSkillNONE
{
public:
			CDlgAdminCharSkillHEAL(CWnd* pParent = NULL); // コンストラクタ
	virtual ~CDlgAdminCharSkillHEAL(); // デストラクタ

	virtual void	Init(CMgrData *pMgrData); // 初期化
	virtual void	Set(CInfoSkillBase *pSrc); // 設定
	virtual void	Get(CInfoSkillBase *pDst); // 取得



public:
	//{{AFX_DATA(CDlgAdminCharSkillHEAL)
	enum { IDD = IDD_CHAR_SKILL_HEAL };
	CComboBox	m_ctlArea;
	CComboBox	m_ctlHealType;
	CComboBox	m_ctlHitEffect;
	DWORD	m_dwValue1;
	DWORD	m_dwValue2;
	DWORD	m_dwDistance;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharSkillHEAL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharSkillHEAL)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharSkillHEAL, *PCDlgAdminCharSkillHEAL;

//{{AFX_INSERT_LOCATION}}