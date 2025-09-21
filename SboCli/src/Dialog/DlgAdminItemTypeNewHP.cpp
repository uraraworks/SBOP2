/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemTypeNewHP.cpp									 */
/* 内容			:アイテム種別(HP増減)設定ダイアログクラス 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/15													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoItemTypeBase.h"
#include "InfoMotion.h"
#include "MgrData.h"
#include "DlgAdminItemTypeNewHP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminItemTypeNewHP::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemTypeNewHP)
	DDX_Control(pDX, IDC_TARGET, m_ctlTarget);
	DDX_Control(pDX, IDC_AREA, m_ctlArea);
	DDX_Text(pDX, IDC_VALUE1, m_nValue1);
	DDX_Text(pDX, IDC_VALUE2, m_nValue2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminItemTypeNewHP, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminItemTypeNewHP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewHP::CDlgAdminItemTypeNewHP					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

CDlgAdminItemTypeNewHP::CDlgAdminItemTypeNewHP(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminItemTypeNewHP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminItemTypeNewHP)
	m_nValue1 = 0;
	m_nValue2 = 0;
	//}}AFX_DATA_INIT

	m_byTarget = 0;		/* 使用対象 */
	m_byArea = 0;		/* 使用範囲 */
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewHP::~CDlgAdminItemTypeNewHP				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

CDlgAdminItemTypeNewHP::~CDlgAdminItemTypeNewHP()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewHP::Set									 */
/* 内容		:編集内容を設定													 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewHP::Set(CInfoItemTypeBase *pSrc)
{
	m_byTarget	= pSrc->m_byTarget;			/* 使用対象 */
	m_byArea	= pSrc->m_byArea;			/* 使用範囲 */
	m_nValue1	= (int)pSrc->m_dwValue;	/* 最小 */
	m_nValue2	= (int)pSrc->m_dwValue2;	/* 最大 */
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewHP::Get									 */
/* 内容		:編集内容を取得													 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewHP::Get(CInfoItemTypeBase *&pDst)
{
	pDst->m_byTarget	= m_byTarget;			/* 使用対象 */
	pDst->m_byArea		= m_byArea;				/* 使用範囲 */
	pDst->m_dwValue		= m_nValue1;			/* 最小 */
	pDst->m_dwValue2	= m_nValue2;			/* 最大 */
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewHP::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

BOOL CDlgAdminItemTypeNewHP::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_ctlTarget.InsertString (0, "味方");
	m_ctlTarget.InsertString (1, "敵");

	m_ctlArea.InsertString (0, "自分");
	m_ctlArea.InsertString (1, "1歩前");
	m_ctlArea.InsertString (2, "周囲");
	m_ctlArea.InsertString (3, "画面内");

	SetTarget (m_byTarget);
	SetArea (m_byArea);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewHP::PostNcDestroy							 */
/* 内容		:終了処理														 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewHP::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewHP::OnOK									 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewHP::OnOK()
{
	UpdateData ();

	m_byTarget	= GetTarget ();
	m_byArea	= GetArea ();

	CDlgAdminBase::EndDialog (IDOK);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewHP::SetTarget								 */
/* 内容		:使用対象を設定													 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewHP::SetTarget(BYTE byTarget)
{
	m_ctlTarget.SetCurSel (byTarget);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewHP::GetTarget								 */
/* 内容		:使用対象を取得													 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

BYTE CDlgAdminItemTypeNewHP::GetTarget(void)
{
	int nSelect;
	BYTE byRet;

	byRet	= 0;
	nSelect	= m_ctlTarget.GetCurSel ();
	if (nSelect >= 0) {
		byRet = (BYTE)nSelect;
	}

	return byRet;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewHP::SetArea								 */
/* 内容		:使用範囲を設定													 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewHP::SetArea(BYTE byTarget)
{
	m_ctlArea.SetCurSel (byTarget);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeNewHP::GetArea								 */
/* 内容		:使用範囲を取得													 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

BYTE CDlgAdminItemTypeNewHP::GetArea(void)
{
	int nSelect;
	BYTE byRet;

	byRet	= 0;
	nSelect	= m_ctlArea.GetCurSel ();
	if (nSelect >= 0) {
		byRet = (BYTE)nSelect;
	}

	return byRet;
}

/* Copyright(C)URARA-works 2008 */
