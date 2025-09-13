/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharAddNPC.h										 */
/* 内容			:NPCの追加ダイアログクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/01													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoCharCli;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharAddNPC : public CDlgAdminBase
{
public:
			CDlgAdminCharAddNPC(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminCharAddNPC();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);						/* 初期化 */
	void	Renew		(void);										/* 更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* メッセージハンドラ(WM_ADMINMSG) */


protected:
	CInfoCharCli	*m_pInfoChar;					/* 編集中のキャラ情報 */



public:
	//{{AFX_DATA(CDlgAdminCharAddNPC)
	enum { IDD = IDD_CHAR_ADDNPC };
	CComboBox	m_ctlFamilyType;
	CString	m_strCharName;
	int		m_nMapID;
	int		m_nPosX;
	int		m_nPosY;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharAddNPC)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharAddNPC)
	virtual BOOL OnInitDialog();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharAddNPC, *PCDlgAdminCharAddNPC;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
