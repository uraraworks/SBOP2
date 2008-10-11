/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminEfcEffect.cpp										 */
/* ���e			:�G�t�F�N�g���ݒ�_�C�A���O�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/06													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoEffect.h"
#include "LibSboSoundLoader.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WndSelectGrp.h"
#include "DlgAdminEfcEffect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminEfcEffect::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminEfcEffect)
	DDX_Control(pDX, IDC_SLIDER, m_ctlSlider);
	DDX_Control(pDX, IDC_SOUNDID, m_cmbSoundID);
	DDX_Control(pDX, IDC_TYPE, m_cmbType);
	DDX_Control(pDX, IDC_GRP, m_ctlGrp);
	DDX_Check(pDX, IDC_LOOP, m_bLoop);
	DDX_Check(pDX, IDC_LOOPSOUND, m_bLoopSound);
	DDX_Text(pDX, IDC_VIEWTIME, m_nViewTime);
	DDX_Text(pDX, IDC_LEVEL, m_nLevel);
	DDX_Text(pDX, IDC_COUNT, m_strCount);
	DDX_Text(pDX, IDC_NOW, m_strNow);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminEfcEffect, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminEfcEffect)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WNDCLOSE, OnWndClose)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::CDlgAdminEfcEffect							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

CDlgAdminEfcEffect::CDlgAdminEfcEffect(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminEfcEffect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminEfcEffect)
	m_bLoop = FALSE;
	m_bLoopSound = FALSE;
	m_nViewTime = 0;
	m_nLevel = 0;
	m_strCount = _T("");
	m_strNow = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT

	m_bModify			= FALSE;
	m_nSelect			= -1;
	m_dwLastTimeAnime	= 0;
	m_pWndSelectGrp		= NULL;
	m_pInfoEffect		= NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::~CDlgAdminEfcEffect						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

CDlgAdminEfcEffect::~CDlgAdminEfcEffect()
{
	SAFE_DELETE (m_pInfoEffect);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::Init										 */
/* ���e		:������															 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::Init(CMgrData *pMgrData, BOOL bModify)
{
	CDlgAdminBase::Init (pMgrData);

	m_bModify = bModify;
	m_pInfoEffect = new CInfoEffect;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::GetInfo									 */
/* ���e		:�擾															 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::GetInfo(CInfoEffect *pDst)
{
	if (m_pInfoEffect == NULL) {
		return;
	}

	pDst->Copy (m_pInfoEffect);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::SetInfo									 */
/* ���e		:�ݒ�															 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::SetInfo(CInfoEffect *pSrc)
{
	if (m_pInfoEffect == NULL) {
		return;
	}

	m_pInfoEffect->Copy (pSrc);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::OnInitDialog								 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

BOOL CDlgAdminEfcEffect::OnInitDialog()
{
	int i, nCount;
	DWORD dwSoundID;
	LPCSTR pszTmp;
	CLibSboSoundLoader LibSboSoundLoader;

	CDlgAdminBase::OnInitDialog();

	m_ctlGrp.Create (this, m_pMgrData);
	m_ctlGrp.Init (m_pInfoEffect->m_dwGrpIDMain);

	m_cmbType.InsertString (0, "�G�t�F�N�g(32)");
	m_cmbType.SetItemData (0, GRPIDMAIN_EFFECT32);
	m_cmbType.InsertString (1, "�G�t�F�N�g(64)");
	m_cmbType.SetItemData (1, GRPIDMAIN_EFFECT64);
	m_cmbType.SetCurSel (0);

	m_cmbSoundID.InsertString (0, "����");
	m_cmbSoundID.SetItemData (0, 0);
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
//	nCount = m_ctlSlider.GetRangeMax ();
//	if (nCount <= 0) {
//		OnAdd ();
//	}

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::PostNcDestroy								 */
/* ���e		:�I������														 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::OnLButtonDown								 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::OnLButtonDown(UINT nFlags, CPoint point)
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
	m_pWndSelectGrp->SetType (m_ctlGrp.GetIDMain ());
	m_pWndSelectGrp->ShowWindow (SW_SHOW);

Exit:
	CDlgAdminBase::OnLButtonDown (nFlags, point);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::OnWndClose									 */
