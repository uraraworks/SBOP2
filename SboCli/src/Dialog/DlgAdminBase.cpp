/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ƒtƒ@ƒCƒ‹–¼	:DlgAdminBase.h												 */
/* “à—e			:ŠÇ—ŽÒƒEƒBƒ“ƒhƒEŠî’êƒ_ƒCƒAƒƒOƒNƒ‰ƒX ŽÀ‘•ƒtƒ@ƒCƒ‹			 */
/* ì¬			:”N‚ª‚ç”N’†t‚¤‚ç‚ç(URARA-works)							 */
/* ì¬ŠJŽn“ú	:2007/03/18													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "MgrData.h"
#include "DlgAdminBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* ƒNƒ‰ƒX‚ÌÝ’è																 */
/* ========================================================================= */

void CDlgAdminBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminBase)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminBase, CDialog)
	//{{AFX_MSG_MAP(CDlgAdminBase)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminBase::CDlgAdminBase									 */
/* “à—e		:ƒRƒ“ƒXƒgƒ‰ƒNƒ^													 */
/* “ú•t		:2007/03/18														 */
/* ========================================================================= */

CDlgAdminBase::CDlgAdminBase(int nResourceID, CWnd* pParent /*=NULL*/)
	: CDialog(nResourceID, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminBase)
	//}}AFX_DATA_INIT

	m_pMgrData		= NULL;
	m_pWndParent	= pParent;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminBase::~CDlgAdminBase									 */
/* “à—e		:ƒfƒXƒgƒ‰ƒNƒ^													 */
/* “ú•t		:2007/03/18														 */
/* ========================================================================= */

CDlgAdminBase::~CDlgAdminBase()
{
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminBase::Init											 */
/* “à—e		:‰Šú‰»															 */
/* “ú•t		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::Init(CMgrData *pMgrData)
{
	m_pMgrData	= pMgrData;
	m_pSock		= m_pMgrData->GetUraraSockTCP ();
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminBase::Renew											 */
/* “à—e		:XV															 */
/* “ú•t		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::Renew(void)
{
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminBase::OnAdminMsg										 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_ADMINMSG)								 */
/* “ú•t		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_ACCOUNTINFO:		/* ƒAƒJƒEƒ“ƒgî•ñŽæ“¾ */
		m_pMgrData->Delete (dwPara);
		break;
	}
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminBase::OnMainFrame										 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_MAINFRAME)								 */
/* “ú•t		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminBase::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminBase::OnInitDialog									 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_INITDIALOG)								 */
/* “ú•t		:2007/03/18														 */
/* ========================================================================= */

BOOL CDlgAdminBase::OnInitDialog()
{
	CDialog::OnInitDialog ();

	CLayoutHelper::Initialize (m_hWnd);

	return TRUE;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminBase::OnSize											 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_SIZE)									 */
/* “ú•t		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize (nType, cx, cy);

	DoLayout (cx, cy);
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminBase::PostNcDestroy									 */
/* “à—e		:I—¹ˆ—														 */
/* “ú•t		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::PostNcDestroy()
{
	delete this;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminBase::SelectCmb										 */
/* “à—e		:€–Ú‚ð‘I‘ð														 */
/* “ú•t		:2007/11/25														 */
/* ========================================================================= */

void CDlgAdminBase::SelectCmb(CComboBox *pCmb, DWORD dwID)
{
	int i, nCount, nNo;
	DWORD dwItemData;

	nNo = 0;

	nCount = pCmb->GetCount ();
	for (i = 0; i < nCount; i ++) {
		dwItemData = pCmb->GetItemData (i);
		if (dwItemData != dwID) {
			continue;
		}
		nNo = i;
		break;
	}
	pCmb->SetCurSel (nNo);
}

/* Copyright(C)URARA-works 2007 */
