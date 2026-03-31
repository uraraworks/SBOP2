/// @file DlgAdminCharAddNPC.cpp
/// @brief NPCの追加ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/01
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_CHAR_ADDNPC.h"
#include "LayoutHelper.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "SBOGlobal.h"
#include "DlgAdminCharAddNPC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminCharAddNPC::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharAddNPC)
	DDX_Control(pDX, IDC_FAMILYTYPE, m_ctlFamilyType);
	DDX_Text(pDX, IDC_CHARNAME, m_strCharName);
	DDX_Text(pDX, IDC_POSMAPID, m_nMapID);
	DDX_Text(pDX, IDC_POSX, m_nPosX);
	DDX_Text(pDX, IDC_POSY, m_nPosY);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharAddNPC, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharAddNPC)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgAdminCharAddNPC::CDlgAdminCharAddNPC(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminCharAddNPC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharAddNPC)
	m_strCharName = _T("");
	m_nMapID = 0;
	m_nPosX = 0;
	m_nPosY = 0;
	//}}AFX_DATA_INIT

	m_pInfoChar = NULL;
}

CDlgAdminCharAddNPC::~CDlgAdminCharAddNPC()
{
}

void CDlgAdminCharAddNPC::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init(pMgrData);

	// ウィンドウ作成
	Create(CDlgAdminCharAddNPC::IDD, m_pWndParent);
	ShowWindow(SW_SHOW);
}

void CDlgAdminCharAddNPC::Renew(void)
{
	if (m_pInfoChar == NULL) {
		goto Exit;
	}

Exit:
	UpdateData(FALSE);
}

void CDlgAdminCharAddNPC::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:	// 左クリック通知
		switch (m_pMgrData->GetAdminNotifyTypeL()) {
		case ADMINNOTIFYTYPE_POS:	// マップ座標
			UpdateData();
			m_nMapID = m_pInfoChar->m_dwMapID;
			m_nPosX  = HIWORD(dwPara);
			m_nPosY  = LOWORD(dwPara);
			UpdateData(FALSE);
			break;
		}
		break;
	}
}

BOOL CDlgAdminCharAddNPC::OnInitDialog()
{
	int i;
	LPCSTR pszFamilyName;

	CDlgAdminBase::OnInitDialog();

	m_pInfoChar = m_pMgrData->GetPlayerChar();

        for (i = FAMILYTYPE_NONE + 1; i < FAMILYTYPE_MAX; i ++) {
                pszFamilyName = m_pMgrData->GetFamilyTypeName(i);
#ifdef _UNICODE
                m_ctlFamilyType.AddString(Utf8ToTString(pszFamilyName));
#else
                m_ctlFamilyType.AddString(pszFamilyName);
#endif
        }
        m_ctlFamilyType.SetCurSel(0);

	return TRUE;
}

void CDlgAdminCharAddNPC::OnSend()
{
	int nResult;
	CInfoCharCli InfoCharTmp;
	CPacketADMIN_CHAR_ADDNPC Packet;

	UpdateData();

        if (m_nMapID == 0) {
                MessageBox(_T("マップIDと座標を確認してください"), _T("確認"), MB_ICONINFORMATION | MB_OK);
                return;
        }

        nResult = MessageBox(_T("指定座標にNPCを追加しますか？"), _T("確認"), MB_ICONQUESTION | MB_YESNO);
	if (nResult != IDYES) {
		return;
	}

	// 入力内容をキャラ情報へ反映
	InfoCharTmp.Create(m_pMgrData);
	InfoCharTmp.m_strCharName	= m_strCharName;
	InfoCharTmp.m_dwMapID	= m_nMapID;
	InfoCharTmp.m_nMapX	= m_nPosX * 2;
	InfoCharTmp.m_nMapY	= m_nPosY * 2;
	InfoCharTmp.m_nMoveType	= CHARMOVETYPE_STAND;
	InfoCharTmp.m_wFamilyID	= m_ctlFamilyType.GetCurSel() + 1;

	Packet.Make(&InfoCharTmp);
	SendPacket(&Packet);

	// 送信したので二度押し防止に座標をクリアしておく
	m_nMapID = m_nPosX = m_nPosY = 0;
	UpdateData(FALSE);
}

