/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharModifySkill.h									 */
/* 内容			:選択キャラ情報編集[所持スキルの設定]ダイアログクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/01/18													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoCharCli;
class CLibInfoSkill;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharModifySkill : public CDlgAdminBase
{
public:
			CDlgAdminCharModifySkill(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual ~CDlgAdminCharModifySkill();					/* デストラクタ */

	void	Init		(CMgrData *pMgrData);						/* 初期化 */
	void	Renew		(void);										/* 更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* メッセージハンドラ(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);			/* メッセージハンドラ(WM_MAINFRAME) */


protected:
	CInfoCharCli	*m_pInfoChar;					/* 編集中のキャラ情報 */
	CLibInfoSkill	*m_pLibInfoSkill;				/* スキル情報ライブラリ */



public:
	//{{AFX_DATA(CDlgAdminCharModifySkill)
	enum { IDD = IDD_CHAR_MODIFY_SKILL };
	CListCtrl	m_List;
	CComboBox	m_Combo;
	CString	m_strCharName;
	CString	m_strCharID;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharModifySkill)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharModifySkill)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharModifySkill, *PCDlgAdminCharModifySkill;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2009 */
