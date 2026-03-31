/// @file DlgAdminServerInfo.cpp
/// @brief サーバー情報ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/08
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "resource.h"
#include "PacketADMIN_SERVER_SAVEINFO.h"
#include "PacketADMIN_REQ_PLAYSOUND.h"
#include "PacketADMIN_RENEW_CLIENTVERSION.h"
#include "UraraSockTCPSBO.h"
#include "SBOVersion.h"
#include "LayoutHelper.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "DlgAdminServerInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminServerInfo::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminServerInfo)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminServerInfo, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminServerInfo)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PLAYSOUND, &CDlgAdminServerInfo::OnBnClickedPlaysound)
	ON_BN_CLICKED(IDC_RENEW_CLIENTVERSION, &CDlgAdminServerInfo::OnBnClickedRenewClientversion)
END_MESSAGE_MAP()

CDlgAdminServerInfo::CDlgAdminServerInfo(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminServerInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminServerInfo)
	//}}AFX_DATA_INIT
}

CDlgAdminServerInfo::~CDlgAdminServerInfo()
{
}

void CDlgAdminServerInfo::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init(pMgrData);

	// ウィンドウ作成
	Create(CDlgAdminServerInfo::IDD, m_pWndParent);
	ShowWindow(SW_SHOW);
}

void CDlgAdminServerInfo::OnSave()
{
	CPacketADMIN_SERVER_SAVEINFO Packet;

	Packet.Make();
	SendPacket(&Packet);
}

void CDlgAdminServerInfo::OnBnClickedPlaysound()
{
	CPacketADMIN_REQ_PLAYSOUND Packet;

	Packet.Make(SOUNDID_PINPONPANPON);
	SendPacket(&Packet);
}

void CDlgAdminServerInfo::OnBnClickedRenewClientversion()
{
	CPacketADMIN_RENEW_CLIENTVERSION Packet;

	Packet.Make(VERTEXT);
	SendPacket(&Packet);
}

