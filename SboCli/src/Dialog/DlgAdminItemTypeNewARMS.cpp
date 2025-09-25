/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemTypeNewARMS.cpp								 */
/* 内容			:アイテム種別(持ち物)設定ダイアログクラス 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoItemTypeBase.h"
#include "LibInfoItemWeapon.h"
#include "InfoMotion.h"
#include "MgrData.h"
#include "DlgAdminItemTypeNewARMSBow.h"
#include "DlgAdminItemTypeNewARMS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMS::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemTypeNewARMS)
	DDX_Control(pDX, IDC_WEAPONTYPE, m_ctlWeaponType);
	DDX_Text(pDX, IDC_VALUE, m_dwValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminItemTypeNewARMS, CDlgAdminBase)
	ON_CBN_SELCHANGE(IDC_WEAPONTYPE, &CDlgAdminItemTypeNewARMS::OnCbnSelchangeWeapontype)
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewARMS::CDlgAdminItemTypeNewARMS				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/12/08														 */
/* ========================================================================= */

CDlgAdminItemTypeNewARMS::CDlgAdminItemTypeNewARMS(CWnd* pParent /*=NULL*/)
: CDlgAdminBase(CDlgAdminItemTypeNewARMS::IDD, pParent)
, m_pSrc(NULL)
, m_dwValue(0)
, m_dwWeaponInfoID(0)
, m_pDlgWeaponType(NULL)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewARMS::~CDlgAdminItemTypeNewARMS			 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/12/08														 */
/* ========================================================================= */

CDlgAdminItemTypeNewARMS::~CDlgAdminItemTypeNewARMS()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewARMS::Set									 */
/* 内容		:編集内容を設定													 */
/* 日付		:2007/12/08														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMS::Set(CInfoItemTypeBase *pSrc)
{
	m_pSrc = pSrc;
	m_dwWeaponInfoID	= pSrc->m_dwWeaponInfoID;		/* 武器情報ID */
	m_dwValue			= pSrc->m_dwValue;				/* 攻撃力 */
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewARMS::Get									 */
/* 内容		:編集内容を取得													 */
/* 日付		:2007/12/08														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMS::Get(CInfoItemTypeBase *&pDst)
{
	pDst->m_dwWeaponInfoID	= m_dwWeaponInfoID;		/* 武器情報ID */
	pDst->m_dwValue			= m_dwValue;			/* 攻撃力 */
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewARMS::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/12/08														 */
/* ========================================================================= */

BOOL CDlgAdminItemTypeNewARMS::OnInitDialog()
{
	int i, nCount;
	PCLibInfoItemWeapon pLibInfoItemWeapon;
	PCInfoItemWeapon pInfoItemWeapon;

	CDlgAdminBase::OnInitDialog();

	pLibInfoItemWeapon = m_pMgrData->GetLibInfoItemWeapon ();

	m_ctlWeaponType.InsertString (0, _T("未指定"));
	nCount = pLibInfoItemWeapon->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoItemWeapon = (PCInfoItemWeapon)pLibInfoItemWeapon->GetPtr (i);
		m_ctlWeaponType.InsertString (i + 1, Utf8ToTString ((LPCSTR)pInfoItemWeapon->m_strName));
		m_ctlWeaponType.SetItemData (i + 1, pInfoItemWeapon->m_dwWeaponInfoID);
	}

	SetWeaponType (m_dwWeaponInfoID);
	OnCbnSelchangeWeapontype();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewARMS::PostNcDestroy						 */
/* 内容		:終了処理														 */
/* 日付		:2007/09/30														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMS::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewARMS::OnOK									 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2007/12/08														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMS::OnOK()
{
	UpdateData ();

	if (m_pDlgWeaponType) {
		m_pDlgWeaponType->Get(m_pSrc);
	}
	m_dwWeaponInfoID = GetWeaponType ();

	CDlgAdminBase::EndDialog (IDOK);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewARMS::SetWeaponType						 */
/* 内容		:武器種別を設定													 */
/* 日付		:2007/12/09														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMS::SetWeaponType(DWORD dwWeaponInfoID)
{
	int i, nCount, nSelect;
	DWORD dwItemData;

	nSelect = 0;

	nCount = m_ctlWeaponType.GetCount ();
	for (i = 0; i < nCount; i ++) {
		dwItemData = m_ctlWeaponType.GetItemData (i);
		if (dwItemData != dwWeaponInfoID) {
			continue;
		}
		nSelect = i;
		break;
	}
	m_ctlWeaponType.SetCurSel (nSelect);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewARMS::GetWeaponType						 */
/* 内容		:武器種別を取得													 */
/* 日付		:2007/12/09														 */
/* ========================================================================= */

DWORD CDlgAdminItemTypeNewARMS::GetWeaponType(void)
{
	int nSelect;
	DWORD dwRet;

	dwRet = 0;

	nSelect = m_ctlWeaponType.GetCurSel ();
	if (nSelect < 0) {
		goto Exit;
	}

	dwRet = m_ctlWeaponType.GetItemData (nSelect);
Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewARMS::OnCbnSelchangeWeapontype				 */
/* 内容		:イベントハンドラ(CBN_SELCHANGE)								 */
/* 日付		:2010/01/05														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMS::OnCbnSelchangeWeapontype()
{
	if (m_pDlgWeaponType) {
		m_pDlgWeaponType->DestroyWindow();
		m_pDlgWeaponType = NULL;
	}

	DWORD dwWeaponInfoID = GetWeaponType();
	PCLibInfoItemWeapon pLibInfoItemWeapon = m_pMgrData->GetLibInfoItemWeapon ();
	PCInfoItemWeapon pInfoItemWeapon = (PCInfoItemWeapon)pLibInfoItemWeapon->GetPtr(dwWeaponInfoID);
	if (pInfoItemWeapon == NULL) {
		return;
	}

	if (pInfoItemWeapon->m_dwMotionType & INFOITEMARMS_MOTION_BOW) {
		m_pDlgWeaponType = new CDlgAdminItemTypeNewARMSBow(this);
		m_pDlgWeaponType->Create(CDlgAdminItemTypeNewARMSBow::IDD, this);
		m_pDlgWeaponType->Set(m_pSrc);
	}

	if (m_pDlgWeaponType == NULL) {
		return;
	}
	CRect rcTmp;
	GetDlgItem(IDC_DLGSUB)->GetWindowRect(rcTmp);
	ScreenToClient(rcTmp);
	m_pDlgWeaponType->ShowWindow(SW_SHOW);
	m_pDlgWeaponType->MoveWindow(&rcTmp);
}

/* Copyright(C)URARA-works 2007 */
