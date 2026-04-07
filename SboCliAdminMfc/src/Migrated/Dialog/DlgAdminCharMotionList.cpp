/// @file DlgAdminCharMotionList.cpp
/// @brief キャラモーションリストダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/10/28
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "resource.h"
#include "PacketADMIN_CHAR_RENEWMOTION.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoMotion.h"
#include "MgrData.h"
#include "myString.h"
#include "DlgAdminCharMotion.h"
#include "DlgAdminCharMotionList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminCharMotionList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharMotionList)
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_MOTIONNAME, m_strMotionName);
	DDX_Text(pDX, IDC_GRPIDSUB, m_nGrpIDSub);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharMotionList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharMotionList)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDlgAdminCharMotionList::CDlgAdminCharMotionList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharMotionList::IDD, pParent)
	, m_nGrpIDSub(0)
{
	//{{AFX_DATA_INIT(CDlgAdminCharMotionList)
	m_strMotionName = _T("");
	//}}AFX_DATA_INIT

	m_dwMotionTypeID = 0;
}


CDlgAdminCharMotionList::~CDlgAdminCharMotionList()
{
}


void CDlgAdminCharMotionList::Init(CMgrData *pMgrData, DWORD dwMotionTypeID)
{
	CDlgAdminBase::Init(pMgrData);

	m_dwMotionTypeID = dwMotionTypeID;
}


void CDlgAdminCharMotionList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWMOTION: // モーション情報更新
		break;
	}
}


BOOL CDlgAdminCharMotionList::OnInitDialog()
{
	int i;
	LPCSTR pszItem;
	CmyString strTmp;

	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn(0, _T("ID"),	LVCFMT_LEFT, 40);
	m_List.InsertColumn(1, _T("編集するモーション"),	LVCFMT_LEFT, 300);

	for (i = CHARMOTIONLISTID_NONE + 1; i < CHARMOTIONLISTID_MAX; i ++) {
                pszItem = m_pMgrData->GetMotionName(i);
                if (pszItem == NULL) {
                        break;
                }
                strTmp.Format(_T("%d"), i);
                m_List.InsertItem(i - 1, strTmp);
                CString strName = Utf8ToTString(pszItem);
                m_List.SetItemText(i - 1, 1, strName);
	}

	return TRUE;
}


void CDlgAdminCharMotionList::PostNcDestroy()
{
}


void CDlgAdminCharMotionList::OnModify()
{
	int nNo, nResult;
	DWORD dwMotionListID;
	PCLibInfoMotion pLibInfo;
	CDlgAdminCharMotion Dlg(this);
	CPacketADMIN_CHAR_RENEWMOTION Packet;

	nNo = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwMotionListID = nNo + 1;

	UpdateData();

	pLibInfo = m_pMgrData->GetLibInfoMotion();

	Dlg.Init(m_pMgrData, m_dwMotionTypeID, dwMotionListID, m_nGrpIDSub);
	Dlg.SetList(pLibInfo);

	nResult = Dlg.DoModal();
	if (nResult != IDOK) {
		return;
	}
	Dlg.GetList(pLibInfo);

	Packet.Make(m_dwMotionTypeID, dwMotionListID, pLibInfo);
	SendPacket(&Packet);
}


void CDlgAdminCharMotionList::OnOK()
{
	UpdateData();

	CDlgAdminBase::EndDialog(IDOK);
}