/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemTypeNewARMSBow.cpp								 */
/* 内容			:アイテム種別(持ち物[弓用の設定])設定ダイアログクラス 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/08/29													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SboCli.h"
#include "InfoItemTypeBase.h"
#include "DlgAdminItemTypeNewARMSBow.h"

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

IMPLEMENT_DYNAMIC(CDlgAdminItemTypeNewARMSBow, CDlgAdminBase)

void CDlgAdminItemTypeNewARMSBow::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MOVEWAIT, m_dwMoveWait);
	DDX_Text(pDX, IDC_MOVECOUNT, m_dwMoveCount);
}

BEGIN_MESSAGE_MAP(CDlgAdminItemTypeNewARMSBow, CDlgAdminBase)
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewARMSBow::CDlgAdminItemTypeNewARMSBow		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/08/29														 */
/* ========================================================================= */

CDlgAdminItemTypeNewARMSBow::CDlgAdminItemTypeNewARMSBow(CWnd* pParent /*=NULL*/)
: CDlgAdminBase(CDlgAdminItemTypeNewARMSBow::IDD, pParent)
, m_dwMoveWait(0)
, m_dwMoveCount(0)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewARMSBow::~CDlgAdminItemTypeNewARMSBow		 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/08/29														 */
/* ========================================================================= */

CDlgAdminItemTypeNewARMSBow::~CDlgAdminItemTypeNewARMSBow()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewARMSBow::Set								 */
/* 内容		:編集内容を設定													 */
/* 日付		:2009/08/29														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMSBow::Set(CInfoItemTypeBase *pSrc)
{
	m_dwMoveWait	= pSrc->m_dwMoveWait;			/* 速度 */
	m_dwMoveCount	= pSrc->m_dwMoveCount;			/* 飛距離 */

	UpdateData(FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewARMSBow::Get								 */
/* 内容		:編集内容を取得													 */
/* 日付		:2009/08/29														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMSBow::Get(CInfoItemTypeBase *&pDst)
{
	UpdateData();

	pDst->m_dwMoveWait	= m_dwMoveWait;			/* 速度 */
	pDst->m_dwMoveCount	= m_dwMoveCount;		/* 飛距離 */
}

/* Copyright(C)URARA-works 2009 */
