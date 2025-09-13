/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharModifyItem.h									 */
/* 内容			:選択キャラ情報編集[所持アイテムの設定]ダイアログクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoCharCli;
class CLibInfoItem;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharModifyItem : public CDlgAdminBase
{
public:
			CDlgAdminCharModifyItem(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual ~CDlgAdminCharModifyItem();						/* デストラクタ */

	void	Init		(CMgrData *pMgrData);						/* 初期化 */
	void	Renew		(void);										/* 更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* メッセージハンドラ(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);			/* メッセージハンドラ(WM_MAINFRAME) */


protected:
	CInfoCharCli	*m_pInfoChar;					/* 編集中のキャラ情報 */
	CLibInfoItem	*m_pLibInfoItem;				/* アイテム情報ライブラリ */



public:
	//{{AFX_DATA(CDlgAdminCharModifyItem)
	enum { IDD = IDD_CHAR_MODIFY_ITEM };
	CListCtrl	m_List;
	CString	m_strCharName;
	CString	m_strCharID;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharModifyItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharModifyItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddnew();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharModifyItem, *PCDlgAdminCharModifyItem;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
