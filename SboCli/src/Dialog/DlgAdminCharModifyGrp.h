/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharModifyGrp.h									 */
/* 内容			:選択キャラ情報編集[画像の設定]ダイアログクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/02													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoCharCli;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharModifyGrp : public CDlgAdminBase
{
public:
			CDlgAdminCharModifyGrp(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual ~CDlgAdminCharModifyGrp();						/* デストラクタ */

	void	Init		(CMgrData *pMgrData);						/* 初期化 */
	void	Renew		(void);										/* 更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* メッセージハンドラ(WM_ADMINMSG) */


protected:
	CInfoCharCli	*m_pInfoChar;					/* 編集中のキャラ情報 */



public:
	//{{AFX_DATA(CDlgAdminCharModifyGrp)
	enum { IDD = IDD_CHAR_MODIFY_GRP };
	CString	m_strCharName;
	CString	m_strCharID;
	int		m_nGrpIDEye;
	int		m_nGrpIDEyeColor;
	int		m_nGrpIDHair;
	int		m_nGrpIDHairColor;
	int		m_nGrpIDCloth;
	int		m_nGrpIDSP;
	int		m_nGrpIDNPC;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharModifyGrp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharModifyGrp)
	virtual BOOL OnInitDialog();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharModifyGrp, *PCDlgAdminCharModifyGrp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
