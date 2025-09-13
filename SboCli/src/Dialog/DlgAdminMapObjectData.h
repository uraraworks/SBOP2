/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapObjectData.h									 */
/* 内容			:マップオブジェクト配置データ一覧ダイアログクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/03													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoMapBase;
class CLibInfoMapObject;
class CLibInfoMapObjectData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminMapObjectData : public CDlgAdminBase
{
public:
			CDlgAdminMapObjectData(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminMapObjectData();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */
	void	Renew		(void);											/* 一覧を更新 */


protected:
	CWnd					*m_pWndNotify;				/* 通知先ウィンドウ */
	CInfoMapBase			*m_pInfoMap;				/* 編集中のマップ情報 */
	CLibInfoMapObject		*m_pLibInfoMapObject;		/* マップオブジェクトデータライブラリ */
	CLibInfoMapObjectData	*m_pLibInfoMapObjectData;	/* マップオブジェクト配置データライブラリ */



public:
	//{{AFX_DATA(CDlgAdminMapObjectData)
	enum { IDD = IDD_MAP_OBJECTDATA };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapObjectData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapObjectData)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnCopy();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminMapObjectData, *PCDlgAdminMapObjectData;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
