/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharModifyStatus.cpp								 */
/* 内容			:選択キャラ情報編集[ステータス]ダイアログクラス 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_CHAR_RENEWSTATUS.h"
#include "PacketADMIN_PARA2.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharModifyStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminCharModifyStatus::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharModifyStatus)
	DDX_Text(pDX, IDC_STATIC_CHARNAME, m_strCharName);
	DDX_Text(pDX, IDC_STATIC_CHARID, m_strCharID);
	DDX_Text(pDX, IDC_MOVEWAIT, m_dwMoveWait);
	DDX_Text(pDX, IDC_MOVEWAITBATTLE, m_dwMoveWaitBattle);
	DDX_Text(pDX, IDC_HP, m_dwHP);
	DDX_Text(pDX, IDC_MAXHP, m_dwMaxHP);
	DDX_Text(pDX, IDC_SP, m_dwSP);
	DDX_Text(pDX, IDC_MAXSP, m_dwMaxSP);
	DDX_Text(pDX, IDC_LIGHTLEVEL, m_nLightLevel);
	DDX_Text(pDX, IDC_LIGHTTIME, m_dwLightTime);
	DDX_Text(pDX, IDC_DROPITEMAVERAGE, m_nDropItemAverage);
	DDX_Text(pDX, IDC_SEARCH_X, m_nSearchX);
	DDX_Text(pDX, IDC_SEARCH_Y, m_nSearchY);
	DDX_Text(pDX, IDC_MOVEAVERAGE, m_nMoveAverage);
	DDX_Text(pDX, IDC_MOVEAVERAGEBATTLE, m_nMoveAverageBattle);
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

BEGIN_MESSAGE_MAP(CDlgAdminCharModifyStatus, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharModifyStatus)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyStatus::CDlgAdminCharModifyStatus			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CDlgAdminCharModifyStatus::CDlgAdminCharModifyStatus(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharModifyStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharModifyStatus)
	m_strCharName = _T("");
	m_strCharID = _T("");
	m_dwMoveWait = 0;
	m_dwMoveWaitBattle = 0;
	m_dwHP = 0;
	m_dwMaxHP = 0;
	m_dwSP = 0;
	m_dwMaxSP = 0;
	m_nLightLevel = 0;
	m_dwLightTime = 0;
	m_nDropItemAverage = 0;
	m_nSearchX = 0;
	m_nSearchY = 0;
	m_nMoveAverage = 0;
	m_nMoveAverageBattle = 0;
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

	m_pInfoChar = NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyStatus::~CDlgAdminCharModifyStatus			 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CDlgAdminCharModifyStatus::~CDlgAdminCharModifyStatus()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyStatus::Init								 */
