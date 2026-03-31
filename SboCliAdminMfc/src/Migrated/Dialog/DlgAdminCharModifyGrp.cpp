/// @file DlgAdminCharModifyGrp.cpp
/// @brief 選択キャラ情報編集[画像の設定]ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/02
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_CHARINFO.h"
#include "LayoutHelper.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharModifyGrp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminCharModifyGrp::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharModifyGrp)
	DDX_Text(pDX, IDC_STATIC_CHARNAME, m_strCharName);
	DDX_Text(pDX, IDC_STATIC_CHARID, m_strCharID);
	DDX_Text(pDX, IDC_EYE, m_nGrpIDEye);
	DDX_Text(pDX, IDC_EYECOLOR, m_nGrpIDEyeColor);
	DDX_Text(pDX, IDC_HAIR, m_nGrpIDHair);
	DDX_Text(pDX, IDC_HAIRCOLOR, m_nGrpIDHairColor);
	DDX_Text(pDX, IDC_CLOTH, m_nGrpIDCloth);
	DDX_Text(pDX, IDC_SP, m_nGrpIDSP);
	DDX_Text(pDX, IDC_NPC, m_nGrpIDNPC);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharModifyGrp, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharModifyGrp)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgAdminCharModifyGrp::CDlgAdminCharModifyGrp(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminCharModifyGrp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharModifyGrp)
	m_strCharName = _T("");
	m_strCharID = _T("");
	m_nGrpIDEye = 0;
	m_nGrpIDEyeColor = 0;
	m_nGrpIDHair = 0;
	m_nGrpIDHairColor = 0;
	m_nGrpIDCloth = 0;
	m_nGrpIDSP = 0;
	m_nGrpIDNPC = 0;
	//}}AFX_DATA_INIT

	m_pInfoChar = NULL;
}

CDlgAdminCharModifyGrp::~CDlgAdminCharModifyGrp()
{
}

void CDlgAdminCharModifyGrp::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init(pMgrData);

	// ウィンドウ作成
	Create(CDlgAdminCharModifyGrp::IDD, m_pWndParent);
	ShowWindow(SW_SHOW);
}

void CDlgAdminCharModifyGrp::Renew(void)
{
	m_strCharID.	Empty();
	m_strCharName.	Empty();

	if (m_pInfoChar == NULL) {
		goto Exit;
	}

	m_strCharID.Format(_T("%u"), m_pInfoChar->m_dwCharID);
	m_strCharName	= (LPCTSTR)m_pInfoChar->m_strCharName;
	m_nGrpIDEye	= m_pInfoChar->m_wGrpIDEye;
	m_nGrpIDEyeColor	= m_pInfoChar->m_wGrpIDEyeColor;
	m_nGrpIDHair	= m_pInfoChar->m_wGrpIDHairType;
	m_nGrpIDHairColor	= m_pInfoChar->m_wGrpIDHairColor;
	m_nGrpIDCloth	= m_pInfoChar->m_wGrpIDCloth;
	m_nGrpIDSP	= m_pInfoChar->m_wGrpIDSP;
	m_nGrpIDNPC	= m_pInfoChar->m_wGrpIDNPC;

Exit:
	UpdateData(FALSE);
}

void CDlgAdminCharModifyGrp::OnAdminMsg(int nType, DWORD dwPara)
{
	PCLibInfoCharCli pLibInfoChar;

	pLibInfoChar	= m_pMgrData->GetLibInfoChar();
	m_pInfoChar	= (PCInfoCharCli)pLibInfoChar->GetPtr(dwPara);

	Renew();
}

BOOL CDlgAdminCharModifyGrp::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}

void CDlgAdminCharModifyGrp::OnSend()
{
	CInfoCharCli InfoCharTmp;
	CPacketADMIN_CHARINFO Packet;

	if (m_pInfoChar == NULL) {
		return;
	}
	UpdateData();

	InfoCharTmp.Create(m_pMgrData);
	InfoCharTmp.Copy(m_pInfoChar);
	InfoCharTmp.m_wGrpIDEye	= m_nGrpIDEye;
	InfoCharTmp.m_wGrpIDEyeColor	= m_nGrpIDEyeColor;
	InfoCharTmp.m_wGrpIDHairType	= m_nGrpIDHair;
	InfoCharTmp.m_wGrpIDHairColor	= m_nGrpIDHairColor;
	InfoCharTmp.m_wGrpIDCloth	= m_nGrpIDCloth;
	InfoCharTmp.m_wGrpIDSP	= m_nGrpIDSP;
	InfoCharTmp.m_wGrpIDNPC	= m_nGrpIDNPC;

	Packet.Make(&InfoCharTmp, FALSE);
	SendPacket(&Packet);
}

