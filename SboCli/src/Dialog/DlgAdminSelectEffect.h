/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminSelectEffect.h										 */
/* 内容			:エフェクト選択ダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "ListCtrlEffectInfo.h"

class CLibInfoEffect;
class CInfoEffect;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminSelectEffect : public CDlgAdminBase
{
public:
			CDlgAdminSelectEffect(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminSelectEffect();							/* デストラクタ */

	DWORD	GetSelectID	(void);				/* 選択されたエフェクトIDを取得 */


protected:
	DWORD	m_dwSelectID;					/* 選択されたエフェクトID */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminSelectEffect)
	enum { IDD = IDD_SELECT_EFFECT };
	CListCtrlEffectInfo	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminSelectEffect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminSelectEffect)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminSelectEffect, *PCDlgAdminSelectEffect;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
