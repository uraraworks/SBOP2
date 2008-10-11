/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminEfcBalloon.cpp										 */
/* ���e			:���o�����ݒ�_�C�A���O�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/25													 */
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
/* �N���X�̐ݒ�																 */
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
/* �֐���	:CDlgAdminEfcBalloon::CDlgAdminEfcBalloon						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/12/25														 */
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
/* �֐���	:CDlgAdminEfcBalloon::~CDlgAdminEfcBalloon						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/12/25														 */
/* ========================================================================= */

CDlgAdminEfcBalloon::~CDlgAdminEfcBalloon()
{
	SAFE_DELETE (m_pLibInfoEfcBalloon);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloon::Init										 */
/* ���e		:������															 */
/* ���t		:2007/12/25														 */
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
/* �֐���	:CDlgAdminEfcBalloon::GetList									 */
/* ���e		:�擾															 */
/* ���t		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::GetList(CLibInfoEfcBalloon *pDst)
{
	if (m_pLibInfoEfcBalloon == NULL) {
		return;
	}

	pDst->SetList (m_dwListID, m_pLibInfoEfcBalloon);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloon::SetList									 */
/* ���e		:�ݒ�															 */
/* ���t		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::SetList(CLibInfoEfcBalloon *pSrc)
{
	if (m_pLibInfoEfcBalloon == NULL) {
		return;
	}

	m_pLibInfoEfcBalloon->SetList (m_dwListID, pSrc);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloon::OnInitDialog								 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2007/12/25														 */
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

	m_cmbSoundID.InsertString (0, "����");
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
/* �֐���	:CDlgAdminEfcBalloon::PostNcDestroy								 */
/* ���e		:�I������														 */
/* ���t		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloon::OnLButtonDown								 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2007/12/25														 */
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
/* �֐���	:CDlgAdminEfcBalloon::OnWndClose								 */
/* ���e		:���b�Z�[�W�n���h��(WM_WNDCLOSE)								 */
/* ���t		:2007/12/25														 */
/* ========================================================================= */

LRESULT CDlgAdminEfcBalloon::OnWndClose(WPARAM wParam, LPARAM lParam)
{
	int nHi, nLow;

	nHi  = HIWORD (lParam);
	nLow = LOWORD (lParam);

	switch (wParam) {
	case WINDOWID_SELECTMAPPARTSGRP:	/* �摜�I�� */
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
/* �֐���	:CDlgAdminEfcBalloon::OnHScroll									 */
/* ���e		:���b�Z�[�W�n���h��(WM_HSCROLL)									 */
/* ���t		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	OnSelectAnime ();

	CDlgAdminBase::OnHScroll (nSBCode, nPos, pScrollBar);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloon::OnTimer									 */
/* ���e		:���b�Z�[�W�n���h��(WM_TIMER)									 */
/* ���t		:2007/12/25														 */
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
/* �֐���	:CDlgAdminEfcBalloon::OnAdd										 */
/* ���e		:�{�^���n���h��(�ǉ�)											 */
/* ���t		:2007/12/25														 */
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
/* �֐���	:CDlgAdminEfcBalloon::OnDel										 */
/* ���e		:�{�^���n���h��(�폜)											 */
/* ���t		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::OnDel()
{
	int nResult, nCount;

	nResult = MessageBox ("���̃p�^�[�����폜���܂����H", "�m�F", MB_YESNO | MB_ICONQUESTION);
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
/* �֐���	:CDlgAdminEfcBalloon::OnPlay									 */
/* ���e		:�{�^���n���h��(�Đ�)											 */
/* ���t		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::OnPlay()
{
	Set (m_nSelect);

	SetTimer (100, 10, NULL);
	m_dwLastTimeAnime = timeGetTime ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloon::OnStop									 */
/* ���e		:�{�^���n���h��(��~)											 */
/* ���t		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::OnStop()
{
	KillTimer (100);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloon::OnOK										 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::OnOK()
{
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	Set (m_nSelect);

	if (m_dwListID == 0) {
		MessageBox ("���ID��ݒ肵�Ă�������", "�m�F", MB_ICONINFORMATION | MB_OK);
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
/* �֐���	:CDlgAdminEfcBalloon::RenewSlider								 */
/* ���e		:�X���C�_�[���X�V												 */
/* ���t		:2007/12/25														 */
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
/* �֐���	:CDlgAdminEfcBalloon::OnSelectAnime								 */
/* ���e		:�R�}�I������													 */
/* ���t		:2007/12/25														 */
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
/* �֐���	:CDlgAdminEfcBalloon::Enable									 */
/* ���e		:���͋֎~�ݒ�													 */
/* ���t		:2007/12/25														 */
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
/* �֐���	:CDlgAdminEfcBalloon::Set										 */
/* ���e		:���o�����֐ݒ�												 */
/* ���t		:2007/12/25														 */
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
	pInfo->m_byWait		= (BYTE)m_nViewTime;			/* �҂�����(�~�P�O�~���b) */
	pInfo->m_bLoop		= m_bLoop;						/* ���[�v�Đ� */
	pInfo->m_dwListID	= m_dwListID;					/* ���ID */
	pInfo->m_dwGrpID	= m_ctlGrp.GetIDSub ();			/* �O���t�B�b�NID */
	pInfo->m_strName	= m_strName;					/* ���o���� */
	pInfo->m_dwAnimeID	= nNo;							/* �R�}�ԍ� */

	nSelect = m_cmbSoundID.GetCurSel ();
	pInfo->m_dwSoundID = m_cmbSoundID.GetItemData (nSelect);	/* ���ʉ�ID */
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloon::Get										 */
/* ���e		:���o����񂩂�擾												 */
/* ���t		:2007/12/25														 */
/* ========================================================================= */

void CDlgAdminEfcBalloon::Get(int nNo)
{
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	pInfo = (PCInfoEfcBalloon)m_pLibInfoEfcBalloon->GetPtr (nNo);
	if (pInfo == NULL) {
		return;
	}

	m_nViewTime	= pInfo->m_byWait;			/* �҂�����(�~�P�O�~���b) */
	m_dwListID	= pInfo->m_dwListID;		/* ���ID */
	m_bLoop		= pInfo->m_bLoop;			/* ���[�v�Đ� */
	m_ctlGrp.Init (GRPIDMAIN_EFCBALLOON);	/* �O���t�B�b�NID���C�� */
	m_ctlGrp.Set (pInfo->m_dwGrpID);		/* �O���t�B�b�NID�T�u */
	if (nNo == 0) {
		m_strName = pInfo->m_strName;		/* ���o���� */
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