/* 内容		:初期化															 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyStatus::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* ウィンドウ作成 */
	Create (CDlgAdminCharModifyStatus::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyStatus::Renew								 */
/* 内容		:更新															 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyStatus::Renew(void)
{
	m_strCharID.	Empty ();
	m_strCharName.	Empty ();
	m_dwMoveWait			= 0;
	m_dwMoveWaitBattle		= 0;
	m_dwHP					= 0;
	m_dwMaxHP				= 0;
	m_dwSP					= 0;
	m_dwMaxSP				= 0;
	m_nLightLevel			= 0;
	m_dwLightTime			= 0;
	m_nDropItemAverage		= 0;
	m_nSearchX				= 0;
	m_nSearchY				= 0;
	m_nMoveAverage			= 0;
	m_nMoveAverageBattle	= 0;
	m_dwStamina				= 0;
	m_dwPower				= 0;
	m_dwStrength			= 0;
	m_dwMagic				= 0;
	m_dwSkillful			= 0;
	m_dwAbillityAT			= 0;
	m_dwAbillityDF			= 0;
	m_dwPAtack				= 0;
	m_dwPDefense			= 0;
	m_dwPMagic				= 0;
	m_dwPMagicDefense		= 0;
	m_dwPHitAverage			= 0;
	m_dwPAvoidAverage		= 0;
	m_dwPCriticalAverage	= 0;
	m_dwAttrFire			= 0;
	m_dwAttrWind			= 0;
	m_dwAttrWater			= 0;
	m_dwAttrEarth			= 0;
	m_dwAttrLight			= 0;
	m_dwAttrDark			= 0;

	if (m_pInfoChar == NULL) {
		goto Exit;
	}

	m_strCharID.Format ("%u", m_pInfoChar->m_dwCharID);
	m_strCharName			= m_pInfoChar->m_strCharName;
	m_dwMoveWait			= m_pInfoChar->m_dwMoveWait;
	m_dwMoveWaitBattle		= m_pInfoChar->m_dwMoveWaitBattle;
	m_dwHP					= m_pInfoChar->m_dwHP;
	m_dwMaxHP				= m_pInfoChar->m_dwMaxHP;
	m_dwSP					= m_pInfoChar->m_dwSP;
	m_dwMaxSP				= m_pInfoChar->m_dwMaxSP;
	m_nLightLevel			= m_pInfoChar->m_nLightLevel;		/* 灯りレベル */
	m_nDropItemAverage		= m_pInfoChar->m_nDropItemAverage;
	m_nSearchX				= m_pInfoChar->m_sizeSearchDistance.cx;	/* 策敵範囲(X) */
	m_nSearchY				= m_pInfoChar->m_sizeSearchDistance.cy;	/* 策敵範囲(Y) */
	m_nMoveAverage			= m_pInfoChar->m_nMoveAverage;
	m_nMoveAverageBattle	= m_pInfoChar->m_nMoveAverageBattle;
	m_dwStamina				= m_pInfoChar->m_wStamina;
	m_dwPower				= m_pInfoChar->m_wPower;
	m_dwStrength			= m_pInfoChar->m_wStrength;
	m_dwMagic				= m_pInfoChar->m_wMagic;
	m_dwSkillful			= m_pInfoChar->m_wSkillful;
	m_dwAbillityAT			= m_pInfoChar->m_wAbillityAT;
	m_dwAbillityDF			= m_pInfoChar->m_wAbillityDF;
	m_dwPAtack				= m_pInfoChar->m_wPAtack;
	m_dwPDefense			= m_pInfoChar->m_wPDefense;
	m_dwPMagic				= m_pInfoChar->m_wPMagic;
	m_dwPMagicDefense		= m_pInfoChar->m_wPMagicDefense;
	m_dwPHitAverage			= m_pInfoChar->m_wPHitAverage;
	m_dwPAvoidAverage		= m_pInfoChar->m_wPAvoidAverage;
	m_dwPCriticalAverage	= m_pInfoChar->m_wPCriticalAverage;
	m_dwAttrFire			= m_pInfoChar->m_wAttrFire;
	m_dwAttrWind			= m_pInfoChar->m_wAttrWind;
	m_dwAttrWater			= m_pInfoChar->m_wAttrWater;
	m_dwAttrEarth			= m_pInfoChar->m_wAttrEarth;
	m_dwAttrLight			= m_pInfoChar->m_wAttrLight;
	m_dwAttrDark			= m_pInfoChar->m_wAttrDark;

Exit:
	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyStatus::OnAdminMsg							 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminCharModifyStatus::OnAdminMsg(int nType, DWORD dwPara)
{
	PCLibInfoCharCli pLibInfoChar;
	CPacketADMIN_PARA2 Packet;

	pLibInfoChar	= m_pMgrData->GetLibInfoChar ();
	m_pInfoChar		= (PCInfoCharCli)pLibInfoChar->GetPtr (dwPara);

	if (m_pInfoChar == NULL) {
		Renew ();
		return;
	}

	Packet.Make (SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_STATUS, m_pInfoChar->m_dwCharID, 0);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyStatus::OnMainFrame							 */
/* 内容		:メッセージハンドラ(WM_MAINFRAME)								 */
/* 日付		:2008/07/22														 */
/* ========================================================================= */

void CDlgAdminCharModifyStatus::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWSTATUS:		/* ステータス情報更新 */
		if (m_pInfoChar == NULL) {
			break;
		}
		if (m_pInfoChar->m_dwCharID != dwParam) {
			break;
		}
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyStatus::OnInitDialog						 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

BOOL CDlgAdminCharModifyStatus::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog ();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyStatus::OnSend								 */
/* 内容		:ボタンハンドラ(送信)											 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyStatus::OnSend()
{
	CInfoCharCli InfoCharTmp;
	CPacketADMIN_CHAR_RENEWSTATUS Packet;

	if (m_pInfoChar == NULL) {
		return;
	}
	UpdateData ();

	InfoCharTmp.Create (m_pMgrData);
	InfoCharTmp.Copy (m_pInfoChar);
	InfoCharTmp.m_dwMoveWait			= m_dwMoveWait;
	InfoCharTmp.m_dwMoveWaitBattle		= m_dwMoveWaitBattle;
	InfoCharTmp.m_dwHP					= m_dwHP;
	InfoCharTmp.m_dwMaxHP				= m_dwMaxHP;
	InfoCharTmp.m_dwSP					= m_dwSP;
	InfoCharTmp.m_dwMaxSP				= m_dwMaxSP;
	InfoCharTmp.m_nLightLevel			= m_nLightLevel;
	InfoCharTmp.m_dwLightTime			= m_dwLightTime;
	InfoCharTmp.m_nDropItemAverage		= m_nDropItemAverage;
	InfoCharTmp.m_sizeSearchDistance.cx	= m_nSearchX;					/* 策敵範囲(X) */
	InfoCharTmp.m_sizeSearchDistance.cy	= m_nSearchY;					/* 策敵範囲(Y) */
	InfoCharTmp.m_nMoveAverage			= m_nMoveAverage;
	InfoCharTmp.m_nMoveAverageBattle	= m_nMoveAverageBattle;
	InfoCharTmp.m_wStamina				= (WORD)m_dwStamina;
	InfoCharTmp.m_wPower				= (WORD)m_dwPower;
	InfoCharTmp.m_wStrength				= (WORD)m_dwStrength;
	InfoCharTmp.m_wMagic				= (WORD)m_dwMagic;
	InfoCharTmp.m_wSkillful				= (WORD)m_dwSkillful;
	InfoCharTmp.m_wAbillityAT			= (WORD)m_dwAbillityAT;
	InfoCharTmp.m_wAbillityDF			= (WORD)m_dwAbillityDF;
	InfoCharTmp.m_wPAtack				= (WORD)m_dwPAtack;
	InfoCharTmp.m_wPDefense				= (WORD)m_dwPDefense;
	InfoCharTmp.m_wPMagic				= (WORD)m_dwPMagic;
	InfoCharTmp.m_wPMagicDefense		= (WORD)m_dwPMagicDefense;
	InfoCharTmp.m_wPHitAverage			= (WORD)m_dwPHitAverage;
	InfoCharTmp.m_wPAvoidAverage		= (WORD)m_dwPAvoidAverage;
	InfoCharTmp.m_wPCriticalAverage		= (WORD)m_dwPCriticalAverage;
	InfoCharTmp.m_wAttrFire				= (WORD)m_dwAttrFire;
	InfoCharTmp.m_wAttrWind				= (WORD)m_dwAttrWind;
	InfoCharTmp.m_wAttrWater			= (WORD)m_dwAttrWater;
	InfoCharTmp.m_wAttrEarth			= (WORD)m_dwAttrEarth;
	InfoCharTmp.m_wAttrLight			= (WORD)m_dwAttrLight;
	InfoCharTmp.m_wAttrDark				= (WORD)m_dwAttrDark;

	Packet.Make (&InfoCharTmp);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2008 */
