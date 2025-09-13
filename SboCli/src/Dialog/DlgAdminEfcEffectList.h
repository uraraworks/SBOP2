/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminEfcEffectList.h									 */
/* 内容			:エフェクト一覧ダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/06													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoEffect;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminEfcEffectList : public CDlgAdminBase
{
public:
			CDlgAdminEfcEffectList(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminEfcEffectList();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	Renew		(void);											/* 一覧を更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* メッセージハンドラ(WM_MAINFRAME) */


protected:


protected:
	CLibInfoEffect	*m_pLibInfo;		/* エフェクト情報 */



public:
	//{{AFX_DATA(CDlgAdminEfcEffectList)
	enum { IDD = IDD_EFC_EFFECT_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminEfcEffectList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminEfcEffectList)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	afx_msg void OnRenew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminEfcEffectList, *PCDlgAdminEfcEffectList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