/* ���e		:���b�Z�[�W�n���h��(WM_WNDCLOSE)								 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

LRESULT CDlgAdminEfcEffect::OnWndClose(WPARAM wParam, LPARAM lParam)
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
/* �֐���	:CDlgAdminEfcEffect::OnHScroll									 */
/* ���e		:���b�Z�[�W�n���h��(WM_HSCROLL)									 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	OnSelectAnime ();

	CDlgAdminBase::OnHScroll (nSBCode, nPos, pScrollBar);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::OnTimer									 */
/* ���e		:���b�Z�[�W�n���h��(WM_TIMER)									 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::OnTimer(UINT nIDEvent)
{
	int nMax, nSelect;
	DWORD dwTime, dwTmp;
	PCInfoAnime pInfo;

	nMax	= m_ctlSlider.GetRangeMax ();
	nSelect	= m_nSelect;

	pInfo = m_pInfoEffect->GetAnimePtr (nSelect);
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
/* �֐���	:CDlgAdminEfcEffect::OnSelchangeType							 */
/* ���e		:���ڑI���n���h��(�摜���)										 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::OnSelchangeType()
{
	int nNo;
	DWORD dwID;

	nNo  = m_cmbType.GetCurSel ();
	dwID = m_cmbType.GetItemData (nNo);
	m_ctlGrp.Init (dwID);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::OnAdd										 */
/* ���e		:�{�^���n���h��(�ǉ�)											 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::OnAdd()
{
	m_pInfoEffect->AddAnime ();
	if (m_nSelect == -1) {
		m_nSelect = 0;
	}

	Get (m_nSelect);
	RenewSlider ();
	OnSelectAnime ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::OnDel										 */
/* ���e		:�{�^���n���h��(�폜)											 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::OnDel()
{
	int nResult, nCount;

	nResult = MessageBox ("���̃p�^�[�����폜���܂����H", "�m�F", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	m_pInfoEffect->DeleteAnime (m_nSelect);
	nCount = m_pInfoEffect->GetAnimeCount ();
	if (m_nSelect >= nCount) {
		m_nSelect = nCount - 1;
	}
	if (nCount <= 1) {
		m_ctlSlider.SetPos (0);
	}

	Get (m_nSelect);
	RenewSlider ();
	OnSelectAnime ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::OnPlay										 */
/* ���e		:�{�^���n���h��(�Đ�)											 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::OnPlay()
{
	Set (m_nSelect);

	SetTimer (100, 10, NULL);
	m_dwLastTimeAnime = timeGetTime ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::OnStop										 */
/* ���e		:�{�^���n���h��(��~)											 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::OnStop()
{
	KillTimer (100);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::OnOK										 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::OnOK()
{
//	int i, nCount;
//	PCInfoEffect pInfo;

	Set (m_nSelect);

//	nCount = m_pInfoEffect->GetCount ();
//	for (i = 0; i < nCount; i ++) {
//		pInfo = (PCInfoEffect)m_pInfoEffect->GetPtr (i);
//		pInfo->m_dwListID = m_dwListID;
//	}

	CDlgAdminBase::OnOK ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::RenewSlider								 */
/* ���e		:�X���C�_�[���X�V												 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::RenewSlider(void)
{
	BOOL bEnable;
	int nMax, nCount;

	nCount	= m_pInfoEffect->GetAnimeCount ();
	nMax	= max (nCount - 1, 0);
	m_ctlSlider.SetRange (0, nMax);
	m_ctlSlider.RedrawWindow ();

	OnSelectAnime ();

	bEnable = TRUE;
	if (nCount == 0) {
		bEnable = FALSE;
	}
	Enable (bEnable);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::OnSelectAnime								 */
