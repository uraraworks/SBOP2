/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemNew.h											 */
/* 内容			:アイテム新規追加ダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/15													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoItemType;
class CInfoItem;
class CMgrGrpData;
class CWndSelectItemGrp;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminItemNew : public CDlgAdminBase
{
public:
			CDlgAdminItemNew(CWnd* pParent = NULL);			/* コンストラクタ */
	virtual ~CDlgAdminItemNew();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData, CWnd **pWndNotify, BOOL bPosSet = TRUE);	/* 初期化 */
	void	Get			(CInfoItem *pDst);							/* 編集内容を取得 */
	void	SetModify	(CInfoItem *pInfoItem);						/* 編集モードとして設定 */


protected:
	void	SelectSound	(DWORD dwSoundID);					/* 落としたときの効果音IDを選択 */


protected:
	DWORD				m_dwSelectItemTypeID,	/* 種別 */
						m_dwSelectSoundID;		/* 落としたときの効果音ID */
	BOOL				m_bModeModify,			/* 編集モード判定 */
						m_bPosSet;				/* 設置位置を設定できるか判定 */
	CWnd				**m_ppWndNotify;		/* 通知先ウィンドウ */
	CMgrGrpData			*m_pMgrGrpData;			/* グラフィックデータ管理 */
	CWndSelectItemGrp	*m_pWndSelectItemGrp;	/* アイテム画像選択ウィンドウ */
	CLibInfoItemType	*m_pLibInfoItemType;	/* アイテム種別情報ライブラリ */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminItemNew)
	enum { IDD = IDD_ITEM_NEW };
	CComboBox	m_ctlDropSoundID;
	CComboBox	m_ctlType;
	CString	m_strName;
	int		m_nMapID;
	int		m_nPosX;
	int		m_nPosY;
	int		m_nGrpID;
	BOOL	m_bPutOn;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemNew)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectItemgrp();
	afx_msg void OnPlay();
	afx_msg void OnSelchangeType();
	//}}AFX_MSG
	afx_msg LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemNew, *PCDlgAdminItemNew;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
