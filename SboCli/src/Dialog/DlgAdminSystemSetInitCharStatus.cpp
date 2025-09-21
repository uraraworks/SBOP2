/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminSystemSetInitCharStatus.cpp						 */
/* 内容			:キャラステータス初期値の設定ダイアログクラス 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/22													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoSystem.h"
#include "Command.h"
#include "PacketADMIN_PARA2.h"
#include "PacketADMIN_SYSTEM_RENEWINFO.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminSystemSetInitCharStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminSystemSetInitCharStatus::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminSystemSetInitCharStatus)
	DDX_Text(pDX, IDC_MAPID, m_dwMapID);
	DDX_Text(pDX, IDC_POSX, m_nPosX);
	DDX_Text(pDX, IDC_POSY, m_nPosY);
	DDX_Text(pDX, IDC_MAXHP, m_dwMaxHP);
	DDX_Text(pDX, IDC_MAXSP, m_dwMaxSP);
	DDX_Text(pDX, IDC_STAMINA, m_dwStamina);
	DDX_Text(pDX, IDC_POWER, m_dwPower);
	DDX_Text(pDX, IDC_STRENGTH, m_dwStrength);
	DDX_Text(pDX, IDC_MAGIC, m_dwMagic);
	DDX_Text(pDX, IDC_SKILLFUL, m_dwSkillful);
	DDX_Text(pDX, IDC_ABILLITYAT, m_dwAbillityAT);
	DDX_Text(pDX, IDC_ABILLITYDF, m_dwAbillityDF);
	DDX_Text(pDX, IDC_PATACK, m_dwPAtack);
	DDX_Text(pDX, IDC_PDEFENSE, m_dwPDefense);
	DDX_Text(pDX, IDC_PMAGIC, m_dwPMagic);
	DDX_Text(pDX, IDC_PMAGICDEFENSE, m_dwPMagicDefense);
	DDX_Text(pDX, IDC_PHITAVERAGE, m_dwPHitAverage);
	DDX_Text(pDX, IDC_PAVOIDAVERAGE, m_dwPAvoidAverage);
	DDX_Text(pDX, IDC_PCRITICALAVERAGE, m_dwPCriticalAverage);
	DDX_Text(pDX, IDC_ATTR_FIRE, m_dwAttrFire);
	DDX_Text(pDX, IDC_ATTR_WIND, m_dwAttrWind);
	DDX_Text(pDX, IDC_ATTR_WATER, m_dwAttrWater);
	DDX_Text(pDX, IDC_ATTR_EARTH, m_dwAttrEarth);
	DDX_Text(pDX, IDC_ATTR_LIGHT, m_dwAttrLight);
	DDX_Text(pDX, IDC_ATTR_DARK, m_dwAttrDark);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminSystemSetInitCharStatus, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminSystemSetInitCharStatus)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminSystemSetInitCharStatus::CDlgAdminSystemSetInitCharStatus	 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/09/22														 */
/* ========================================================================= */

CDlgAdminSystemSetInitCharStatus::CDlgAdminSystemSetInitCharStatus(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminSystemSetInitCharStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminSystemSetInitCharStatus)
	m_dwMapID = 0;
	m_nPosX = 0;
	m_nPosY = 0;
	m_dwMaxHP = 0;
	m_dwMaxSP = 0;
	m_dwStamina = 0;
	m_dwPower = 0;
	m_dwStrength = 0;
	m_dwMagic = 0;
	m_dwSkillful = 0;
	m_dwAbillityAT = 0;
	m_dwAbillityDF = 0;
	m_dwPAtack = 0;
	m_dwPDefense = 0;
	m_dwPMagic = 0;
	m_dwPMagicDefense = 0;
	m_dwPHitAverage = 0;
	m_dwPAvoidAverage = 0;
	m_dwPCriticalAverage = 0;
	m_dwAttrFire = 0;
	m_dwAttrWind = 0;
	m_dwAttrWater = 0;
	m_dwAttrEarth = 0;
	m_dwAttrLight = 0;
	m_dwAttrDark = 0;
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSystemSetInitCharStatus::~CDlgAdminSystemSetInitCharStatus */
/* 内容		:デストラクタ													 */
/* 日付		:2008/09/22														 */
/* ========================================================================= */

CDlgAdminSystemSetInitCharStatus::~CDlgAdminSystemSetInitCharStatus()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSystemSetInitCharStatus::Init							 */
/* 内容		:初期化															 */
/* 日付		:2008/09/22														 */
/* ========================================================================= */

