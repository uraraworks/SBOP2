/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemTypeNewARMSBow.h								 */
/* 内容			:アイテム種別(持ち物[弓用の設定])設定ダイアログクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/08/29													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoItemTypeBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CDlgAdminItemTypeNewARMSBow : public CDlgAdminBase
{
	DECLARE_DYNAMIC(CDlgAdminItemTypeNewARMSBow)

public:
			CDlgAdminItemTypeNewARMSBow(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminItemTypeNewARMSBow();							/* デストラクタ */

	void	Set		(CInfoItemTypeBase *pSrc);								/* 編集内容を設定 */
	void	Get		(CInfoItemTypeBase *&pDst);								/* 編集内容を取得 */


public:
	enum { IDD = IDD_ITEMTYPE_NEW_ARMS_BOW };
	DWORD m_dwMoveWait;
	DWORD m_dwMoveCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};

/* Copyright(C)URARA-works 2009 */
