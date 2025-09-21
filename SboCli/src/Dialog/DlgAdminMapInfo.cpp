﻿/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapInfo.cpp										 */
/* 内容			:マップ情報編集ダイアログクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "InfoMapBase.h"
#include "MgrSound.h"
#include "PacketADMIN_MAP_RENEWMAPSIZE.h"
#include "PacketADMIN_MAP_SETMAPNAME.h"
#include "PacketADMIN_PARA2.h"
#include "Command.h"
#include "MgrData.h"
#include "DlgAdminMapInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminMapInfo::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapInfo)
	DDX_Text(pDX, IDC_MAPSIZE, m_strMapSize);
	DDX_Text(pDX, IDC_VALUE, m_nValue);
	DDX_Text(pDX, IDC_MAPID, m_strMapID);
	DDX_Text(pDX, IDC_MAPNAME, m_strMapName);
	DDX_Text(pDX, IDC_DARKLEVEL, m_nDarkLevel);
	DDX_Control(pDX, IDC_BGMID, m_cmbBGMID);
	DDX_Control(pDX, IDC_WEATHERTYPE, m_cmbWeatherType);
	DDX_Check(pDX, IDC_ENABLEBATTLE, m_bEnableBattle);
	DDX_Check(pDX, IDC_RECOVERY, m_bRecovery);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapInfo, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapInfo)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_CHANGEMAPNAME, OnChangemapname)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminMapInfo::CDlgAdminMapInfo								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

CDlgAdminMapInfo::CDlgAdminMapInfo(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapInfo::IDD, pParent)
	, m_bEnableBattle(TRUE)
	, m_bRecovery(TRUE)
{
	//{{AFX_DATA_INIT(CDlgAdminMapInfo)
	m_strMapSize = _T("");
	m_nValue = 0;
	m_strMapID = _T("");
	m_strMapName = _T("");
	m_nDarkLevel = 0;
	//}}AFX_DATA_INIT

	m_pInfoMap = NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapInfo::~CDlgAdminMapInfo							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

CDlgAdminMapInfo::~CDlgAdminMapInfo()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapInfo::Init											 */
/* 内容		:初期化															 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

void CDlgAdminMapInfo::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pInfoMap = m_pMgrData->GetMap ();

	/* ウィンドウ作成 */
	Create (CDlgAdminMapInfo::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapInfo::Renew										 */
/* 内容		:更新															 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

void CDlgAdminMapInfo::Renew(void)
{
	m_pInfoMap = m_pMgrData->GetMap ();
	if (m_pInfoMap == NULL) {
		return;
	}

	m_strMapSize.	Format ("(%d, %d)", m_pInfoMap->m_sizeMap.cx, m_pInfoMap->m_sizeMap.cy);
	m_strMapID.		Format ("%u", m_pInfoMap->m_dwMapID);

	m_strMapName = m_pInfoMap->m_strMapName;

	SelectCmb (&m_cmbBGMID,			m_pInfoMap->m_dwBGMID);
	SelectCmb (&m_cmbWeatherType,	m_pInfoMap->m_dwWeatherType);
	m_nDarkLevel	= m_pInfoMap->m_byLevel;		/* 暗度 */
	m_bEnableBattle	= m_pInfoMap->m_bEnableBattle;	/* 戦闘許可 */
	m_bRecovery		= m_pInfoMap->m_bRecovery;		/* 気絶後回復する */

	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapInfo::OnAdminMsg									 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

void CDlgAdminMapInfo::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_RENEWMAPINFO:	/* マップ情報更新 */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapInfo::OnInitDialog									 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

BOOL CDlgAdminMapInfo::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog ();

	CheckRadioButton (IDC_UP, IDC_RIGHT, IDC_UP);

	m_cmbBGMID.InsertString (0, "無し");
	m_cmbBGMID.InsertString (1, "大地の鼓動 short ver.");
	m_cmbBGMID.SetItemData	(1, BGMID_DAICHI_S);
	m_cmbBGMID.InsertString (2, "飛翔");
	m_cmbBGMID.SetItemData	(2, BGMID_HISYOU);
	m_cmbBGMID.InsertString (3, "水車の見える情景");
	m_cmbBGMID.SetItemData	(3, BGMID_SUISHA);
	m_cmbBGMID.InsertString (4, "A Little Fairy Tale");
	m_cmbBGMID.SetItemData	(4, BGMID_FAIRYTALE);
	m_cmbBGMID.InsertString (5, "Tabla_Image");
	m_cmbBGMID.SetItemData	(5, BGMID_TABLA_IMAGE);
	m_cmbBGMID.InsertString (6, "flowed piano solo");
	m_cmbBGMID.SetItemData	(6, BGMID_FLOWED_PIANO);
	m_cmbBGMID.InsertString (7, "冬の街　フルオケver");
	m_cmbBGMID.SetItemData	(7, BGMID_HUYUNOMATI_FULL);
	m_cmbBGMID.InsertString (8, "スーパーおやこどん 並盛ver/北");
	m_cmbBGMID.SetItemData	(8, BGMID_OYAKODON_NAMI);

	m_cmbWeatherType.InsertString (0, "無し");
	m_cmbWeatherType.InsertString (1, "雲");
	m_cmbWeatherType.SetItemData  (1, WEATHERTYPE_CLOUD);
	m_cmbWeatherType.InsertString (2, "霧");
	m_cmbWeatherType.SetItemData  (2, WEATHERTYPE_MISTY);
	m_cmbWeatherType.InsertString (3, "雪");
	m_cmbWeatherType.SetItemData  (3, WEATHERTYPE_SNOW);

	Renew ();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapInfo::OnAdd										 */
/* 内容		:ボタンハンドラ(増やす)											 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

void CDlgAdminMapInfo::OnAdd()
{
	int nResult, nDirection;
	CPacketADMIN_MAP_RENEWMAPSIZE Packet;

	if (m_pInfoMap == NULL) {
		return;
	}

	UpdateData ();
	if (m_nValue <= 0) {
		return;
	}

	nDirection = 0;
	nResult = GetCheckedRadioButton (IDC_UP, IDC_RIGHT);
	switch (nResult) {
	case IDC_UP:
		nDirection = 0;
		break;
	case IDC_DOWN:
		nDirection = 1;
		break;
	case IDC_LEFT:
		nDirection = 2;
		break;
	case IDC_RIGHT:
		nDirection = 3;
		break;
	default:
		return;
	}
	Packet.Make (m_pInfoMap->m_dwMapID, nDirection, m_nValue);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapInfo::OnDel										 */
/* 内容		:ボタンハンドラ(減らす)											 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

void CDlgAdminMapInfo::OnDel()
{
	int nResult, nDirection;
	CPacketADMIN_MAP_RENEWMAPSIZE Packet;

	if (m_pInfoMap == NULL) {
		return;
	}

	UpdateData ();
	if (m_nValue <= 0) {
		return;
	}

	nDirection = 0;
	nResult = GetCheckedRadioButton (IDC_UP, IDC_RIGHT);
	switch (nResult) {
	case IDC_UP:
		nDirection = 0;
		break;
	case IDC_DOWN:
		nDirection = 1;
		break;
	case IDC_LEFT:
		nDirection = 2;
		break;
	case IDC_RIGHT:
		nDirection = 3;
		break;
	default:
		return;
	}
	Packet.Make (m_pInfoMap->m_dwMapID, nDirection, m_nValue * -1);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapInfo::OnChangemapname								 */
/* 内容		:ボタンハンドラ(変更)											 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

void CDlgAdminMapInfo::OnChangemapname()
{
	int nSelect;
	CPacketADMIN_MAP_SETMAPNAME Packet;

	if (m_pInfoMap == NULL) {
		return;
	}

	UpdateData ();

	nSelect = m_cmbBGMID.GetCurSel ();
	m_pInfoMap->m_dwBGMID = m_cmbBGMID.GetItemData (nSelect);				/* BGMID */
	nSelect = m_cmbWeatherType.GetCurSel ();
	m_pInfoMap->m_dwWeatherType = m_cmbWeatherType.GetItemData (nSelect);	/* 天気種別 */
	m_pInfoMap->m_byLevel		= (BYTE)m_nDarkLevel;						/* 暗度 */
	m_pInfoMap->m_bEnableBattle	= m_bEnableBattle;							/* 戦闘許可 */
	m_pInfoMap->m_bRecovery		= m_bRecovery;								/* 気絶後回復する */

	Packet.Make (
			m_pInfoMap->m_dwMapID,
			m_pInfoMap->m_dwBGMID,
			m_pInfoMap->m_dwWeatherType,
			m_pInfoMap->m_bEnableBattle,
			m_pInfoMap->m_bRecovery,
			m_pInfoMap->m_byLevel,
			(LPCSTR)m_strMapName);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapInfo::OnCopy										 */
/* 内容		:ボタンハンドラ(コピー)											 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

void CDlgAdminMapInfo::OnCopy()
{
	int nResult;
	CPacketADMIN_PARA2 Packet;

	nResult = MessageBox ("新しいマップとしてコピーしますか？", "確認", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make (SBOCOMMANDID_SUB_ADMIN_MAP_ADD, m_pInfoMap->m_dwMapID, 0);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2007 */
