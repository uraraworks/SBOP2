/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapEventINITSTATUS.cpp								 */
/* 内容			:マップイベント(ステータス初期化)ダイアログクラス 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/10/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapEventINITSTATUS.h"
#include "LibInfoEffect.h"
#include "MgrData.h"
#include "DlgAdminMapEventINITSTATUS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminMapEventINITSTATUS::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminMapEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEventINITSTATUS)
	DDX_Control(pDX, IDC_EFFECTID, m_ctlEffectID);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEventINITSTATUS, CDlgAdminMapEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminMapEventINITSTATUS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventINITSTATUS::CDlgAdminMapEventINITSTATUS		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/10/05														 */
/* ========================================================================= */

CDlgAdminMapEventINITSTATUS::CDlgAdminMapEventINITSTATUS(CWnd* pParent /*=NULL*/)
	: CDlgAdminMapEventNONE(pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEventINITSTATUS)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminMapEventINITSTATUS::IDD;

	m_dwEffectID = 0;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventINITSTATUS::~CDlgAdminMapEventINITSTATUS		 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/10/05														 */
/* ========================================================================= */

CDlgAdminMapEventINITSTATUS::~CDlgAdminMapEventINITSTATUS()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventINITSTATUS::Set								 */
/* 内容		:設定															 */
/* 日付		:2008/10/05														 */
/* ========================================================================= */

void CDlgAdminMapEventINITSTATUS::Set(CInfoMapEventBase *pSrc)
{
	int i, nNo, nCount;
	PCInfoMapEventINITSTATUS pSrcTmp = (PCInfoMapEventINITSTATUS)pSrc;

	m_dwEffectID = pSrcTmp->m_dwEffectID;

	/* エフェクトID */
	nNo = 0;
	nCount = m_ctlEffectID.GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (m_dwEffectID == m_ctlEffectID.GetItemData (i)) {
			nNo = i;
			break;
		}
	}
	m_ctlEffectID.SetCurSel (nNo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventINITSTATUS::Get								 */
/* 内容		:取得															 */
/* 日付		:2008/10/05														 */
/* ========================================================================= */

void CDlgAdminMapEventINITSTATUS::Get(CInfoMapEventBase *pDst)
{
	int nNo;
	PCInfoMapEventINITSTATUS pDstTmp = (PCInfoMapEventINITSTATUS)pDst;

	nNo = m_ctlEffectID.GetCurSel ();
	pDstTmp->m_dwEffectID = m_ctlEffectID.GetItemData (nNo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventINITSTATUS::OnInitDialog						 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/10/05														 */
/* ========================================================================= */

BOOL CDlgAdminMapEventINITSTATUS::OnInitDialog()
{
	int i, nCount;
	PCInfoEffect pInfoEffect;
	PCLibInfoEffect pLibInfoEffect;

	CDlgAdminMapEventNONE::OnInitDialog();

	/* エフェクトID */
	m_ctlEffectID.InsertString (0, "無し");
	pLibInfoEffect = m_pMgrData->GetLibInfoEffect ();
	nCount = pLibInfoEffect->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoEffect = (PCInfoEffect)pLibInfoEffect->GetPtr (i);
		m_ctlEffectID.InsertString (i + 1, (LPCTSTR)pInfoEffect->m_strName);
		m_ctlEffectID.SetItemData (i + 1, pInfoEffect->m_dwEffectID);
	}

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
