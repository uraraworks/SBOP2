/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemTypeNewLIGHT.h									 */
/* 内容			:アイテム種別(灯り)設定ダイアログクラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/10/19													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoItemTypeBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminItemTypeNewLIGHT : public CDlgAdminBase
{
public:
			CDlgAdminItemTypeNewLIGHT(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual ~CDlgAdminItemTypeNewLIGHT();						/* デストラクタ */

	void	Set		(CInfoItemTypeBase *pSrc);							/* 編集内容を設定 */
	void	Get		(CInfoItemTypeBase *&pDst);							/* 編集内容を取得 */


public:
	//{{AFX_DATA(CDlgAdminItemTypeNewLIGHT)
	enum { IDD = IDD_ITEMTYPE_NEW_LIGHT };
	int m_nValue1;
	int m_nValue2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemTypeNewLIGHT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemTypeNewLIGHT)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemTypeNewLIGHT, *PCDlgAdminItemTypeNewLIGHT;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
