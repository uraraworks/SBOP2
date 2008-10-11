/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminServerInfo.cpp										 */
/* 内容			:サーバー情報ダイアログクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/08													 */
/* ========================================================================= */

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

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

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


/* ========================================================================= */
/* 関数名	:CDlgAdminServerInfo::CDlgAdminServerInfo						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/08														 */
/* ========================================================================= */

CDlgAdminServerInfo::CDlgAdminServerInfo(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminServerInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminServerInfo)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* 関数名	:CDlgAdminServerInfo::~CDlgAdminServerInfo						 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/08														 */
/* ========================================================================= */

CDlgAdminServerInfo::~CDlgAdminServerInfo()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminServerInfo::Init										 */
/* 内容		:初期化															 */
/* 日付		:2007/07/08														 */
/* ========================================================================= */

void CDlgAdminServerInfo::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* ウィンドウ作成 */
	Create (CDlgAdminServerInfo::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminServerInfo::OnSave									 */
/* 内容		:ボタンハンドラ(サーバー情報保存)								 */
/* 日付		:2007/07/08														 */
/* ========================================================================= */

void CDlgAdminServerInfo::OnSave()
{
	CPacketADMIN_SERVER_SAVEINFO Packet;

	Packet.Make ();
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminServerInfo::OnBnClickedPlaysound						 */
/* 内容		:ボタンハンドラ(放送前のお知らせ音)								 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

void CDlgAdminServerInfo::OnBnClickedPlaysound()
{
	CPacketADMIN_REQ_PLAYSOUND Packet;

	Packet.Make (SOUNDID_PINPONPANPON);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminServerInfo::OnBnClickedRenewClientversion				 */
/* 内容		:ボタンハンドラ(クライアントバージョン更新)						 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CDlgAdminServerInfo::OnBnClickedRenewClientversion()
{
	CPacketADMIN_RENEW_CLIENTVERSION Packet;

	Packet.Make (VERTEXT);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2007 */