void CDlgAdminSystemSetInitCharStatus::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* ウィンドウ作成 */
	Create (CDlgAdminSystemSetInitCharStatus::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSystemSetInitCharStatus::Renew						 */
/* 内容		:更新															 */
/* 日付		:2008/09/22														 */
/* ========================================================================= */

void CDlgAdminSystemSetInitCharStatus::Renew(void)
{
	PCInfoSystem pInfoSystem;
	PSTSYSTEM_INITCHARSTATUS pInitCharStatus;

	pInfoSystem		= (PCInfoSystem)m_pMgrData->GetLibInfoSystem ()->GetPtr ();
	pInitCharStatus	= pInfoSystem->m_pInitCharStatus;

	m_dwMapID				= pInitCharStatus->dwInitPosMapID;
	m_nPosX					= pInitCharStatus->ptInitPos.x;
	m_nPosY					= pInitCharStatus->ptInitPos.y;
	m_dwMaxHP				= pInitCharStatus->dwMaxHP;
	m_dwMaxSP				= pInitCharStatus->dwMaxSP;
	m_dwStamina				= pInitCharStatus->wStamina;
	m_dwPower				= pInitCharStatus->wPower;
	m_dwStrength			= pInitCharStatus->wStrength;
	m_dwMagic				= pInitCharStatus->wMagic;
	m_dwSkillful			= pInitCharStatus->wSkillful;
	m_dwAbillityAT			= pInitCharStatus->wAbillityAT;
	m_dwAbillityDF			= pInitCharStatus->wAbillityDF;
	m_dwPAtack				= pInitCharStatus->wPAtack;
	m_dwPDefense			= pInitCharStatus->wPDefense;
	m_dwPMagic				= pInitCharStatus->wPMagic;
	m_dwPMagicDefense		= pInitCharStatus->wPMagicDefense;
	m_dwPHitAverage			= pInitCharStatus->wPHitAverage;
	m_dwPAvoidAverage		= pInitCharStatus->wPAvoidAverage;
	m_dwPCriticalAverage	= pInitCharStatus->wPCriticalAverage;
	m_dwAttrFire			= pInitCharStatus->wAttrFire;
	m_dwAttrWind			= pInitCharStatus->wAttrWind;
	m_dwAttrWater			= pInitCharStatus->wAttrWater;
	m_dwAttrEarth			= pInitCharStatus->wAttrEarth;
	m_dwAttrLight			= pInitCharStatus->wAttrLight;
	m_dwAttrDark			= pInitCharStatus->wAttrDark;

	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSystemSetInitCharStatus::OnAdminMsg					 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/09/22														 */
/* ========================================================================= */

void CDlgAdminSystemSetInitCharStatus::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSystemSetInitCharStatus::OnMainFrame					 */
/* 内容		:メッセージハンドラ(WM_MAINFRAME)								 */
/* 日付		:2008/09/22														 */
/* ========================================================================= */

void CDlgAdminSystemSetInitCharStatus::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWSYSTEMINFO:		/* システム情報更新 */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSystemSetInitCharStatus::OnInitDialog					 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/09/22														 */
/* ========================================================================= */

BOOL CDlgAdminSystemSetInitCharStatus::OnInitDialog()
{
	CPacketADMIN_PARA2 Packet;

	CDlgAdminBase::OnInitDialog ();

	Packet.Make (SBOCOMMANDID_SUB_ADMIN_SYSTEM_REQ_INFO);
	m_pSock->Send (&Packet);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSystemSetInitCharStatus::OnSend						 */
/* 内容		:ボタンハンドラ(送信)											 */
/* 日付		:2008/09/22														 */
/* ========================================================================= */

void CDlgAdminSystemSetInitCharStatus::OnSend()
{
	CInfoSystem InfoSystemTmp, *pInfoSystem;
	PSTSYSTEM_INITCHARSTATUS pInitCharStatus;
	CPacketADMIN_SYSTEM_RENEWINFO Packet;

	UpdateData ();

	pInfoSystem = (PCInfoSystem)m_pMgrData->GetLibInfoSystem ()->GetPtr ();

	InfoSystemTmp.Copy (pInfoSystem);
	pInitCharStatus	= InfoSystemTmp.m_pInitCharStatus;

	pInitCharStatus->dwInitPosMapID		= m_dwMapID;
	pInitCharStatus->ptInitPos.x		= m_nPosX;
	pInitCharStatus->ptInitPos.y		= m_nPosY;
	pInitCharStatus->dwMaxHP			= m_dwMaxHP;
	pInitCharStatus->dwMaxSP			= m_dwMaxSP;
	pInitCharStatus->wStamina			= (WORD)m_dwStamina;
	pInitCharStatus->wPower				= (WORD)m_dwPower;
	pInitCharStatus->wStrength			= (WORD)m_dwStrength;
	pInitCharStatus->wMagic				= (WORD)m_dwMagic;
	pInitCharStatus->wSkillful			= (WORD)m_dwSkillful;
	pInitCharStatus->wAbillityAT		= (WORD)m_dwAbillityAT;
	pInitCharStatus->wAbillityDF		= (WORD)m_dwAbillityDF;
	pInitCharStatus->wPAtack			= (WORD)m_dwPAtack;
	pInitCharStatus->wPDefense			= (WORD)m_dwPDefense;
	pInitCharStatus->wPMagic			= (WORD)m_dwPMagic;
	pInitCharStatus->wPMagicDefense		= (WORD)m_dwPMagicDefense;
	pInitCharStatus->wPHitAverage		= (WORD)m_dwPHitAverage;
	pInitCharStatus->wPAvoidAverage		= (WORD)m_dwPAvoidAverage;
	pInitCharStatus->wPCriticalAverage	= (WORD)m_dwPCriticalAverage;
	pInitCharStatus->wAttrFire			= (WORD)m_dwAttrFire;
	pInitCharStatus->wAttrWind			= (WORD)m_dwAttrWind;
	pInitCharStatus->wAttrWater			= (WORD)m_dwAttrWater;
	pInitCharStatus->wAttrEarth			= (WORD)m_dwAttrEarth;
	pInitCharStatus->wAttrLight			= (WORD)m_dwAttrLight;
	pInitCharStatus->wAttrDark			= (WORD)m_dwAttrDark;

	Packet.Make (&InfoSystemTmp);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2008 */
