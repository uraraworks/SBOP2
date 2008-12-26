/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventBase.h									 */
/* 内容			:会話イベントの設定ダイアログクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "afxwin.h"

class CDlgAdminTalkEventNONE;
class CInfoTalkEventBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventBase : public CDlgAdminBase
{
public:
			CDlgAdminTalkEventBase(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminTalkEventBase();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData, CInfoTalkEventBase *pInfo = NULL);	/* 初期化 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */
	void	Get			(CInfoTalkEventBase *&pDst);					/* 取得 */


protected:


protected:
	CDlgAdminTalkEventNONE	*m_pDlgType;	/* 編集中のイベント種別ダイアログ */
	CInfoTalkEventBase		*m_pInfo;		/* 編集中のイベント情報 */



public:
	void OnOK();
	//{{AFX_DATA(CDlgAdminTalkEventBase)
	enum { IDD = IDD_TALKEVENT_BASE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventBase)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Type;
	afx_msg void OnCbnSelchangeType();
} CDlgAdminTalkEventBase, *PCDlgAdminTalkEventBase;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
