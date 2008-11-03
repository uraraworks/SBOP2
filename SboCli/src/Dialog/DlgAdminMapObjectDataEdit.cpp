/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapObjectDataEdit.cpp								 */
/* 内容			:マップオブジェクト配置データ編集ダイアログクラス 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/03													 */
/* ========================================================================= */

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

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

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


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectDataEdit::CDlgAdminMapObjectDataEdit			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

CDlgAdminMapObjectDataEdit::CDlgAdminMapObjectDataEdit(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapObjectDataEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapObjectDataEdit)
	m_nPosX = 0;
	m_nPosY = 0;
	//}}AFX_DATA_INIT

	m_bModeModify		= FALSE;
	m_ppWndNotify		= NULL;
	m_pLibInfoMapObject	= NULL;
	m_pInfo				= NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectDataEdit::~CDlgAdminMapObjectDataEdit		 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

CDlgAdminMapObjectDataEdit::~CDlgAdminMapObjectDataEdit()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectDataEdit::Init								 */
/* 内容		:初期化															 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectDataEdit::Init(CMgrData *pMgrData, CWnd **pWndNotify)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoMapObject = m_pMgrData->GetLibInfoMapObject ();
	m_pInfo = (PCInfoMapObjectData)m_pLibInfoMapObject->GetNew ();

	m_ppWndNotify = pWndNotify;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectDataEdit::Get								 */
/* 内容		:編集内容を取得													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectDataEdit::Get(CInfoMapObjectData *&pDst)
{
	if (m_pInfo == NULL) {
		return;
	}

	pDst->Copy (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectDataEdit::SetModify							 */
/* 内容		:編集モードとして設定											 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectDataEdit::SetModify(CInfoMapObjectData *pSrc)
{
	CLibInfoMapObjectData LibInfo;

	SAFE_DELETE (m_pInfo);
	m_pInfo = (PCInfoMapObjectData)LibInfo.GetNew ();
	m_pInfo->Copy (pSrc);

	m_nPosX  = m_pInfo->m_ptPos.x;
	m_nPosY  = m_pInfo->m_ptPos.y;

	m_bModeModify = TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectDataEdit::OnInitDialog						 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

BOOL CDlgAdminMapObjectDataEdit::OnInitDialog()
{
	int i, nNo, nCount;
	PCInfoMapObject pInfo;

	CDlgAdminBase::OnInitDialog();

	if (m_ppWndNotify) {
		*m_ppWndNotify = this;
	}

	if (m_bModeModify) {
		SetWindowText ("マップオブジェクト配置データの編集");
	}

	nCount = m_pLibInfoMapObject->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObject)m_pLibInfoMapObject->GetPtr (i);
		m_ctlType.InsertString (i, (LPCSTR)pInfo->m_strName);
		m_ctlType.SetItemData (i, pInfo->m_dwObjectID);
	}

	nNo = 0;
	if (m_pInfo) {
		for (i = 0; i < nCount; i ++) {
			if (m_pInfo->m_dwObjectID == m_ctlType.GetItemData (i)) {
				nNo = i;
				break;
			}
		}
	}
	m_ctlType.SetCurSel (nNo);

	OnSelchangeType ();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnAdminMsg										 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

LRESULT CDlgAdminMapObjectDataEdit::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:		/* 左クリック通知 */
		m_nPosX = HIWORD (lParam);
		m_nPosY = LOWORD (lParam);
		UpdateData (FALSE);
		break;
	case ADMINMSG_NOTIFYTYPE_RBUTTONDOWN:		/* 右クリック通知 */
		break;
	}

	return -1;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectDataEdit::PostNcDestroy						 */
/* 内容		:終了処理														 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectDataEdit::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectDataEdit::OnSelchangeType					 */
/* 内容		:イベントハンドラ(CBN_SELCHANGE)								 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectDataEdit::OnSelchangeType()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectDataEdit::OnOK								 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectDataEdit::OnOK()
{
	int nNo;

	UpdateData ();

	nNo = m_ctlType.GetCurSel ();

	if (m_pInfo) {
		m_pInfo->m_ptPos.x		= m_nPosX;
		m_pInfo->m_ptPos.y		= m_nPosY;
		m_pInfo->m_dwObjectID	= m_ctlType.GetItemData (nNo);
	}

	CDlgAdminBase::EndDialog (IDOK);
}

/* Copyright(C)URARA-works 2008 */