/* ���e		:�R�}�I������													 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::OnSelectAnime(void)
{
	int nCount;

	Set (m_nSelect);
	m_nSelect = m_ctlSlider.GetPos ();

	nCount = m_pInfoEffect->GetAnimeCount ();
	m_strCount.Format ("%d", nCount);
	m_strNow.Empty ();
	if (nCount > 0) {
		m_strNow.Format ("%d", m_nSelect + 1);
	}
	UpdateData (FALSE);

	Get (m_nSelect);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::Enable										 */
/* ���e		:���͋֎~�ݒ�													 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::Enable(BOOL bEnable)
{
	int i, nResourceID,
		anResourceID[] = {
			IDC_VIEWTIME,
			IDC_DEL,
			IDC_PLAY,
			IDC_STOP,
			IDC_LOOP,
			IDC_LOOPSOUND,
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
/* �֐���	:CDlgAdminEfcEffect::Set										 */
/* ���e		:�G�t�F�N�g���֐ݒ�											 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::Set(int nNo)
{
	int nSelect;
	PCInfoAnime pInfo;

	pInfo = m_pInfoEffect->GetAnimePtr (nNo);
	if (pInfo == NULL) {
		return;
	}

	UpdateData ();
	pInfo->m_byWait		= (BYTE)m_nViewTime;			/* �҂�����(�~�P�O�~���b) */
	pInfo->m_byLevel	= (BYTE)m_nLevel;				/* �����x */
	pInfo->m_wGrpIDBase	= (WORD)m_ctlGrp.GetIDSub ();	/* �O���t�B�b�NID */

	m_pInfoEffect->m_dwGrpIDMain	= m_ctlGrp.GetIDMain ();	/* �O���t�B�b�NID���C�� */
	m_pInfoEffect->m_bLoop			= m_bLoop;					/* ���[�v�Đ� */
	m_pInfoEffect->m_bLoopSound		= m_bLoopSound;				/* ���[�v���Ɍ��ʉ����Đ����� */
	m_pInfoEffect->m_strName		= m_strName;				/* �G�t�F�N�g�� */

	nSelect = m_cmbSoundID.GetCurSel ();
	m_pInfoEffect->m_dwSoundID = m_cmbSoundID.GetItemData (nSelect);	/* ���ʉ�ID */
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffect::Get										 */
/* ���e		:�G�t�F�N�g��񂩂�擾											 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffect::Get(int nNo)
{
	int i, nCount;
	PCInfoAnime pInfo;

	pInfo = m_pInfoEffect->GetAnimePtr (nNo);
	if (pInfo == NULL) {
		return;
	}

	m_nViewTime	= pInfo->m_byWait;					/* �҂�����(�~�P�O�~���b) */
	m_nLevel	= pInfo->m_byLevel;					/* �����x */
	m_ctlGrp.Init (m_pInfoEffect->m_dwGrpIDMain);	/* �O���t�B�b�NID���C�� */
	m_ctlGrp.Set (pInfo->m_wGrpIDBase);				/* �O���t�B�b�NID�T�u */

	m_bLoop			= m_pInfoEffect->m_bLoop;		/* ���[�v�Đ� */
	m_bLoopSound	= m_pInfoEffect->m_bLoopSound;	/* ���[�v���Ɍ��ʉ����Đ����� */
	m_strName		= m_pInfoEffect->m_strName;		/* �G�t�F�N�g�� */

	nCount = m_cmbSoundID.GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (m_pInfoEffect->m_dwSoundID != m_cmbSoundID.GetItemData (i)) {
			continue;
		}
		m_cmbSoundID.SetCurSel (i);
		break;
	}

	UpdateData (FALSE);
}

/* Copyright(C)URARA-works 2008 */
