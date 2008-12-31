/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharSkillBase.h									 */
/* 内容			:スキル編集ダイアログクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoSkillBase;
class CDlgAdminCharSkillNONE;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharSkillBase : public CDlgAdminBase
{
public:
			CDlgAdminCharSkillBase(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual ~CDlgAdminCharSkillBase();						/* デストラクタ */

	void	Init		(CMgrData *pMgrData);						/* 初期化 */
	void	Get			(CInfoSkillBase *&pDst);					/* 編集内容を取得 */
	void	SetModify	(CInfoSkillBase *pSrc);						/* 編集モードとして設定 */


protected:


protected:
	int			m_nTypeMain,				/* スキル種別(メイン) */
				m_nTypeSub,					/* スキル種別(サブ) */
				m_nUse;						/* 使用制限 */
	BOOL		m_bModeModify;				/* 編集モード判定 */
	CDlgAdminCharSkillNONE	*m_pDlgType;	/* 編集中のイベント種別ダイアログ */
	CInfoSkillBase			*m_pInfo;		/* 編集中のスキル情報 */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminCharSkillBase)
	enum { IDD = IDD_CHAR_SKILL_BASE };
	CComboBox	m_ctlTypeMain;
	CComboBox	m_ctlTypeSub;
	CComboBox	m_ctlUse;
	CString m_strName;
	DWORD m_dwSP;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharSkillBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharSkillBase)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTypeMain();
	afx_msg void OnSelchangeTypeSub();
	afx_msg void OnSelchangeUse();
	afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharSkillBase, *PCDlgAdminCharSkillBase;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
