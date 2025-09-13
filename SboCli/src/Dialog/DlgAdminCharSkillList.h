/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharSkillList.h									 */
/* 内容			:スキルリストダイアログクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoSkill;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharSkillList : public CDlgAdminBase
{
public:
			CDlgAdminCharSkillList(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminCharSkillList();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* メッセージハンドラ(WM_MAINFRAME) */

	void	Renew		(void);											/* リスト更新 */


protected:
	CLibInfoSkill		*m_pLibInfoSkill;		/* スキル情報 */



public:
	//{{AFX_DATA(CDlgAdminCharSkillList)
	enum { IDD = IDD_CHAR_SKILL_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharSkillList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CDlgAdminCharSkillList)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharSkillList, *PCDlgAdminCharSkillList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
