/// @file DlgAdminCharMotionTypeList.cpp
/// @brief キャラモーション種別リストダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/05/26
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoMotion.h"
#include "LibInfoMotionType.h"
#include "PacketADMIN_CHAR_ADDMOTIONTYPE.h"
#include "PacketADMIN_CHAR_RENEWMOTIONTYPE.h"
#include "MgrData.h"
#include "DlgAdminCharMotionList.h"
#include "DlgAdminCharMotion.h"
#include "DlgAdminCharMotionTypeList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminCharMotionTypeList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharMotionTypeList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharMotionTypeList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharMotionTypeList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgAdminCharMotionTypeList::CDlgAdminCharMotionTypeList(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminCharMotionTypeList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharMotionTypeList)
	//}}AFX_DATA_INIT

	m_pLibInfoMotionType = NULL;
}

CDlgAdminCharMotionTypeList::~CDlgAdminCharMotionTypeList()
{
}

void CDlgAdminCharMotionTypeList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init(pMgrData);

	m_pLibInfoMotionType = m_pMgrData->GetLibInfoMotionType();

	// ウィンドウ作成
	Create(CDlgAdminCharMotionTypeList::IDD, m_pWndParent);
	ShowWindow(SW_SHOW);
}

void CDlgAdminCharMotionTypeList::Renew(void)
{
	int i, nCount;
	PCInfoMotionType pInfo;
	CmyString strTmp;

	m_List.DeleteAllItems();

	nCount = m_pLibInfoMotionType->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMotionType)m_pLibInfoMotionType->GetPtr(i);

		strTmp.Format(_T("%d"), pInfo->m_dwMotionTypeID);
		m_List.InsertItem(i, strTmp);
		CString strName = (LPCTSTR)pInfo->m_strName;
		m_List.SetItemText(i, 1, strName);
		m_List.SetItemData(i, pInfo->m_dwMotionTypeID);
	}
}

void CDlgAdminCharMotionTypeList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWMOTIONTYPE:	// モーション種別情報更新
		Renew();
		break;
	}
}

BOOL CDlgAdminCharMotionTypeList::OnInitDialog()
{
	CmyString strTmp;

	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn(0, _T("ID"),	LVCFMT_LEFT, 40);
	m_List.InsertColumn(1, _T("モーション名"),	LVCFMT_LEFT, 300);

	Renew();

	return TRUE;
}

void CDlgAdminCharMotionTypeList::OnAdd()
{
	CPacketADMIN_CHAR_ADDMOTIONTYPE Packet;

	Packet.Make();
	SendPacket(&Packet);
}

void CDlgAdminCharMotionTypeList::OnModify()
{
	int nNo, nResult;
	DWORD dwMotionTypeID;
	PCInfoMotionType pInfo;
	CDlgAdminCharMotionList Dlg(this);
	CPacketADMIN_CHAR_RENEWMOTIONTYPE Packet;

	nNo = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwMotionTypeID = m_List.GetItemData(nNo);
	pInfo = (PCInfoMotionType)m_pLibInfoMotionType->GetPtr(dwMotionTypeID);
	if (pInfo == NULL) {
		return;
	}

	Dlg.Init(m_pMgrData, dwMotionTypeID);
	Dlg.m_strMotionName	= (LPCTSTR)pInfo->m_strName;
	Dlg.m_nGrpIDSub	= pInfo->m_wGrpIDSub;

	nResult = Dlg.DoModal();
	if (nResult != IDOK) {
		return;
	}
	pInfo->m_strName	= Dlg.m_strMotionName;
	pInfo->m_wGrpIDSub	= Dlg.m_nGrpIDSub;
	Renew();

	Packet.Make(dwMotionTypeID, m_pLibInfoMotionType);
	SendPacket(&Packet);
}

void CDlgAdminCharMotionTypeList::OnDelete()
{
}

