/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ƒtƒ@ƒCƒ‹–¼	:DlgAdminItemTypeList.cpp									 */
/* “à—e			:ƒAƒCƒeƒ€Ží•Êˆê——ƒ_ƒCƒAƒƒOƒNƒ‰ƒX ŽÀ‘•ƒtƒ@ƒCƒ‹				 */
/* ì¬			:”N‚ª‚ç”N’†t‚¤‚ç‚ç(URARA-works)							 */
/* ì¬ŠJŽn“ú	:2007/09/28													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "PacketADMIN_ITEMTYPE_ADD.h"
#include "PacketADMIN_ITEMTYPE_COPY.h"
#include "PacketADMIN_ITEMTYPE_DELETE.h"
#include "PacketITEM_REQ_ITEMTYPEINFO.h"
#include "PacketITEM_RENEWITEMTYPEINFO.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoItemType.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminItemTypeNew.h"
#include "DlgAdminItemTypeList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* ƒNƒ‰ƒX‚ÌÝ’è																 */
/* ========================================================================= */

void CDlgAdminItemTypeList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemTypeList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminItemTypeList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminItemTypeList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_RENEW, OnRenew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminItemTypeList::CDlgAdminItemTypeList					 */
/* “à—e		:ƒRƒ“ƒXƒgƒ‰ƒNƒ^													 */
/* “ú•t		:2007/09/28														 */
/* ========================================================================= */

CDlgAdminItemTypeList::CDlgAdminItemTypeList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminItemTypeList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminItemTypeList)
	//}}AFX_DATA_INIT

	m_pWndNotify	= NULL;
	m_pLibInfoItemType	= NULL;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminItemTypeList::~CDlgAdminItemTypeList					 */
/* “à—e		:ƒfƒXƒgƒ‰ƒNƒ^													 */
/* “ú•t		:2007/09/28														 */
/* ========================================================================= */

CDlgAdminItemTypeList::~CDlgAdminItemTypeList()
{
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminItemTypeList::Init									 */
/* “à—e		:‰Šú‰»															 */
/* “ú•t		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoItemType = m_pMgrData->GetLibInfoItemType ();

	/* ƒEƒBƒ“ƒhƒEì¬ */
	Create (CDlgAdminItemTypeList::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminItemTypeList::Renew									 */
/* “à—e		:ˆê——‚ðXV														 */
/* “ú•t		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::Renew(void)
{
	int i, nCount;
	PCInfoItemTypeBase pInfoItemType;
	CString strTmp;

	m_List.DeleteAllItems ();

	nCount = m_pLibInfoItemType->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (i);
		strTmp.Format ("%d", pInfoItemType->m_dwTypeID);
		m_List.InsertItem (i, strTmp);
		m_List.SetItemData (i, pInfoItemType->m_dwTypeID);		/* ID */
		m_List.SetItemText (i, 1, pInfoItemType->m_strName);	/* ƒAƒCƒeƒ€–¼ */

		strTmp = m_pLibInfoItemType->GetTypeName (pInfoItemType->m_dwItemTypeID);
		m_List.SetItemText (i, 2, strTmp);					/* Ží•Ê */
	}
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminItemTypeList::OnAdminMsg								 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_ADMINMSG)								 */
/* “ú•t		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminItemTypeList::OnInitDialog							 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_INITDIALOG)								 */
/* “ú•t		:2007/09/28														 */
/* ========================================================================= */

BOOL CDlgAdminItemTypeList::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "ID",			LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, "ƒAƒCƒeƒ€–¼",	LVCFMT_LEFT, 120);
	m_List.InsertColumn (2, "Ží•Ê",			LVCFMT_LEFT, 120);

	RegisterControl (IDC_RENEW,	LH_CTRL_X);
	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	/* ‘SƒAƒCƒeƒ€î•ñ‚ð—v‹ */
	OnRenew ();

	return TRUE;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminItemTypeList::OnMainFrame								 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_MAINFRAME)								 */
