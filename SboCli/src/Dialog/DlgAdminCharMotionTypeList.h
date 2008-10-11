/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharMotionTypeList.h								 */
/* 内容			:キャラモーション種別リストダイアログクラス 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/05/26													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoMotionType;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharMotionTypeList : public CDlgAdminBase
{
public:
			CDlgAdminCharMotionTypeList(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual ~CDlgAdminCharMotionTypeList();						/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* メッセージハンドラ(WM_MAINFRAME) */

	void	Renew		(void);											/* リスト更新 */


protected:
	CLibInfoMotionType		*m_pLibInfoMotionType;		/* モーション種別情報 */



public:
	//{{AFX_DATA(CDlgAdminCharMotionTypeList)
	enum { IDD = IDD_CHAR_MOTIONTYPE_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharMotionTypeList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CDlgAdminCharMotionTypeList)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharMotionTypeList, *PCDlgAdminCharMotionTypeList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
