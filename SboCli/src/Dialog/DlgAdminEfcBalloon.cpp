/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminEfcBalloon.cpp										 */
/* 内容			:噴出し情報設定ダイアログクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/25													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "LibInfoEfcBalloon.h"
#include "LibSboSoundLoader.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WndSelectGrp.h"
#include "DlgAdminEfcBalloon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminEfcBalloon)
	DDX_Control(pDX, IDC_SLIDER, m_ctlSlider);
	DDX_Control(pDX, IDC_SOUNDID, m_cmbSoundID);
	DDX_Control(pDX, IDC_GRP, m_ctlGrp);
	DDX_Check(pDX, IDC_LOOP, m_bLoop);
	DDX_Text(pDX, IDC_VIEWTIME, m_nViewTime);
	DDX_Text(pDX, IDC_LISTID, m_dwListID);
	DDX_Text(pDX, IDC_COUNT, m_strCount);
	DDX_Text(pDX, IDC_NOW, m_strNow);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminEfcBalloon, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminEfcBalloon)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WNDCLOSE, OnWndClose)
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::CDlgAdminEfcBalloon						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

CDlgAdminEfcBalloon::CDlgAdminEfcBalloon(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminEfcBalloon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminEfcBalloon)
	m_bLoop = FALSE;
	m_nViewTime = 0;
	m_dwListID = 0;
	m_strCount = _T("");
	m_strNow = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT

	m_bModify				= FALSE;
	m_nSelect				= -1;
	m_dwLastTimeAnime		= 0;
	m_pWndSelectGrp			= NULL;
	m_pLibInfoEfcBalloon	= NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::~CDlgAdminEfcBalloon						 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

CDlgAdminEfcBalloon::~CDlgAdminEfcBalloon()
{
	SAFE_DELETE (m_pLibInfoEfcBalloon);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::Init										 */
/* 内容		:初期化															 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::Init(CMgrData *pMgrData, DWORD dwListID, BOOL bModify)
{
	CDlgAdminBase::Init (pMgrData);

	m_bModify = bModify;
	m_pLibInfoEfcBalloon = new CLibInfoEfcBalloon;
	m_pLibInfoEfcBalloon->Create ();

	m_dwListID = dwListID;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::GetList									 */
/* 内容		:取得															 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::GetList(CLibInfoEfcBalloon *pDst)
{
	if (m_pLibInfoEfcBalloon == NULL) {
		return;
	}

	pDst->SetList (m_dwListID, m_pLibInfoEfcBalloon);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::SetList									 */
/* 内容		:設定															 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::SetList(CLibInfoEfcBalloon *pSrc)
{
	if (m_pLibInfoEfcBalloon == NULL) {
		return;
	}

	m_pLibInfoEfcBalloon->SetList (m_dwListID, pSrc);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::OnInitDialog								 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

BOOL CDlgAdminEfcBalloon::OnInitDialog()
{
	int i, nCount;
	DWORD dwSoundID;
	LPCSTR pszTmp;
	CLibSboSoundLoader LibSboSoundLoader;

	CDlgAdminBase::OnInitDialog();

	m_ctlGrp.Create (this, m_pMgrData);
	m_ctlGrp.Init (GRPIDMAIN_EFCBALLOON);

	m_cmbSoundID.InsertString (0, "無し");
	nCount = LibSboSoundLoader.GetSoundCount ();
	for (i = 0; i < nCount; i ++) {
		dwSoundID	= LibSboSoundLoader.GetSoundID (i);
		pszTmp		= LibSboSoundLoader.GetSoundName (dwSoundID);
		m_cmbSoundID.InsertString (i + 1, pszTmp);
		m_cmbSoundID.SetItemData (i + 1, dwSoundID);
	}

	m_cmbSoundID.SetCurSel (0);

	RenewSlider ();

	Get (0);
	nCount = m_ctlSlider.GetRangeMax ();
	if (nCount <= 0) {
		OnAdd ();
	}

	if (m_bModify) {
		GetDlgItem (IDC_LISTID)->EnableWindow (FALSE);
	}

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::PostNcDestroy								 */
/* 内容		:終了処理														 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::OnLButtonDown								 */
/* 内容		:メッセージハンドラ(WM_LBUTTONDOWN)								 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bResult;
	CRect rc;
	CPoint ptTmp;

	if (m_nSelect < 0) {
		goto Exit;
	}
	if (m_pWndSelectGrp) {
		goto Exit;
	}

	ptTmp = point;
	ClientToScreen (&ptTmp);

	GetDlgItem (IDC_GRP)->GetWindowRect (rc);
	bResult = rc.PtInRect (ptTmp);
	if (bResult == FALSE) {
		goto Exit;
	}
	m_pWndSelectGrp = new CWndSelectGrp;
	m_pWndSelectGrp->Create (this, m_pMgrData, 0);
	m_pWndSelectGrp->SetType (GRPIDMAIN_EFCBALLOON);
	m_pWndSelectGrp->ShowWindow (SW_SHOW);

Exit:
	CDlgAdminBase::OnLButtonDown (nFlags, point);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::OnWndClose								 */
