/// @file DlgAdminMapObjectDataEdit.cpp
/// @brief マップオブジェクト配置データ編集ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/03
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "LibInfoMapObject.h"
#include "LibInfoMapObjectData.h"
#include "MgrData.h"
#include "DlgAdminMapObjectDataEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminMapObjectDataEdit::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapObjectDataEdit)
	DDX_Control(pDX, IDC_TYPE, m_ctlType);
	DDX_Text(pDX, IDC_POSX, m_nPosX);
	DDX_Text(pDX, IDC_POSY, m_nPosY);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapObjectDataEdit, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapObjectDataEdit)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDlgAdminMapObjectDataEdit::CDlgAdminMapObjectDataEdit(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapObjectDataEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapObjectDataEdit)
	m_nPosX = 0;
	m_nPosY = 0;
	//}}AFX_DATA_INIT

	m_bModeModify	= FALSE;
	m_ppWndNotify	= NULL;
	m_pLibInfoMapObject	= NULL;
	m_pInfo	= NULL;
}


CDlgAdminMapObjectDataEdit::~CDlgAdminMapObjectDataEdit()
{
	SAFE_DELETE(m_pInfo);
}


void CDlgAdminMapObjectDataEdit::Init(CMgrData *pMgrData, CWnd **pWndNotify)
{
	CDlgAdminBase::Init(pMgrData);

	m_pLibInfoMapObject = m_pMgrData->GetLibInfoMapObject();
	m_pInfo = (PCInfoMapObjectData)m_pLibInfoMapObject->GetNew();

	m_ppWndNotify = pWndNotify;
}


void CDlgAdminMapObjectDataEdit::Get(CInfoMapObjectData *&pDst)
{
	if (m_pInfo == NULL) {
		return;
	}

	pDst->Copy(m_pInfo);
}


void CDlgAdminMapObjectDataEdit::SetModify(CInfoMapObjectData *pSrc)
{
	CLibInfoMapObjectData LibInfo;

	SAFE_DELETE(m_pInfo);
	m_pInfo = (PCInfoMapObjectData)LibInfo.GetNew();
	m_pInfo->Copy(pSrc);

	m_nPosX  = m_pInfo->m_ptPos.x;
	m_nPosY  = m_pInfo->m_ptPos.y;

	m_bModeModify = TRUE;
}


BOOL CDlgAdminMapObjectDataEdit::OnInitDialog()
{
	int i, nNo, nCount;
	PCInfoMapObject pInfo;

	CDlgAdminBase::OnInitDialog();

	if (m_ppWndNotify) {
		*m_ppWndNotify = this;
	}

	if (m_bModeModify) {
		SetWindowText(_T("マップオブジェクト配置データの編集"));
	}

	nCount = m_pLibInfoMapObject->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObject)m_pLibInfoMapObject->GetPtr(i);
		m_ctlType.InsertString(i, Utf8ToTString((LPCSTR)pInfo->m_strName));
		m_ctlType.SetItemData(i, pInfo->m_dwObjectID);
	}

	nNo = 0;
	if (m_pInfo) {
		for (i = 0; i < nCount; i ++) {
			if (m_pInfo->m_dwObjectID == m_ctlType.GetItemData(i)) {
				nNo = i;
				break;
			}
		}
	}
	m_ctlType.SetCurSel(nNo);

	OnSelchangeType();

	return TRUE;
}


LRESULT CDlgAdminMapObjectDataEdit::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN: // 左クリック通知
		m_nPosX = HIWORD(lParam);
		m_nPosY = LOWORD(lParam);
		UpdateData(FALSE);
		break;
	case ADMINMSG_NOTIFYTYPE_RBUTTONDOWN: // 右クリック通知
		break;
	}

	return -1;
}


void CDlgAdminMapObjectDataEdit::PostNcDestroy()
{
	CDialog::PostNcDestroy();
}


void CDlgAdminMapObjectDataEdit::OnSelchangeType()
{
}


void CDlgAdminMapObjectDataEdit::OnOK()
{
	int nNo;

	UpdateData();

	nNo = m_ctlType.GetCurSel();

	if (m_pInfo) {
		m_pInfo->m_ptPos.x	= m_nPosX;
		m_pInfo->m_ptPos.y	= m_nPosY;
		m_pInfo->m_dwObjectID	= m_ctlType.GetItemData(nNo);
	}

	CDlgAdminBase::EndDialog(IDOK);
}