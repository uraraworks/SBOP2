/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ƒtƒ@ƒCƒ‹–¼	:DlgAdminCharSkillList.cpp									 */
/* “à—e			:ƒXƒLƒ‹ƒŠƒXƒgƒ_ƒCƒAƒƒOƒNƒ‰ƒX ŽÀ‘•ƒtƒ@ƒCƒ‹					 */
/* ì¬			:”N‚ª‚ç”N’†t‚¤‚ç‚ç(URARA-works)							 */
/* ì¬ŠJŽn“ú	:2008/12/07													 */
/* ========================================================================= */

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

/* ========================================================================= */
/* ƒNƒ‰ƒX‚ÌÝ’è																 */
/* ========================================================================= */

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


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminCharSkillList::CDlgAdminCharSkillList					 */
/* “à—e		:ƒRƒ“ƒXƒgƒ‰ƒNƒ^													 */
/* “ú•t		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminCharSkillList::CDlgAdminCharSkillList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharSkillList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharSkillList)
	//}}AFX_DATA_INIT

	m_pLibInfoSkill = NULL;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminCharSkillList::~CDlgAdminCharSkillList				 */
/* “à—e		:ƒfƒXƒgƒ‰ƒNƒ^													 */
/* “ú•t		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminCharSkillList::~CDlgAdminCharSkillList()
{
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminCharSkillList::Init									 */
/* “à—e		:‰Šú‰»															 */
/* “ú•t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoSkill = m_pMgrData->GetLibInfoSkill ();

	/* ƒEƒBƒ“ƒhƒEì¬ */
	Create (CDlgAdminCharSkillList::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminCharSkillList::Renew									 */
/* “à—e		:ƒŠƒXƒgXV														 */
/* “ú•t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::Renew(void)
{
	int i, nCount;
	PCInfoSkillBase pInfo;
	CmyString strTmp;

	m_List.DeleteAllItems ();

	nCount = m_pLibInfoSkill->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (i);

		strTmp.Format ("%d", pInfo->m_dwSkillID);
		m_List.InsertItem (i, strTmp);
		m_List.SetItemText (i, 1, pInfo->m_strName);
		m_List.SetItemData (i, pInfo->m_dwSkillID);
	}
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminCharSkillList::OnMainFrame							 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_MAINFRAME)								 */
/* “ú•t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWSKILLINFO:		/* ƒXƒLƒ‹î•ñXV */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminCharSkillList::OnInitDialog							 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_INITDIALOG)								 */
/* “ú•t		:2008/12/07														 */
/* ========================================================================= */

BOOL CDlgAdminCharSkillList::OnInitDialog()
{
	CmyString strTmp;

	CDlgAdminBase::OnInitDialog ();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "ID",		LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, "ƒXƒLƒ‹–¼",	LVCFMT_LEFT, 300);

	Renew ();

	return TRUE;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminCharSkillList::OnAdd									 */
/* “à—e		:ƒ{ƒ^ƒ“ƒnƒ“ƒhƒ‰(’Ç‰Á)											 */
/* “ú•t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::OnAdd()
{
	int nResult;
	PCInfoSkillBase pInfo;
	CPacketADMIN_SKILL_RENEWSKILL Packet;
	CDlgAdminCharSkillBase Dlg(this);

	pInfo = NULL;

	Dlg.Init (m_pMgrData);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		goto Exit;
	}

	Dlg.Get (pInfo);

	Packet.Make (pInfo);
	m_pSock->Send (&Packet);

Exit:
	SAFE_DELETE (pInfo);
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminCharSkillList::OnModify								 */
/* “à—e		:ƒ{ƒ^ƒ“ƒnƒ“ƒhƒ‰(•ÒW)											 */
/* “ú•t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::OnModify()
{
	int nResult;
	DWORD dwSkillID;
	CDlgAdminCharSkillBase Dlg(this);
	PCInfoSkillBase pInfo, pInfoTmp;
	CPacketADMIN_SKILL_RENEWSKILL Packet;

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		goto Exit;
	}
	dwSkillID	= m_List.GetItemData (nResult);
	pInfo		= (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (dwSkillID);
	if (pInfo == NULL) {
		goto Exit;
	}

	Dlg.Init (m_pMgrData);
	Dlg.SetModify (pInfo);

	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		goto Exit;
	}

	pInfoTmp = NULL;
	Dlg.Get (pInfoTmp);
	pInfo = m_pLibInfoSkill->Renew (pInfoTmp);
	SAFE_DELETE (pInfoTmp);

	Packet.Make (pInfo);
	m_pSock->Send (&Packet);

Exit:
	return;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminCharSkillList::OnDelete								 */
/* “à—e		:ƒ{ƒ^ƒ“ƒnƒ“ƒhƒ‰(íœ)											 */
/* “ú•t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::OnDelete()
{
}

/* Copyright(C)URARA-works 2008 */
