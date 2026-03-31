/// @file DlgAdminCharAccountInfo.cpp
/// @brief アカウント情報ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/06
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "InfoAccount.h"
#include "UraraSockTCPSBO.h"
#include "Packet.h"
#include "myString.h"
#include "LayoutHelper.h"
#include "InfoCharCli.h"
#include "LibInfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharAccountInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminCharAccountInfo::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharAccountInfo)
	DDX_Text(pDX, IDC_CHARNAME, m_strCharName);
	DDX_Text(pDX, IDC_ACCOUNTID, m_strAccountID);
	DDX_Text(pDX, IDC_ACCOUNT, m_strAccount);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_IP, m_strIP);
	DDX_Text(pDX, IDC_MAC, m_strMac);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharAccountInfo, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharAccountInfo)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_DISABLE, OnDisable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgAdminCharAccountInfo::CDlgAdminCharAccountInfo(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminCharAccountInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharAccountInfo)
	m_strCharName = _T("");
	m_strAccountID = _T("");
	m_strAccount = _T("");
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
}

CDlgAdminCharAccountInfo::~CDlgAdminCharAccountInfo()
{
}

void CDlgAdminCharAccountInfo::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init(pMgrData);

	// ウィンドウ作成
	Create(CDlgAdminCharAccountInfo::IDD, m_pWndParent);
	ShowWindow(SW_SHOW);
}

void CDlgAdminCharAccountInfo::Renew(void)
{
}

void CDlgAdminCharAccountInfo::OnAdminMsg(int nType, DWORD dwPara)
{
	PCLibInfoCharCli pLibInfoChar;

	pLibInfoChar = m_pMgrData->GetLibInfoChar();

	switch (nType) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:	// 左クリック通知
		switch (m_pMgrData->GetAdminNotifyTypeL()) {
		case ADMINNOTIFYTYPE_CHARID:	// キャラID
			{
				PCInfoCharCli pInfoChar;
				CPacketADMIN_CHAR_REQ_ACCOUNT Packet;

				pInfoChar = (PCInfoCharCli)pLibInfoChar->GetPtr(dwPara);
				m_dwAccountID = 0;
				m_strCharName.	Empty();
				m_strAccount.	Empty();
				m_strPassword.	Empty();
				m_strAccountID.	Empty();
				m_strIP.	Empty();
				m_strMac.	Empty();
				if (pInfoChar) {
					m_dwAccountID = pInfoChar->m_dwAccountID;
					m_strCharName = (LPCTSTR)pInfoChar->m_strCharName;
					m_strAccountID.Format(_T("%d"), m_dwAccountID);
					Packet.Make(m_dwAccountID);
					SendPacket(&Packet);
				}
				UpdateData(FALSE);
			}
			break;
		}
		break;
	case ADMINMSG_ACCOUNTINFO:	// アカウント情報取得
		{
			PBYTE pData;
			CInfoAccount InfoAccount;
			IN_ADDR AddrTmp;

			pData = m_pMgrData->GetPtr(dwPara);
			InfoAccount.SetTmpData(pData);
			m_dwAccountID	= InfoAccount.m_dwAccountID;
			m_strAccount	= (LPCTSTR)InfoAccount.m_strAccount;
			m_strPassword	= (LPCTSTR)InfoAccount.m_strPassword;
			AddrTmp.S_un.S_addr = InfoAccount.m_dwIP;
			m_strIP.Format(_T("%d.%d.%d.%d"),
					AddrTmp.S_un.S_un_b.s_b1, AddrTmp.S_un.S_un_b.s_b2, AddrTmp.S_un.S_un_b.s_b3, AddrTmp.S_un.S_un_b.s_b4);
			m_strMac	= (LPCTSTR)InfoAccount.m_strLastMacAddr;
			m_strAccountID.Format(_T("%d"), m_dwAccountID);
			m_pMgrData->Delete(dwPara);
			UpdateData(FALSE);
		}
		break;
	}
}

BOOL CDlgAdminCharAccountInfo::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}

void CDlgAdminCharAccountInfo::OnSend()
{
	int nResult;
	CPacketADMIN_CHAR_RENEW_ACCOUNT Packet;
	CInfoAccount InfoAccount;

	if (m_dwAccountID == 0) {
		return;
	}
        nResult = MessageBox(_T("アカウント情報を送信しますか？"), _T("確認"), MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	UpdateData();

	InfoAccount.m_dwAccountID	= m_dwAccountID;
	InfoAccount.m_strAccount	= m_strAccount;
	InfoAccount.m_strPassword	= m_strPassword;
	Packet.Make(&InfoAccount);
	SendPacket(&Packet);
}

void CDlgAdminCharAccountInfo::OnDisable()
{
	int nResult;
	CPacketADMIN_CHAR_RENEW_ACCOUNT Packet;
	CInfoAccount InfoAccount;

        nResult = MessageBox(_T("ログイン拒否しますか？"), _T("確認"), MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	UpdateData();

	InfoAccount.m_dwAccountID	= m_dwAccountID;
	InfoAccount.m_strAccount	= m_strAccount;
	InfoAccount.m_strPassword	= m_strPassword;
	CStringA strMacA = TStringToUtf8(m_strMac);

	Packet.Make(&InfoAccount, TRUE, strMacA);
	SendPacket(&Packet);
}

