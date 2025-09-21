/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharModifyTypePUTNPC.h								 */
/* 内容			:キャラ情報 移動種別の設定(NPC発生)ダイアログクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/12													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminCharModifyTypeBase.h"

class CInfoCharCli;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharModifyTypePUTNPC : public CDlgAdminCharModifyTypeBase
{
public:
			CDlgAdminCharModifyTypePUTNPC(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminCharModifyTypePUTNPC();							/* デストラクタ */

	virtual void	Set	(CInfoCharCli *pSrc);									/* 設定 */
	virtual void	Get	(CInfoCharCli *pDst);									/* 取得 */


protected:
	int	m_nPutMoveType;			/* 移動種別 */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminCharModifyTypePUTNPC)
	enum { IDD = IDD_CHAR_MODIFY_TYPE_PUTNPC };
	CComboBox	m_ctlMoveType;
	DWORD m_dwPutCycle;
	int	m_nMaxPutCount;
	int	m_nPutAverage;
	int	m_nPutAreaX;
	int	m_nPutAreaY;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharModifyTypePUTNPC)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharModifyTypePUTNPC)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharModifyTypePUTNPC, *PCDlgAdminCharModifyTypePUTNPC;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
