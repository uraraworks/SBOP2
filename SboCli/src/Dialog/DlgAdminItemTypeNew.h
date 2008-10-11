/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemTypeNew.h										 */
/* 内容			:アイテム種別新規追加ダイアログクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/30													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoItemTypeBase;
class CMgrGrpData;
class CWndSelectItemGrp;
class CLibInfoItemType;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminItemTypeNew : public CDlgAdminBase
{
public:
			CDlgAdminItemTypeNew(CWnd* pParent = NULL);			/* コンストラクタ */
	virtual ~CDlgAdminItemTypeNew();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData, CWnd **pWndNotify, BOOL bPosSet = TRUE);	/* 初期化 */
	void	Get			(CInfoItemTypeBase *&pDst);						/* 編集内容を取得 */
	void	SetModify	(CInfoItemTypeBase *pInfoItemType);				/* 編集モードとして設定 */


protected:
	void	SetGrpIDMain	(DWORD dwItemTypeID);	/* 装備メイン画像IDコンボボックスを設定 */


protected:
	DWORD				m_dwSelectItemTypeID,	/* 種別 */
						m_dwSelectSoundID,		/* 落としたときの効果音ID */
						m_dwUseEffectID,		/* 使った時に再生するエフェクトID */
						m_dwUseSoundID,			/* 使った時に再生する効果音ID */
						m_dwSelectGrpIDMain;	/* 装備メイン画像ID */
	BOOL				m_bModeModify;			/* 編集モード判定 */
	CWnd				**m_ppWndNotify;		/* 通知先ウィンドウ */
	CMgrGrpData			*m_pMgrGrpData;			/* グラフィックデータ管理 */
	CWndSelectItemGrp	*m_pWndSelectItemGrp;	/* アイテム画像選択ウィンドウ */
	CLibInfoItemType	*m_pLibInfoItemType;	/* アイテム種別情報ライブラリ */
	CInfoItemTypeBase	*m_pInfoItemType;		/* 編集中のアイテム種別情報 */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminItemTypeNew)
	enum { IDD = IDD_ITEMTYPE_NEW };
	CComboBox	m_ctlTypeGrpIDMain;
	CComboBox	m_ctlDropSoundID;
	CComboBox	m_ctlUseSoundID;
	CComboBox	m_ctlUseEffectID;
	CComboBox	m_ctlType;
	CString	m_strName;
	int		m_nDelAverage;
	int		m_nGrpID;
	BOOL	m_bPutOn;
	int		m_nGrpIDSub;
	int		m_nIconGrpID;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemTypeNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemTypeNew)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectItemgrp();
	afx_msg void OnPlay();
	afx_msg void OnSelchangeType();
	afx_msg void OnTypeinfo();
	//}}AFX_MSG
	afx_msg LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemTypeNew, *PCDlgAdminItemTypeNew;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
