/// @file DlgAdminItemTypeNewARMSBow.cpp
/// @brief アイテム種別(持ち物[弓用の設定])設定ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/08/29
/// @copyright Copyright(C)URARA-works 2009

#include "stdafx.h"
#include "SboCli.h"
#include "InfoItemTypeBase.h"
#include "DlgAdminItemTypeNewARMSBow.h"

// クラスの設定

IMPLEMENT_DYNAMIC(CDlgAdminItemTypeNewARMSBow, CDlgAdminBase)

void CDlgAdminItemTypeNewARMSBow::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MOVEWAIT, m_dwMoveWait);
	DDX_Text(pDX, IDC_MOVECOUNT, m_dwMoveCount);
}

BEGIN_MESSAGE_MAP(CDlgAdminItemTypeNewARMSBow, CDlgAdminBase)
END_MESSAGE_MAP()


CDlgAdminItemTypeNewARMSBow::CDlgAdminItemTypeNewARMSBow(CWnd* pParent /*=NULL*/)
: CDlgAdminBase(CDlgAdminItemTypeNewARMSBow::IDD, pParent)
, m_dwMoveWait(0)
, m_dwMoveCount(0)
{
}


CDlgAdminItemTypeNewARMSBow::~CDlgAdminItemTypeNewARMSBow()
{
}


void CDlgAdminItemTypeNewARMSBow::Set(CInfoItemTypeBase *pSrc)
{
	m_dwMoveWait	= pSrc->m_dwMoveWait; // 速度
	m_dwMoveCount	= pSrc->m_dwMoveCount; // 飛距離

	UpdateData(FALSE);
}


void CDlgAdminItemTypeNewARMSBow::Get(CInfoItemTypeBase *&pDst)
{
	UpdateData();

	pDst->m_dwMoveWait	= m_dwMoveWait; // 速度
	pDst->m_dwMoveCount	= m_dwMoveCount; // 飛距離
}