/* 内容		:メッセージハンドラ(WM_WNDCLOSE)								 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

LRESULT CDlgAdminEfcBalloon::OnWndClose(WPARAM wParam, LPARAM lParam)
{
	int nHi, nLow;

	nHi  = HIWORD (lParam);
	nLow = LOWORD (lParam);

	switch (wParam) {
	case WINDOWID_SELECTMAPPARTSGRP:	/* 画像選択 */
		m_pWndSelectGrp = NULL;
		if (nLow == 0xFFFF) {
			break;
		}
		m_ctlGrp.Set (nLow);
		Set (m_nSelect);
		break;
	}

	return -1;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::OnHScroll									 */
/* 内容		:メッセージハンドラ(WM_HSCROLL)									 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	OnSelectAnime ();

	CDlgAdminBase::OnHScroll (nSBCode, nPos, pScrollBar);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::OnTimer									 */
/* 内容		:メッセージハンドラ(WM_TIMER)									 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::OnTimer(UINT nIDEvent)
{
	int nMax, nSelect;
	DWORD dwTime, dwTmp;
	PCInfoEfcBalloon pInfo;

	nMax	= m_ctlSlider.GetRangeMax ();
	nSelect	= m_nSelect;

	pInfo = (PCInfoEfcBalloon)m_pLibInfoEfcBalloon->GetPtr (nSelect);
	if (pInfo == NULL) {
		goto Exit;
	}

	dwTime = timeGetTime () - m_dwLastTimeAnime;
	dwTmp  = (DWORD)pInfo->m_byWait * 10;
	if (dwTime < dwTmp) {
		goto Exit;
	}
	m_dwLastTimeAnime = timeGetTime ();

	nSelect ++;
	if (nSelect > nMax) {
		nSelect = 0;
		if (m_bLoop == FALSE) {
			PostMessage (WM_COMMAND, IDC_STOP);
			m_dwLastTimeAnime = 0;
		}
	}

	m_ctlSlider.SetPos (nSelect);
	OnSelectAnime ();
Exit:
	CDlgAdminBase::OnTimer (nIDEvent);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::OnAdd										 */
/* 内容		:ボタンハンドラ(追加)											 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::OnAdd()
{
	PCInfoEfcBalloon pInfo;

	pInfo = (PCInfoEfcBalloon)m_pLibInfoEfcBalloon->GetNew ();
	pInfo->m_dwListID = m_dwListID;
	pInfo->m_dwGrpID  = GRPIDMAIN_EFCBALLOON;

	m_pLibInfoEfcBalloon->Add (pInfo);
	if (m_nSelect == -1) {
		m_nSelect = 0;
	}
	pInfo->m_dwAnimeID = m_nSelect;
	Get (m_nSelect);

	RenewSlider ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::OnDel										 */
/* 内容		:ボタンハンドラ(削除)											 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::OnDel()
{
	int nResult, nCount;

        nResult = MessageBox (_T("このパターンを削除しますか？"), _T("確認"), MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	m_pLibInfoEfcBalloon->Delete (m_nSelect);
	nCount = m_pLibInfoEfcBalloon->GetCount ();
	if (m_nSelect >= nCount) {
		m_nSelect = nCount - 1;
	}
	if (nCount <= 1) {
		m_ctlSlider.SetPos (0);
	}
	Get (m_nSelect);

	RenewSlider ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::OnPlay									 */
/* 内容		:ボタンハンドラ(再生)											 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::OnPlay()
{
	Set (m_nSelect);

	SetTimer (100, 10, NULL);
	m_dwLastTimeAnime = timeGetTime ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::OnStop									 */
