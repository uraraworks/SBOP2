/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventNONE.h									 */
/* 内容			:会話イベント設定(未選択)ダイアログクラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoTalkEventBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventNONE : public CDlgAdminBase
{
public:
			CDlgAdminTalkEventNONE(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminTalkEventNONE();							/* デストラクタ */

	virtual void	Init		(CMgrData *pMgrData);					/* 初期化 */
	virtual void	OnAdminMsg	(int nType, DWORD dwPara);				/* メッセージハンドラ(WM_ADMINMSG) */
	virtual void	Set			(CInfoTalkEventBase *pSrc);				/* 設定から画面に反映 */
	virtual void	Get			(CInfoTalkEventBase *pDst);				/* 画面から設定に反映 */


protected:
	int		m_nResourceID;		/* ダイアログリソースID */


protected:



public:
	//{{AFX_DATA(CDlgAdminTalkEventNONE)
	enum { IDD = IDD_TALKEVENT_NONE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventNONE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventNONE)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminTalkEventNONE, *PCDlgAdminTalkEventNONE;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
