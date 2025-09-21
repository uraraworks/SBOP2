/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharSkillNONE.h									 */
/* 内容			:スキル(未設定)ダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoSkillBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharSkillNONE : public CDlgAdminBase
{
public:
			CDlgAdminCharSkillNONE(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminCharSkillNONE();							/* デストラクタ */

	virtual void	Init	(CMgrData *pMgrData);						/* 初期化 */
	virtual void	Set		(CInfoSkillBase *pSrc);						/* 設定 */
	virtual void	Get		(CInfoSkillBase *pDst);						/* 取得 */


protected:
	int		m_nResourceID;			/* ダイアログリソースID */



public:
	//{{AFX_DATA(CDlgAdminCharSkillNONE)
	enum { IDD = IDD_CHAR_SKILL_NONE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharSkillNONE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharSkillNONE)
	virtual BOOL OnInitDialog();
	virtual afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharSkillNONE, *PCDlgAdminCharSkillNONE;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