/* “ú•t		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWITEMTYPEINFO:		/* ƒAƒCƒeƒ€Ží•Êî•ñXV */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminItemTypeList::OnAdd									 */
/* “à—e		:ƒ{ƒ^ƒ“ƒnƒ“ƒhƒ‰(V‹K’Ç‰Á)										 */
/* “ú•t		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::OnAdd()
{
	int nResult;
	PCInfoItemTypeBase pInfoItem;
	CPacketADMIN_ITEMTYPE_ADD Packet;
	CDlgAdminItemTypeNew Dlg(this);

	pInfoItem = NULL;

	Dlg.Init (m_pMgrData, &m_pWndNotify);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		goto Exit;
	}

	Dlg.Get (pInfoItem);

	Packet.Make (pInfoItem);
	m_pSock->Send (&Packet);

Exit:
	SAFE_DELETE (pInfoItem);
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminItemTypeList::OnModify								 */
/* “à—e		:ƒ{ƒ^ƒ“ƒnƒ“ƒhƒ‰(•ÒW)											 */
/* “ú•t		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::OnModify()
{
	int nResult;
	DWORD dwItemID;
	CDlgAdminItemTypeNew Dlg(this);
	PCInfoItemTypeBase pInfoItem, pInfoItemTmp;
	CPacketITEM_RENEWITEMTYPEINFO Packet;

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		goto Exit;
	}
	dwItemID	= m_List.GetItemData (nResult);
	pInfoItem	= (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		goto Exit;
	}

	Dlg.Init (m_pMgrData, &m_pWndNotify);
	Dlg.SetModify (pInfoItem);

	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		goto Exit;
	}

	pInfoItemTmp = NULL;
	Dlg.Get (pInfoItemTmp);
	pInfoItem->Copy (pInfoItemTmp);
	SAFE_DELETE (pInfoItemTmp);

	Packet.Make (pInfoItem);
	m_pSock->Send (&Packet);

Exit:
	return;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminItemTypeList::OnCopy									 */
/* “à—e		:ƒ{ƒ^ƒ“ƒnƒ“ƒhƒ‰(ƒRƒs[)											 */
/* “ú•t		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::OnCopy()
{
	int nNo, nResult;
	DWORD dwItemID;
	PCInfoItemTypeBase pInfoItem;
	CPacketADMIN_ITEMTYPE_COPY Packet;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData (nNo);
	pInfoItem	= (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	nResult = MessageBox ("‘I‘ð‚³‚ê‚Ä‚¢‚éƒAƒCƒeƒ€‚ðƒRƒs[‚µ‚Ü‚·‚©H", "Šm”F", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make (dwItemID);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminItemTypeList::OnDelete								 */
/* “à—e		:ƒ{ƒ^ƒ“ƒnƒ“ƒhƒ‰(íœ)											 */
/* “ú•t		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::OnDelete()
{
	int nResult;
	DWORD dwItemID;
	PCInfoItemTypeBase pInfoItem;
	CString strTmp;
	CPacketADMIN_ITEMTYPE_DELETE Packet;

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData (nResult);
	pInfoItem	= (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	strTmp.Format ("[%s]‚ðíœ‚µ‚Ü‚·‚©H", (LPCSTR)pInfoItem->m_strName);
	nResult = MessageBox (strTmp, "Šm”F", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make (dwItemID);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminItemTypeList::OnRenew									 */
/* “à—e		:ƒ{ƒ^ƒ“ƒnƒ“ƒhƒ‰(XV)											 */
/* “ú•t		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::OnRenew()
{
	CPacketITEM_REQ_ITEMTYPEINFO Packet;

	/* ‘SƒAƒCƒeƒ€Ží•Êî•ñ‚ð—v‹ */
	Packet.Make (0);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminItemTypeList::GetIndex								 */
/* “à—e		:ƒAƒCƒeƒ€ID‚©‚çƒCƒ“ƒfƒbƒNƒX‚ðŽæ“¾								 */
/* “ú•t		:2007/09/28														 */
/* ========================================================================= */

int CDlgAdminItemTypeList::GetIndex(DWORD dwItemID)
{
	int nRet, i, nCount;
	DWORD dwItemData;

	nRet = -1;

	nCount = m_List.GetItemCount ();
	for (i = 0; i < nCount; i ++) {
		dwItemData = m_List.GetItemData (i);
		if (dwItemData == dwItemID) {
			nRet = i;
			break;
		}
	}

	return nRet;
}

/* Copyright(C)URARA-works 2007 */
