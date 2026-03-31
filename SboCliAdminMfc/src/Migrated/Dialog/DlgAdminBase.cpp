/// @file DlgAdminBase.h
/// @brief 管理者ウィンドウ基底ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/03/18
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "resource.h"
#include "MgrData.h"
#include "DlgAdminBase.h"
#include "PacketBase.h"
#include "AdminApi/AdminUiApi.h"

IMPLEMENT_DYNAMIC(CDlgAdminBase, CDialog)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

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

CDlgAdminBase::CDlgAdminBase(int nResourceID, CWnd* pParent)
	: CDialog(nResourceID, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminBase)
	//}}AFX_DATA_INIT

	m_pMgrData	= NULL;
	m_pWndParent	= pParent;
	m_pHost	= NULL;
}

CDlgAdminBase::~CDlgAdminBase()
{
}

void CDlgAdminBase::Init(CMgrData *pMgrData)
{
	m_pMgrData	= pMgrData;
}

void CDlgAdminBase::SetHost(const SboAdminUiHost* pHost)
{
	m_pHost = pHost;
}

void CDlgAdminBase::Renew(void)
{
}

void CDlgAdminBase::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_ACCOUNTINFO:	// アカウント情報取得
		m_pMgrData->Delete(dwPara);
		break;
	}
}

void CDlgAdminBase::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
}

BOOL CDlgAdminBase::OnInitDialog()
{
	CDialog::OnInitDialog();

	CLayoutHelper::Initialize(m_hWnd);

	return TRUE;
}

void CDlgAdminBase::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	DoLayout(cx, cy);
}

void CDlgAdminBase::PostNcDestroy()
{
	delete this;
}

void CDlgAdminBase::SelectCmb(CComboBox *pCmb, DWORD dwID)
{
	int i, nCount, nNo;
	DWORD dwItemData;

	nNo = 0;

	nCount = pCmb->GetCount();
	for (i = 0; i < nCount; i ++) {
		dwItemData = pCmb->GetItemData(i);
		if (dwItemData != dwID) {
			continue;
		}
		nNo = i;
		break;
	}
	pCmb->SetCurSel(nNo);
}

void CDlgAdminBase::SendPacket(CPacketBase* pPacket)
{
	if (pPacket == NULL) {
		return;
	}
	if ((m_pHost) && (m_pHost->SendAdminPacket)) {
		m_pHost->SendAdminPacket(m_pHost->userData, pPacket);
	}
}

