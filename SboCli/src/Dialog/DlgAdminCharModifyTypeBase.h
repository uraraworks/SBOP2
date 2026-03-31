/// @file DlgAdminCharModifyTypeBase.h
/// @brief キャラ情報 移動種別の設定ダイアログクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/07/12
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "DlgAdminBase.h"

class CInfoCharCli;

// クラス宣言

typedef class CDlgAdminCharModifyTypeBase : public CDlgAdminBase
{
public:
			CDlgAdminCharModifyTypeBase(CWnd* pParent = NULL); // コンストラクタ
			CDlgAdminCharModifyTypeBase(int nResourceID, CWnd* pParent = NULL);
	virtual ~CDlgAdminCharModifyTypeBase(); // デストラクタ

	virtual void	Set(CInfoCharCli *pSrc); // 設定
	virtual void	Get(CInfoCharCli *pDst); // 取得



public:
	//{{AFX_DATA(CDlgAdminCharModifyTypeBase)
	enum { IDD = IDD_CHAR_MODIFY_TYPE_PUTNPC };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharModifyTypeBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharModifyTypeBase)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharModifyTypeBase, *PCDlgAdminCharModifyTypeBase;

//{{AFX_INSERT_LOCATION}}