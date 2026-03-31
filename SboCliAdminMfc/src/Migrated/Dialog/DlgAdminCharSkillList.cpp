/// @file DlgAdminCharSkillList.cpp
/// @brief スキルリストダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/07
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_SKILL_RENEWSKILL.h"
#include "LibInfoSkill.h"
#include "MgrData.h"
#include "DlgAdminCharSkillBase.h"
#include "DlgAdminCharSkillList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminCharSkillList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharSkillList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharSkillList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharSkillList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgAdminCharSkillList::CDlgAdminCharSkillList(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminCharSkillList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharSkillList)
	//}}AFX_DATA_INIT

	m_pLibInfoSkill = NULL;
}

CDlgAdminCharSkillList::~CDlgAdminCharSkillList()
{
}

void CDlgAdminCharSkillList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init(pMgrData);

	m_pLibInfoSkill = m_pMgrData->GetLibInfoSkill();

	// ウィンドウ作成
	Create(CDlgAdminCharSkillList::IDD, m_pWndParent);
	ShowWindow(SW_SHOW);
}

void CDlgAdminCharSkillList::Renew(void)
{
	int i, nCount;
	PCInfoSkillBase pInfo;
	CmyString strTmp;

	m_List.DeleteAllItems();

	nCount = m_pLibInfoSkill->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoSkillBase)m_pLibInfoSkill->GetPtr(i);

		strTmp.Format(_T("%d"), pInfo->m_dwSkillID);
		m_List.InsertItem(i, strTmp);
		CString strName = (LPCTSTR)pInfo->m_strName;
		m_List.SetItemText(i, 1, strName);
		m_List.SetItemData(i, pInfo->m_dwSkillID);
	}
}

void CDlgAdminCharSkillList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWSKILLINFO:	// スキル情報更新
		Renew();
		break;
	}
}

BOOL CDlgAdminCharSkillList::OnInitDialog()
{
	CmyString strTmp;

	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn(0, _T("ID"),	LVCFMT_LEFT, 40);
	m_List.InsertColumn(1, _T("スキル名"),	LVCFMT_LEFT, 300);

	Renew();

	return TRUE;
}

void CDlgAdminCharSkillList::OnAdd()
{
	int nResult;
	PCInfoSkillBase pInfo;
	CPacketADMIN_SKILL_RENEWSKILL Packet;
	CDlgAdminCharSkillBase Dlg(this);

	pInfo = NULL;

	Dlg.Init(m_pMgrData);
	nResult = Dlg.DoModal();
	if (nResult != IDOK) {
		goto Exit;
	}

	Dlg.Get(pInfo);

	Packet.Make(pInfo);
	SendPacket(&Packet);

Exit:
	SAFE_DELETE(pInfo);
}

void CDlgAdminCharSkillList::OnModify()
{
	int nResult;
	DWORD dwSkillID;
	CDlgAdminCharSkillBase Dlg(this);
	PCInfoSkillBase pInfo, pInfoTmp;
	CPacketADMIN_SKILL_RENEWSKILL Packet;

	nResult = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nResult < 0) {
		goto Exit;
	}
	dwSkillID	= m_List.GetItemData(nResult);
	pInfo	= (PCInfoSkillBase)m_pLibInfoSkill->GetPtr(dwSkillID);
	if (pInfo == NULL) {
		goto Exit;
	}

	Dlg.Init(m_pMgrData);
	Dlg.SetModify(pInfo);

	nResult = Dlg.DoModal();
	if (nResult != IDOK) {
		goto Exit;
	}

	pInfoTmp = NULL;
	Dlg.Get(pInfoTmp);
	pInfo = m_pLibInfoSkill->Renew(pInfoTmp);
	SAFE_DELETE(pInfoTmp);

	Packet.Make(pInfo);
	SendPacket(&Packet);

Exit:
	return;
}

void CDlgAdminCharSkillList::OnDelete()
{
}