/* 内容		:ボタンハンドラ(停止)											 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::OnStop()
{
	KillTimer (100);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::OnOK										 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::OnOK()
{
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	Set (m_nSelect);

        if (m_dwListID == 0) {
                MessageBox (_T("種別IDを設定してください"), _T("確認"), MB_ICONINFORMATION | MB_OK);
		return;
	}

	nCount = m_pLibInfoEfcBalloon->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoEfcBalloon)m_pLibInfoEfcBalloon->GetPtr (i);
		pInfo->m_dwListID = m_dwListID;
	}

	CDlgAdminBase::OnOK ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::RenewSlider								 */
/* 内容		:スライダーを更新												 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::RenewSlider(void)
{
	BOOL bEnable;
	int nMax, nCount;

	nCount	= m_pLibInfoEfcBalloon->GetCount ();
	nMax	= max (nCount - 1, 0);
	m_ctlSlider.SetRange (0, nMax);

	OnSelectAnime ();

	bEnable = TRUE;
	if (nCount == 0) {
		bEnable = FALSE;
	}
	Enable (bEnable);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::OnSelectAnime								 */
/* 内容		:コマ選択処理													 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::OnSelectAnime(void)
{
	int nCount;

	Set (m_nSelect);
	m_nSelect = m_ctlSlider.GetPos ();

	nCount = m_pLibInfoEfcBalloon->GetCount ();
	m_strCount.Format ("%d", nCount);
	m_strNow.Empty ();
	if (nCount > 0) {
		m_strNow.Format ("%d", m_nSelect + 1);
	}
	UpdateData (FALSE);

	Get (m_nSelect);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::Enable									 */
/* 内容		:入力禁止設定													 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::Enable(BOOL bEnable)
{
	int i, nResourceID,
		anResourceID[] = {
			IDC_VIEWTIME,
			IDC_DEL,
			IDC_PLAY,
			IDC_STOP,
			IDC_LOOP,
			IDC_SLIDER,
			-1
		};

	for (i = 0; ; i ++) {
		nResourceID = anResourceID[i];
		if (nResourceID < 0) {
			break;
		}
		GetDlgItem (nResourceID)->EnableWindow (bEnable);
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::Set										 */
/* 内容		:噴出し情報へ設定												 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::Set(int nNo)
{
	int nSelect;
	PCInfoEfcBalloon pInfo;

	pInfo = (PCInfoEfcBalloon)m_pLibInfoEfcBalloon->GetPtr (nNo);
	if (pInfo == NULL) {
		return;
	}

	UpdateData ();
	pInfo->m_byWait		= (BYTE)m_nViewTime;			/* 待ち時間(×１０ミリ秒) */
	pInfo->m_bLoop		= m_bLoop;						/* ループ再生 */
	pInfo->m_dwListID	= m_dwListID;					/* 種別ID */
	pInfo->m_dwGrpID	= m_ctlGrp.GetIDSub ();			/* グラフィックID */
	pInfo->m_strName	= m_strName;					/* 噴出し名 */
	pInfo->m_dwAnimeID	= nNo;							/* コマ番号 */

	nSelect = m_cmbSoundID.GetCurSel ();
	pInfo->m_dwSoundID = m_cmbSoundID.GetItemData (nSelect);	/* 効果音ID */
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloon::Get										 */
/* 内容		:噴出し情報から取得												 */
/* 日付		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::Get(int nNo)
{
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	pInfo = (PCInfoEfcBalloon)m_pLibInfoEfcBalloon->GetPtr (nNo);
	if (pInfo == NULL) {
		return;
	}

	m_nViewTime	= pInfo->m_byWait;			/* 待ち時間(×１０ミリ秒) */
	m_dwListID	= pInfo->m_dwListID;		/* 種別ID */
	m_bLoop		= pInfo->m_bLoop;			/* ループ再生 */
	m_ctlGrp.Init (GRPIDMAIN_EFCBALLOON);	/* グラフィックIDメイン */
	m_ctlGrp.Set (pInfo->m_dwGrpID);		/* グラフィックIDサブ */
	if (nNo == 0) {
		m_strName = pInfo->m_strName;		/* 噴出し名 */
	}

	nCount = m_cmbSoundID.GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (pInfo->m_dwSoundID != m_cmbSoundID.GetItemData (i)) {
			continue;
		}
		m_cmbSoundID.SetCurSel (i);
		break;
	}

	UpdateData (FALSE);
}

/* Copyright(C)URARA-works 2007 */
