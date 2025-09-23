/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapEventGRPIDTMP.cpp								 */
/* 内容			:マップイベント(一時画像設定)ダイアログクラス 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/10/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "MgrData.h"
#include "InfoMapEventGRPIDTMP.h"
#include "DlgAdminMapEventGRPIDTMP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminMapEventGRPIDTMP::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminMapEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEventGRPIDTMP)
	DDX_Control(pDX, IDC_IDMAIN, m_ctlIDMain);
	DDX_Text(pDX, IDC_IDSUB, m_dwIDSub);
	DDX_CBIndex(pDX, IDC_IDMAIN, m_nSelect);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEventGRPIDTMP, CDlgAdminMapEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminMapEventGRPIDTMP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventGRPIDTMP::CDlgAdminMapEventGRPIDTMP			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

CDlgAdminMapEventGRPIDTMP::CDlgAdminMapEventGRPIDTMP(CWnd* pParent /*=NULL*/)
	: CDlgAdminMapEventNONE(pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEventGRPIDTMP)
	m_nSelect = 0;
	m_dwIDSub = 0;
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminMapEventGRPIDTMP::IDD;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventGRPIDTMP::~CDlgAdminMapEventGRPIDTMP			 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

CDlgAdminMapEventGRPIDTMP::~CDlgAdminMapEventGRPIDTMP()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventGRPIDTMP::Set									 */
/* 内容		:設定															 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

void CDlgAdminMapEventGRPIDTMP::Set(CInfoMapEventBase *pSrc)
{
	int i, nNo, nCount, nSelect;
	PCInfoMapEventGRPIDTMP pSrcTmp = (PCInfoMapEventGRPIDTMP)pSrc;

	m_dwIDSub = pSrcTmp->m_dwIDSub;

	/* 画像IDメイン */
	nNo = 0;
	nCount = m_ctlIDMain.GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (pSrcTmp->m_dwIDMain == m_ctlIDMain.GetItemData (i)) {
			nNo = i;
			break;
		}
	}
	nSelect = (pSrcTmp->m_nSetType == GRPIDTMPTYPE_ON) ? IDC_RADIO_ON : IDC_RADIO_OFF;
	CheckRadioButton (IDC_RADIO_OFF, IDC_RADIO_ON, nSelect);
	m_ctlIDMain.SetCurSel (nNo);

	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventGRPIDTMP::Get									 */
/* 内容		:取得															 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

void CDlgAdminMapEventGRPIDTMP::Get(CInfoMapEventBase *pDst)
{
	int nSelect;

	PCInfoMapEventGRPIDTMP pDstTmp = (PCInfoMapEventGRPIDTMP)pDst;

	UpdateData ();

	nSelect = GetCheckedRadioButton (IDC_RADIO_OFF, IDC_RADIO_ON);
	pDstTmp->m_nSetType	= (nSelect == IDC_RADIO_ON) ? GRPIDTMPTYPE_ON : GRPIDTMPTYPE_OFF;
	pDstTmp->m_dwIDMain	= m_ctlIDMain.GetItemData (m_nSelect);
	pDstTmp->m_dwIDSub	= m_dwIDSub;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventGRPIDTMP::OnInitDialog						 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

BOOL CDlgAdminMapEventGRPIDTMP::OnInitDialog()
{
	CDlgAdminMapEventNONE::OnInitDialog();

	/* 画像IDメイン */
	m_ctlIDMain.InsertString (0, _T("服(2x2)"));
	m_ctlIDMain.SetItemData  (0, GRPIDMAIN_2X2_CLOTH);
	m_ctlIDMain.InsertString (1, _T("特殊服(2x2)"));
	m_ctlIDMain.SetItemData  (1, GRPIDMAIN_2X2_SPCLOTH);

	m_ctlIDMain.SetCurSel (0);
	CheckRadioButton (IDC_RADIO_OFF, IDC_RADIO_ON, IDC_RADIO_OFF);

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
