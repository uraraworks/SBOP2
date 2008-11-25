/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharModify.h										 */
/* 内容			:選択キャラ情報編集ダイアログクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/03/17													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoCharCli;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharModify : public CDlgAdminBase
{
public:
			CDlgAdminCharModify(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminCharModify();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);						/* 初期化 */
	void	Renew		(void);										/* 更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* メッセージハンドラ(WM_ADMINMSG) */


protected:
	void		Send		(BOOL bChgScreenPos);					/* 送信処理 */
	COLORREF	GetColor	(int nNo);								/* 番号から色を取得 */


protected:
	DWORD			m_dwCharID;					/* 最後に選択したキャラID */
	CInfoCharCli	*m_pInfoChar;				/* 編集中のキャラ情報 */



public:
	//{{AFX_DATA(CDlgAdminCharModify)
	enum { IDD = IDD_CHAR_MODIFY };
	CComboBox	m_ctlMoveType;
	CComboBox	m_ctlMotionType;
	CComboBox	m_ctlSex;
	CComboBox	m_ctlColor;
	CString	m_strCharName;
	CString	m_strCharID;
	BOOL	m_bBlock;
	BOOL	m_bPush;
	DWORD	m_dwMapID;
	int		m_nPosX;
	int		m_nPosY;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharModify)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharModify)
	virtual BOOL OnInitDialog();
	afx_msg void OnSend();
	afx_msg void OnSetMoveType();
	afx_msg void OnTalk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharModify, *PCDlgAdminCharModify;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
