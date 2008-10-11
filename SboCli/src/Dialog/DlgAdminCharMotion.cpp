/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharMotion.cpp										 */
/* ���e			:�L�������[�V�����ݒ�_�C�A���O�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/28													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "LibInfoMotion.h"
#include "LibSboSoundLoader.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WndSelectGrp.h"
#include "DlgAdminCharMotion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharMotion::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharMotion)
	DDX_Control(pDX, IDC_SLIDER, m_ctlSlider);
	DDX_Control(pDX, IDC_PROCID, m_cmbProcID);
	DDX_Control(pDX, IDC_SOUNDID, m_cmbSoundID);
	DDX_Control(pDX, IDC_TYPE_PILE3, m_cmbTypePile3);
	DDX_Control(pDX, IDC_TYPE_PILE2, m_cmbTypePile2);
	DDX_Control(pDX, IDC_TYPE_PILE1, m_cmbTypePile1);
	DDX_Control(pDX, IDC_TYPE_CHAR, m_cmbTypeChar);
	DDX_Control(pDX, IDC_GRP_PREVIEW, m_ctlGrpPreview);
	DDX_Control(pDX, IDC_GRP_PILE3, m_ctlGrpPile3);
	DDX_Control(pDX, IDC_GRP_PILE2, m_ctlGrpPile2);
	DDX_Control(pDX, IDC_GRP_PILE1, m_ctlGrpPile1);
	DDX_Control(pDX, IDC_GRP_CHAR, m_ctlGrpChar);
	DDX_Check(pDX, IDC_LOOP, m_bLoop);
	DDX_Check(pDX, IDC_PILE, m_bPile);
	DDX_Check(pDX, IDC_REDRAWHAND, m_bRedrawHand);
	DDX_Text(pDX, IDC_VIEWTIME, m_nViewTime);
	DDX_Text(pDX, IDC_LEVEL1, m_nLevel1);
	DDX_Text(pDX, IDC_LEVEL2, m_nLevel2);
	DDX_Text(pDX, IDC_LEVEL3, m_nLevel3);
	DDX_Text(pDX, IDC_COUNT, m_strCount);
	DDX_Text(pDX, IDC_NOW, m_strNow);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SPIN_X0, m_ctlSpinX0);
	DDX_Control(pDX, IDC_SPIN_Y0, m_ctlSpinY0);
	DDX_Control(pDX, IDC_SPIN_X1, m_ctlSpinX1);
	DDX_Control(pDX, IDC_SPIN_Y1, m_ctlSpinY1);
	DDX_Control(pDX, IDC_SPIN_X2, m_ctlSpinX2);
	DDX_Control(pDX, IDC_SPIN_Y2, m_ctlSpinY2);
	DDX_Control(pDX, IDC_SPIN_X3, m_ctlSpinX3);
	DDX_Control(pDX, IDC_SPIN_Y3, m_ctlSpinY3);
	DDX_Text(pDX, IDC_VIEWPOS_X0, m_nViewPosX0);
	DDX_Text(pDX, IDC_VIEWPOS_Y0, m_nViewPosY0);
	DDX_Text(pDX, IDC_VIEWPOS_X1, m_nViewPosX1);
	DDX_Text(pDX, IDC_VIEWPOS_Y1, m_nViewPosY1);
	DDX_Text(pDX, IDC_VIEWPOS_X2, m_nViewPosX2);
	DDX_Text(pDX, IDC_VIEWPOS_Y2, m_nViewPosY2);
	DDX_Text(pDX, IDC_VIEWPOS_X3, m_nViewPosX3);
	DDX_Text(pDX, IDC_VIEWPOS_Y3, m_nViewPosY3);
	DDX_Control(pDX, IDC_DRAWLIST, m_ctlDrawList);
}

BEGIN_MESSAGE_MAP(CDlgAdminCharMotion, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharMotion)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_TYPE_CHAR, OnSelchangeTypeChar)
	ON_CBN_SELCHANGE(IDC_TYPE_PILE1, OnSelchangeTypePile1)
	ON_CBN_SELCHANGE(IDC_TYPE_PILE2, OnSelchangeTypePile2)
	ON_CBN_SELCHANGE(IDC_TYPE_PILE3, OnSelchangeTypePile3)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WNDCLOSE, OnWndClose)
	ON_EN_CHANGE(IDC_VIEWPOS_X0, &CDlgAdminCharMotion::OnEnChangeViewpos)
	ON_EN_CHANGE(IDC_VIEWPOS_Y0, &CDlgAdminCharMotion::OnEnChangeViewpos)
	ON_EN_CHANGE(IDC_VIEWPOS_X1, &CDlgAdminCharMotion::OnEnChangeViewpos)
	ON_EN_CHANGE(IDC_VIEWPOS_Y1, &CDlgAdminCharMotion::OnEnChangeViewpos)
	ON_EN_CHANGE(IDC_VIEWPOS_X2, &CDlgAdminCharMotion::OnEnChangeViewpos)
	ON_EN_CHANGE(IDC_VIEWPOS_Y2, &CDlgAdminCharMotion::OnEnChangeViewpos)
	ON_EN_CHANGE(IDC_VIEWPOS_X3, &CDlgAdminCharMotion::OnEnChangeViewpos)
	ON_EN_CHANGE(IDC_VIEWPOS_Y3, &CDlgAdminCharMotion::OnEnChangeViewpos)
	ON_BN_CLICKED(IDC_DRAW_UP, &CDlgAdminCharMotion::OnBnClickedDrawUp)
	ON_BN_CLICKED(IDC_DRAW_DOWN, &CDlgAdminCharMotion::OnBnClickedDrawDown)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::CDlgAdminCharMotion						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/10/28														 */
/* ========================================================================= */

CDlgAdminCharMotion::CDlgAdminCharMotion(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharMotion::IDD, pParent)
	, m_nViewPosX0(0)
	, m_nViewPosY0(0)
	, m_nViewPosX1(0)
	, m_nViewPosY1(0)
	, m_nViewPosX2(0)
	, m_nViewPosY2(0)
	, m_nViewPosX3(0)
	, m_nViewPosY3(0)
{
	//{{AFX_DATA_INIT(CDlgAdminCharMotion)
	m_bLoop = FALSE;
	m_bPile = FALSE;
	m_bRedrawHand = FALSE;
	m_nViewTime = 0;
	m_nLevel1 = 0;
	m_nLevel2 = 0;
	m_nLevel3 = 0;
	m_strCount = _T("");
	m_strNow = _T("");
	//}}AFX_DATA_INIT

	m_nSelect			= -1;
	m_nGrpIDSub			= 0;
	m_dwMotionTypeID	= 0;
	m_dwMotionListID	= 0;
	m_dwLastTimeAnime	= 0;
	m_pWndSelectGrp		= NULL;
	m_pLibInfoMotion	= NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::~CDlgAdminCharMotion						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/10/28														 */
/* ========================================================================= */

CDlgAdminCharMotion::~CDlgAdminCharMotion()
{
	SAFE_DELETE (m_pLibInfoMotion);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::Init										 */
/* ���e		:������															 */
/* ���t		:2007/08/23														 */
/* ========================================================================= */

void CDlgAdminCharMotion::Init(CMgrData *pMgrData, DWORD dwMotionTypeID, DWORD dwMotionListID, int nGrpIDSub)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoMotion = new CLibInfoMotion;
	m_pLibInfoMotion->Create ();

	m_dwMotionTypeID	= dwMotionTypeID;
	m_dwMotionListID	= dwMotionListID;
	m_nGrpIDSub			= nGrpIDSub;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::GetList									 */
/* ���e		:�擾															 */
/* ���t		:2007/11/23														 */
/* ========================================================================= */

void CDlgAdminCharMotion::GetList(CLibInfoMotion *pDst)
{
	if (m_pLibInfoMotion == NULL) {
		return;
	}

	pDst->SetList (m_dwMotionTypeID, m_dwMotionListID, m_pLibInfoMotion);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::SetList									 */
/* ���e		:�ݒ�															 */
/* ���t		:2007/11/23														 */
/* ========================================================================= */

void CDlgAdminCharMotion::SetList(CLibInfoMotion *pSrc)
{
	if (m_pLibInfoMotion == NULL) {
		return;
	}

	m_pLibInfoMotion->SetList (m_dwMotionTypeID, m_dwMotionListID, pSrc);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnInitDialog								 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2007/10/28														 */
/* ========================================================================= */

BOOL CDlgAdminCharMotion::OnInitDialog()
{
	int i, nCount;
	DWORD dwSoundID;
	LPCSTR pszTmp;
	CLibSboSoundLoader LibSboSoundLoader;

	CDlgAdminBase::OnInitDialog();

	m_ctlGrpChar.	Create (this, m_pMgrData);
	m_ctlGrpPile1.	Create (this, m_pMgrData);
	m_ctlGrpPile2.	Create (this, m_pMgrData);
	m_ctlGrpPile3.	Create (this, m_pMgrData);
	m_ctlGrpPreview.Create (this, m_pMgrData);

	m_ctlGrpChar.Set (1);

	m_cmbTypeChar.InsertString (0, "�W��(16x16)");
	m_cmbTypeChar.SetItemData (0, GRPIDMAIN_CHAR);
	m_cmbTypeChar.InsertString (1, "NPC(16x16)");
	m_cmbTypeChar.SetItemData (1, GRPIDMAIN_NPC);
	m_cmbTypeChar.InsertString (2, "�W��(32x32)");
	m_cmbTypeChar.SetItemData (2, GRPIDMAIN_2X2_CHAR);
	m_cmbTypeChar.InsertString (3, "NPC(32x32)");
	m_cmbTypeChar.SetItemData (3, GRPIDMAIN_2X2_NPC);
	m_cmbTypePile1.InsertString (0, "����");
	m_cmbTypePile1.SetItemData (0, 0);
	m_cmbTypePile1.InsertString (1, "����");
	m_cmbTypePile1.SetItemData (1, GRPIDMAIN_WEAPON);
	m_cmbTypePile1.InsertString (2, "�|");
	m_cmbTypePile1.SetItemData (2, GRPIDMAIN_WEAPON_BOW);
	m_cmbTypePile1.InsertString (3, "�G�t�F�N�g(32)");
	m_cmbTypePile1.SetItemData (3, GRPIDMAIN_EFFECT32);
	m_cmbTypePile1.InsertString (4, "�G�t�F�N�g(64)");
	m_cmbTypePile1.SetItemData (4, GRPIDMAIN_EFFECT64);
	m_cmbTypePile1.InsertString (5, "�Ō�");
	m_cmbTypePile1.SetItemData (5, GRPIDMAIN_WEAPON_GLOVE);
	m_cmbTypePile1.InsertString (6, "���̑�");
	m_cmbTypePile1.SetItemData (6, GRPIDMAIN_WEAPON_ETC);
	m_cmbTypePile1.InsertString (7, "������(2x2)");
	m_cmbTypePile1.SetItemData (7, GRPIDMAIN_2X2_ARMS);
	m_cmbTypePile1.InsertString (8, "���ꎝ����(2x2)");
	m_cmbTypePile1.SetItemData (8, GRPIDMAIN_2X2_ARMSSP);
	m_cmbTypePile1.InsertString (9, "�|(2x2)");
	m_cmbTypePile1.SetItemData (9, GRPIDMAIN_2X2_BOW);
	m_cmbTypePile2.InsertString (0, "����");
	m_cmbTypePile2.SetItemData (0, 0);
	m_cmbTypePile2.InsertString (1, "����");
	m_cmbTypePile2.SetItemData (1, GRPIDMAIN_WEAPON);
	m_cmbTypePile2.InsertString (2, "�|");
	m_cmbTypePile2.SetItemData (2, GRPIDMAIN_WEAPON_BOW);
	m_cmbTypePile2.InsertString (3, "�G�t�F�N�g(32)");
	m_cmbTypePile2.SetItemData (3, GRPIDMAIN_EFFECT32);
	m_cmbTypePile2.InsertString (4, "�G�t�F�N�g(64)");
	m_cmbTypePile2.SetItemData (4, GRPIDMAIN_EFFECT64);
	m_cmbTypePile2.InsertString (5, "�Ō�");
	m_cmbTypePile2.SetItemData (5, GRPIDMAIN_WEAPON_GLOVE);
	m_cmbTypePile2.InsertString (6, "���̑�");
	m_cmbTypePile2.SetItemData (6, GRPIDMAIN_WEAPON_ETC);
	m_cmbTypePile2.InsertString (7, "������(2x2)");
	m_cmbTypePile2.SetItemData (7, GRPIDMAIN_2X2_ARMS);
	m_cmbTypePile2.InsertString (8, "��(2x2)");
	m_cmbTypePile2.SetItemData (8, GRPIDMAIN_2X2_SHIELD);
	m_cmbTypePile2.InsertString (9, "���ꎝ����(2x2)");
	m_cmbTypePile2.SetItemData (9, GRPIDMAIN_2X2_ARMSSP);
	m_cmbTypePile2.InsertString (10, "�|(2x2)");
	m_cmbTypePile2.SetItemData (10, GRPIDMAIN_2X2_BOW);
	m_cmbTypePile3.InsertString (0, "����");
	m_cmbTypePile3.SetItemData (0, 0);
	m_cmbTypePile3.InsertString (1, "����");
	m_cmbTypePile3.SetItemData (1, GRPIDMAIN_WEAPON);
	m_cmbTypePile3.InsertString (2, "�|");
	m_cmbTypePile3.SetItemData (2, GRPIDMAIN_WEAPON_BOW);
	m_cmbTypePile3.InsertString (3, "�G�t�F�N�g(32)");
	m_cmbTypePile3.SetItemData (3, GRPIDMAIN_EFFECT32);
	m_cmbTypePile3.InsertString (4, "�G�t�F�N�g(64)");
	m_cmbTypePile3.SetItemData (4, GRPIDMAIN_EFFECT64);
	m_cmbTypePile3.InsertString (5, "�Ō�");
	m_cmbTypePile3.SetItemData (5, GRPIDMAIN_WEAPON_GLOVE);
	m_cmbTypePile3.InsertString (6, "���̑�");
	m_cmbTypePile3.SetItemData (6, GRPIDMAIN_WEAPON_ETC);
	m_cmbTypePile3.InsertString (7, "������(2x2)");
	m_cmbTypePile3.SetItemData (7, GRPIDMAIN_2X2_ARMS);
	m_cmbTypePile3.InsertString (8, "��(2x2)");
	m_cmbTypePile3.SetItemData (8, GRPIDMAIN_2X2_SHIELD);
	m_cmbTypePile3.InsertString (9, "���ꎝ����(2x2)");
	m_cmbTypePile3.SetItemData (9, GRPIDMAIN_2X2_ARMSSP);
	m_cmbTypePile3.InsertString (10, "�|(2x2)");
	m_cmbTypePile3.SetItemData (10, GRPIDMAIN_2X2_BOW);
	m_ctlSpinX0.SetRange (-64, 64);
	m_ctlSpinY0.SetRange (-64, 64);
	m_ctlSpinX1.SetRange (-64, 64);
	m_ctlSpinY1.SetRange (-64, 64);
	m_ctlSpinX2.SetRange (-64, 64);
	m_ctlSpinY2.SetRange (-64, 64);
	m_ctlSpinX3.SetRange (-64, 64);
	m_ctlSpinY3.SetRange (-64, 64);

	m_cmbSoundID.InsertString (0, "����");
	nCount = LibSboSoundLoader.GetSoundCount ();
	for (i = 0; i < nCount; i ++) {
		dwSoundID	= LibSboSoundLoader.GetSoundID (i);
		pszTmp		= LibSboSoundLoader.GetSoundName (dwSoundID);
		m_cmbSoundID.InsertString (i + 1, pszTmp);
		m_cmbSoundID.SetItemData (i + 1, dwSoundID);
	}

	for (i = 0; i < CHARMOTIONPROCID_MAX; i ++) {
		pszTmp = m_pMgrData->GetMotionProcName (i);
		m_cmbProcID.InsertString (i, pszTmp);
		m_cmbProcID.SetItemData (i, i);
	}

	m_cmbTypeChar.SetCurSel (0);
	m_cmbTypePile1.SetCurSel (0);
	m_cmbTypePile2.SetCurSel (0);
	m_cmbTypePile3.SetCurSel (0);
	m_cmbSoundID.SetCurSel (0);
	m_cmbProcID.SetCurSel (0);

	OnSelchangeTypeChar ();
	OnSelchangeTypePile1 ();
	OnSelchangeTypePile2 ();
	OnSelchangeTypePile3 ();
	RenewSlider ();

	Get (0);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::PostNcDestroy								 */
/* ���e		:�I������														 */
/* ���t		:2007/10/28														 */
/* ========================================================================= */

void CDlgAdminCharMotion::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnLButtonDown								 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2007/10/30														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bResult;
	DWORD dwID;
	CRect rc;
	CPoint ptTmp;
	int i, nResourceID, anResourceID[] = {IDC_GRP_CHAR, IDC_GRP_PILE1, IDC_GRP_PILE2, IDC_GRP_PILE3, -1};
	CComboBox *apCmb[] = {&m_cmbTypeChar, &m_cmbTypePile1, &m_cmbTypePile2, &m_cmbTypePile3};

	if (m_nSelect < 0) {
		goto Exit;
	}
	if (m_pWndSelectGrp) {
		goto Exit;
	}

	ptTmp = point;
	ClientToScreen (&ptTmp);

	for (i = 0; ; i ++) {
		nResourceID = anResourceID[i];
		if (nResourceID < 0) {
			break;
		}
		GetDlgItem (nResourceID)->GetWindowRect (rc);
		bResult = rc.PtInRect (ptTmp);
		if (bResult == FALSE) {
			continue;
		}
		dwID = apCmb[i]->GetItemData (apCmb[i]->GetCurSel ());
		m_pWndSelectGrp = new CWndSelectGrp;
		m_pWndSelectGrp->Create (this, m_pMgrData, i);
		m_pWndSelectGrp->SetType (dwID, m_nGrpIDSub);
		m_pWndSelectGrp->ShowWindow (SW_SHOW);
		break;
	}

Exit:
	CDlgAdminBase::OnLButtonDown (nFlags, point);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnWndClose								 */
/* ���e		:���b�Z�[�W�n���h��(WM_WNDCLOSE)								 */
/* ���t		:2007/11/04														 */
/* ========================================================================= */

LRESULT CDlgAdminCharMotion::OnWndClose(WPARAM wParam, LPARAM lParam)
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
		switch (nHi) {
		case 0:		/* �L�����摜 */
			m_ctlGrpChar.Set (nLow);
			break;
		case 1:		/* �d�ˉ摜�P */
			m_ctlGrpPile1.Set (nLow);
			break;
		case 2:		/* �d�ˉ摜�Q */
			m_ctlGrpPile2.Set (nLow);
			break;
		case 3:		/* �d�ˉ摜�R */
			m_ctlGrpPile3.Set (nLow);
			break;
		}
		Set (m_nSelect);
		RenewPreview ();
		break;
	}

	return -1;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnHScroll									 */
/* ���e		:���b�Z�[�W�n���h��(WM_HSCROLL)									 */
/* ���t		:2007/11/14														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	OnSelectAnime ();

	CDlgAdminBase::OnHScroll (nSBCode, nPos, pScrollBar);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnTimer									 */
/* ���e		:���b�Z�[�W�n���h��(WM_TIMER)									 */
/* ���t		:2007/11/17														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnTimer(UINT nIDEvent)
{
	int nMax, nSelect;
	DWORD dwTime, dwTmp;
	PCInfoMotion pInfo;

	nMax	= m_ctlSlider.GetRangeMax ();
	nSelect	= m_nSelect;

	pInfo = (PCInfoMotion)m_pLibInfoMotion->GetPtr (nSelect);
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
/* �֐���	:CDlgAdminCharMotion::OnAdd										 */
/* ���e		:�{�^���n���h��(�ǉ�)											 */
/* ���t		:2007/11/14														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnAdd()
{
	PCInfoMotion pInfo;

	pInfo = (PCInfoMotion)m_pLibInfoMotion->GetNew ();
	pInfo->m_dwMotionTypeID	= m_dwMotionTypeID;
	pInfo->m_dwMotionListID = m_dwMotionListID;
	pInfo->m_wGrpIDMainBase = GRPIDMAIN_CHAR;

	m_pLibInfoMotion->Add (pInfo, m_nSelect);
	if (m_nSelect == -1) {
		m_nSelect = 0;
	}
	Get (m_nSelect);

	RenewSlider ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnDel										 */
/* ���e		:�{�^���n���h��(�폜)											 */
/* ���t		:2007/11/14														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnDel()
{
	int nResult;

	nResult = MessageBox ("���̃p�^�[�����폜���܂����H", "�m�F", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	m_pLibInfoMotion->Delete (m_nSelect);
	if (m_nSelect >= m_pLibInfoMotion->GetCount ()) {
		m_nSelect = m_pLibInfoMotion->GetCount () - 1;
	}
	Get (m_nSelect);

	RenewSlider ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnPlay									 */
/* ���e		:�{�^���n���h��(�Đ�)											 */
/* ���t		:2007/11/14														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnPlay()
{
	Set (m_nSelect);

	SetTimer (100, 10, NULL);
	m_dwLastTimeAnime = timeGetTime ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnStop									 */
/* ���e		:�{�^���n���h��(��~)											 */
/* ���t		:2007/11/14														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnStop()
{
	KillTimer (100);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnBnClickedDrawUp							 */
/* ���e		:�{�^���n���h��(��)												 */
/* ���t		:2008/07/02														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnBnClickedDrawUp()
{
	int nResult;
	CString strTmp;

	nResult = m_ctlDrawList.GetCurSel ();
	if (nResult <= 0) {
		return;
	}
	m_ctlDrawList.GetText (nResult - 1, strTmp);
	m_ctlDrawList.InsertString (nResult + 1, strTmp);
	m_ctlDrawList.SetItemData (nResult + 1, m_ctlDrawList.GetItemData (nResult - 1));
	m_ctlDrawList.DeleteString (nResult - 1);

	RenewPreview ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnBnClickedDrawDown						 */
/* ���e		:�{�^���n���h��(��)												 */
/* ���t		:2008/07/02														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnBnClickedDrawDown()
{
	int nResult;
	CString strTmp;

	nResult = m_ctlDrawList.GetCurSel ();
	if (nResult >= m_ctlDrawList.GetCount () - 1) {
		return;
	}
	m_ctlDrawList.GetText (nResult, strTmp);
	m_ctlDrawList.InsertString (nResult + 2, strTmp);
	m_ctlDrawList.SetItemData (nResult + 2, m_ctlDrawList.GetItemData (nResult));
	m_ctlDrawList.DeleteString (nResult);
	m_ctlDrawList.SetCurSel (nResult + 1);

	RenewPreview ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnOK										 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2007/11/14														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnOK()
{
	Set (m_nSelect);

	CDlgAdminBase::OnOK ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnSelchangeTypeChar						 */
/* ���e		:���ڑI���n���h��(�L�����摜)									 */
/* ���t		:2007/11/14														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnSelchangeTypeChar()
{
	int nNo;
	DWORD dwID;

	dwID = 0;
	if (m_nSelect >= 0) {
		nNo  = m_cmbTypeChar.GetCurSel ();
		dwID = m_cmbTypeChar.GetItemData (nNo);
	}
	m_ctlGrpChar.Init (dwID);
	m_ctlGrpChar.SetParam (m_nGrpIDSub);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnSelchangeTypePile1						 */
/* ���e		:���ڑI���n���h��(�d�ˉ摜�P)									 */
/* ���t		:2007/11/17														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnSelchangeTypePile1()
{
	int nNo;
	DWORD dwID;

	dwID = 0;
	if (m_nSelect >= 0) {
		nNo  = m_cmbTypePile1.GetCurSel ();
		dwID = m_cmbTypePile1.GetItemData (nNo);
	}
	m_ctlGrpPile1.Init (dwID);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnSelchangeTypePile2						 */
/* ���e		:���ڑI���n���h��(�d�ˉ摜�Q)									 */
/* ���t		:2007/11/17														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnSelchangeTypePile2()
{
	int nNo;
	DWORD dwID;

	dwID = 0;
	if (m_nSelect >= 0) {
		nNo  = m_cmbTypePile2.GetCurSel ();
		dwID = m_cmbTypePile2.GetItemData (nNo);
	}
	m_ctlGrpPile2.Init (dwID);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnSelchangeTypePile3						 */
/* ���e		:���ڑI���n���h��(�d�ˉ摜�R)									 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnSelchangeTypePile3()
{
	int nNo;
	DWORD dwID;

	dwID = 0;
	if (m_nSelect >= 0) {
		nNo  = m_cmbTypePile3.GetCurSel ();
		dwID = m_cmbTypePile3.GetItemData (nNo);
	}
	m_ctlGrpPile3.Init (dwID);
}



/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnEnChangeViewpos							 */
/* ���e		:�C�x���g�n���h��(EN_CHANGE:�`��ʒu)							 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnEnChangeViewpos()
{
	if (IsWindowVisible ()) {
		Set (m_nSelect);
		RenewPreview ();
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::RenewSlider								 */
/* ���e		:�X���C�_�[���X�V												 */
/* ���t		:2007/11/14														 */
/* ========================================================================= */

void CDlgAdminCharMotion::RenewSlider(void)
{
	BOOL bEnable;
	int nMax;

	nMax = m_pLibInfoMotion->GetCount ();
	m_ctlSlider.SetRange (0, nMax - 1);

	OnSelectAnime ();

	bEnable = TRUE;
	if (nMax == 0) {
		bEnable = FALSE;
	}
	Enable (bEnable);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::RenewPreview								 */
/* ���e		:�v���r���[�摜���X�V											 */
/* ���t		:2007/11/17														 */
/* ========================================================================= */

void CDlgAdminCharMotion::RenewPreview(void)
{
	int nNo, nTmp, nNoChar, nSize, nSizeChar, nCountX, nCountCharX, x, y, i, nCount;
	DWORD dwParam;
	PCInfoMotion pInfo;
	PCMgrGrpData pMgrGrpData;
	CImg32 Img, *pImg, ImgTmp;
	HDC hDCBmp, hDCBmp2;

	pMgrGrpData = m_pMgrData->GetMgrGrpData ();

	pInfo = (PCInfoMotion)m_pLibInfoMotion->GetPtr (m_nSelect);
	if (pInfo == NULL) {
		return;
	}

	ImgTmp.Create (64, 64);
	Img.Create (64, 64);
	Img.FillRect (0, 0, Img.Width (), Img.Height (), RGB (255, 0, 255));
	nNoChar		= pInfo->m_wGrpIDSubBase - 1;
	nSizeChar	= pMgrGrpData->GetGrpSize (pInfo->m_wGrpIDMainBase);
	nCountCharX	= pMgrGrpData->GetGrpCountX (pInfo->m_wGrpIDMainBase);
	x = (nSizeChar == 16) ? 8 : 0;
	y = (nSizeChar == 16) ? 8 : 0;

	switch (pInfo->m_wGrpIDMainBase) {
	case GRPIDMAIN_CHAR:		/* �L���� */
	case GRPIDMAIN_2X2_CHAR:	/* �L����(32x32) */
		dwParam = FAMILYTYPE_HUMAN;
		break;
	default:
		dwParam = m_nGrpIDSub;
		break;
	}

	nCount = m_ctlDrawList.GetCount ();
	for (i = 0; i < nCount; i ++) {
		switch (m_ctlDrawList.GetItemData (i)) {
		case 0:		/* ���n */
			pImg = pMgrGrpData->GetDib (pInfo->m_wGrpIDMainBase, 0, dwParam);
			if (pImg) {
				Img.BltFrom256 (x + pInfo->m_ptDrawPosPile0.x / 2, y + pInfo->m_ptDrawPosPile0.y / 2, nSizeChar, nSizeChar, pImg, (nNoChar % nCountCharX) * nSizeChar, (nNoChar / nCountCharX) * nSizeChar, TRUE);
			}
			break;
		case 1:		/* �d�ˍ��킹1 */
			nNo		= pInfo->m_wGrpIDSubPile1;
			nSize	= pMgrGrpData->GetGrpSize (pInfo->m_wGrpIDMainPile1);
			nCountX	= pMgrGrpData->GetGrpCountX (pInfo->m_wGrpIDMainPile1);
			if (nNo > 0) {
				nNo --;
				nTmp = 0;
				if (nSize > nSizeChar) {
					nTmp = (nSize - nSizeChar) / 2 * -1;
				}
				pImg = pMgrGrpData->GetDib (pInfo->m_wGrpIDMainPile1, nNo, 0);
				if (pImg) {
					Img.BltAlphaFrom256 (x + nTmp + pInfo->m_ptDrawPosPile1.x / 2, y + nTmp + pInfo->m_ptDrawPosPile1.y / 2, nSize, nSize, pImg, (nNo % nCountX) * nSize, (nNo / nCountX) * nSize, pInfo->m_byLevel1, TRUE);
				}
			}
			break;
		case 2:		/* �d�ˍ��킹2 */
			nNo		= pInfo->m_wGrpIDSubPile2;
			nSize	= pMgrGrpData->GetGrpSize (pInfo->m_wGrpIDMainPile2);
			nCountX	= pMgrGrpData->GetGrpCountX (pInfo->m_wGrpIDMainPile2);
			if (nNo > 0) {
				nNo --;
				nTmp = 0;
				if (nSize > nSizeChar) {
					nTmp = (nSize - nSizeChar) / 2 * -1;
				}
				pImg = pMgrGrpData->GetDib (pInfo->m_wGrpIDMainPile2, pInfo->m_wGrpIDSubPile2, 0);
				if (pImg) {
					Img.BltAlphaFrom256 (x + nTmp + pInfo->m_ptDrawPosPile2.x / 2, y + nTmp + pInfo->m_ptDrawPosPile2.y / 2, nSize, nSize, pImg, (nNo % nCountX) * nSize, (nNo / nCountX) * nSize, pInfo->m_byLevel2, TRUE);
				}
			}
			break;
		case 3:		/* �d�ˍ��킹3 */
			nNo		= pInfo->m_wGrpIDSubPile3;
			nSize	= pMgrGrpData->GetGrpSize (pInfo->m_wGrpIDMainPile3);
			nCountX	= pMgrGrpData->GetGrpCountX (pInfo->m_wGrpIDMainPile3);
			if (nNo > 0) {
				nNo --;
				nTmp = 0;
				if (nSize > nSizeChar) {
					nTmp = (nSize - nSizeChar) / 2 * -1;
				}
				pImg = pMgrGrpData->GetDib (pInfo->m_wGrpIDMainPile3, pInfo->m_wGrpIDSubPile3, 0);
				if (pImg) {
					Img.BltAlphaFrom256 (x + nTmp + pInfo->m_ptDrawPosPile3.x / 2, y + nTmp + pInfo->m_ptDrawPosPile3.y / 2, nSize, nSize, pImg, (nNo % nCountX) * nSize, (nNo / nCountX) * nSize, pInfo->m_byLevel3, TRUE);
				}
			}
			break;
		}
	}

	hDCBmp2 = Img.Lock ();
	hDCBmp = ImgTmp.Lock ();
	StretchBlt (hDCBmp, 0, 0, 64, 64, hDCBmp2, 0, 0, 32, 32, SRCCOPY);
	Img.Unlock ();
	ImgTmp.Unlock ();

	m_ctlGrpPreview.Init (&ImgTmp);
	m_ctlGrpPreview.RedrawWindow ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::OnSelectAnime								 */
/* ���e		:�R�}�I������													 */
/* ���t		:2007/11/14														 */
/* ========================================================================= */

void CDlgAdminCharMotion::OnSelectAnime(void)
{
	int nCount;

	Set (m_nSelect);
	m_nSelect = m_ctlSlider.GetPos ();

	nCount = m_pLibInfoMotion->GetCount ();
	m_strCount.Format ("%d", nCount);
	m_strNow.Empty ();
	if (nCount > 0) {
		m_strNow.Format ("%d", m_nSelect + 1);
	}
	UpdateData (FALSE);

	Get (m_nSelect);
	RenewPreview ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::Enable									 */
/* ���e		:���͋֎~�ݒ�													 */
/* ���t		:2007/11/14														 */
/* ========================================================================= */

void CDlgAdminCharMotion::Enable(BOOL bEnable)
{
	int i, nResourceID,
		anResourceID[] = {
			IDC_TYPE_CHAR,
			IDC_TYPE_PILE1,
			IDC_TYPE_PILE2,
			IDC_TYPE_PILE3,
			IDC_VIEWPOS_X1,
			IDC_VIEWPOS_Y1,
			IDC_VIEWPOS_X2,
			IDC_VIEWPOS_Y2,
			IDC_VIEWPOS_X3,
			IDC_VIEWPOS_Y3,
			IDC_VIEWTIME,
			IDC_LEVEL1,
			IDC_LEVEL2,
			IDC_LEVEL3,
			IDC_DRAWLIST,
			IDC_DRAW_UP,
			IDC_DRAW_DOWN,
			IDC_DEL,
			IDC_PLAY,
			IDC_STOP,
			IDC_LOOP,
			IDC_SLIDER,
			IDC_PILE,
			IDC_REDRAWHAND,
			IDC_SOUNDID,
			IDC_PROCID,
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
/* �֐���	:CDlgAdminCharMotion::Set										 */
/* ���e		:���[�V�������֐ݒ�											 */
/* ���t		:2007/11/14														 */
/* ========================================================================= */

void CDlgAdminCharMotion::Set(int nNo)
{
	int nSelect, i, nCount;
	PCInfoMotion pInfo;

	pInfo = (PCInfoMotion)m_pLibInfoMotion->GetPtr (nNo);
	if (pInfo == NULL) {
		return;
	}

	UpdateData ();
	pInfo->m_byWait				= (BYTE)m_nViewTime;				/* �҂�����(�~�P�O�~���b) */
	pInfo->m_byLevel1			= (BYTE)m_nLevel1;					/* �����x1 */
	pInfo->m_byLevel2			= (BYTE)m_nLevel2;					/* �����x2 */
	pInfo->m_byLevel3			= (BYTE)m_nLevel3;					/* �����x3 */
	pInfo->m_bPile				= m_bPile;							/* �d�ˉ摜���ɕ`�� */
	pInfo->m_bRedrawHand		= m_bRedrawHand;					/* �����O�ɕ`�� */
	pInfo->m_bLoop				= m_bLoop;							/* ���[�v�Đ� */
	pInfo->m_ptDrawPosPile0.x	= m_nViewPosX0;						/* �`��ʒu(���n)�� */
	pInfo->m_ptDrawPosPile0.y	= m_nViewPosY0;						/* �`��ʒu(���n)�c */
	pInfo->m_ptDrawPosPile1.x	= m_nViewPosX1;						/* �`��ʒu(�d�ˍ��킹1)�� */
	pInfo->m_ptDrawPosPile1.y	= m_nViewPosY1;						/* �`��ʒu(�d�ˍ��킹1)�c */
	pInfo->m_ptDrawPosPile2.x	= m_nViewPosX2;						/* �`��ʒu(�d�ˍ��킹2)�� */
	pInfo->m_ptDrawPosPile2.y	= m_nViewPosY2;						/* �`��ʒu(�d�ˍ��킹2)�c */
	pInfo->m_ptDrawPosPile3.x	= m_nViewPosX3;						/* �`��ʒu(�d�ˍ��킹3)�� */
	pInfo->m_ptDrawPosPile3.y	= m_nViewPosY3;						/* �`��ʒu(�d�ˍ��킹3)�c */
	pInfo->m_wGrpIDMainBase		= (WORD)m_ctlGrpChar. GetIDMain ();	/* �O���t�B�b�NID���C��(���n) */
	pInfo->m_wGrpIDMainPile1	= (WORD)m_ctlGrpPile1.GetIDMain ();	/* �O���t�B�b�NID���C��(�d�ˍ��킹1) */
	pInfo->m_wGrpIDMainPile2	= (WORD)m_ctlGrpPile2.GetIDMain ();	/* �O���t�B�b�NID���C��(�d�ˍ��킹2) */
	pInfo->m_wGrpIDMainPile3	= (WORD)m_ctlGrpPile3.GetIDMain ();	/* �O���t�B�b�NID���C��(�d�ˍ��킹3) */
	pInfo->m_wGrpIDSubBase		= (WORD)m_ctlGrpChar. GetIDSub ();	/* �O���t�B�b�NID�T�u(���n) */
	pInfo->m_wGrpIDSubPile1		= (WORD)m_ctlGrpPile1.GetIDSub ();	/* �O���t�B�b�NID�T�u(�d�ˍ��킹1) */
	pInfo->m_wGrpIDSubPile2		= (WORD)m_ctlGrpPile2.GetIDSub ();	/* �O���t�B�b�NID�T�u(�d�ˍ��킹2) */
	pInfo->m_wGrpIDSubPile3		= (WORD)m_ctlGrpPile3.GetIDSub ();	/* �O���t�B�b�NID�T�u(�d�ˍ��킹3) */

	nSelect = m_cmbSoundID.GetCurSel ();
	pInfo->m_dwSoundID	= m_cmbSoundID.GetItemData (nSelect);	/* ���ʉ�ID */
	nSelect = m_cmbProcID.GetCurSel ();
	pInfo->m_dwProcID	= m_cmbProcID.GetItemData (nSelect);	/* �s��ID */

	pInfo->m_anDrawList.RemoveAll ();
	nCount = m_ctlDrawList.GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo->m_anDrawList.Add (m_ctlDrawList.GetItemData (i));
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotion::Get										 */
/* ���e		:���[�V������񂩂�擾											 */
/* ���t		:2007/11/14														 */
/* ========================================================================= */

void CDlgAdminCharMotion::Get(int nNo)
{
	int i, nCount;
	PCInfoMotion pInfo;
	ARRAYINT anTmp;
	LPSTR paszName[] = {"���n", "�d�ˉ摜1", "�d�ˉ摜2", "�d�ˉ摜3"};

	pInfo = (PCInfoMotion)m_pLibInfoMotion->GetPtr (nNo);
	if (pInfo == NULL) {
		return;
	}

	m_nViewTime		= pInfo->m_byWait;				/* �҂�����(�~�P�O�~���b) */
	m_nLevel1		= pInfo->m_byLevel1;			/* �����x1 */
	m_nLevel2		= pInfo->m_byLevel2;			/* �����x2 */
	m_nLevel3		= pInfo->m_byLevel3;			/* �����x3 */
	m_bPile			= pInfo->m_bPile;				/* �d�ˉ摜���ɕ`�� */
	m_bRedrawHand	= pInfo->m_bRedrawHand;			/* �����O�ɕ`�� */
	m_bLoop			= pInfo->m_bLoop;				/* ���[�v�Đ� */
	m_nViewPosX0	= pInfo->m_ptDrawPosPile0.x;	/* �`��ʒu(���n)�� */
	m_nViewPosY0	= pInfo->m_ptDrawPosPile0.y;	/* �`��ʒu(���n)�c */
	m_nViewPosX1	= pInfo->m_ptDrawPosPile1.x;	/* �`��ʒu(�d�ˍ��킹1)�� */
	m_nViewPosY1	= pInfo->m_ptDrawPosPile1.y;	/* �`��ʒu(�d�ˍ��킹1)�c */
	m_nViewPosX2	= pInfo->m_ptDrawPosPile2.x;	/* �`��ʒu(�d�ˍ��킹2)�� */
	m_nViewPosY2	= pInfo->m_ptDrawPosPile2.y;	/* �`��ʒu(�d�ˍ��킹2)�c */
	m_nViewPosX3	= pInfo->m_ptDrawPosPile3.x;	/* �`��ʒu(�d�ˍ��킹3)�� */
	m_nViewPosY3	= pInfo->m_ptDrawPosPile3.y;	/* �`��ʒu(�d�ˍ��킹3)�c */
	m_ctlGrpChar. Init (pInfo->m_wGrpIDMainBase);	/* �O���t�B�b�NID���C��(���n) */
	m_ctlGrpPile1.Init (pInfo->m_wGrpIDMainPile1);	/* �O���t�B�b�NID���C��(�d�ˍ��킹1) */
	m_ctlGrpPile2.Init (pInfo->m_wGrpIDMainPile2);	/* �O���t�B�b�NID���C��(�d�ˍ��킹2) */
	m_ctlGrpPile3.Init (pInfo->m_wGrpIDMainPile3);	/* �O���t�B�b�NID���C��(�d�ˍ��킹3) */
	m_ctlGrpChar. Set (pInfo->m_wGrpIDSubBase);		/* �O���t�B�b�NID���C��(���n) */
	m_ctlGrpPile1.Set (pInfo->m_wGrpIDSubPile1);	/* �O���t�B�b�NID���C��(�d�ˍ��킹1) */
	m_ctlGrpPile2.Set (pInfo->m_wGrpIDSubPile2);	/* �O���t�B�b�NID���C��(�d�ˍ��킹2) */
	m_ctlGrpPile3.Set (pInfo->m_wGrpIDSubPile3);	/* �O���t�B�b�NID���C��(�d�ˍ��킹3) */

	UpdateData (FALSE);

	SelectCmb (&m_cmbTypeChar,	pInfo->m_wGrpIDMainBase);
	SelectCmb (&m_cmbTypePile1,	pInfo->m_wGrpIDMainPile1);
	SelectCmb (&m_cmbTypePile2,	pInfo->m_wGrpIDMainPile2);
	SelectCmb (&m_cmbTypePile3,	pInfo->m_wGrpIDMainPile3);
	SelectCmb (&m_cmbSoundID,	pInfo->m_dwSoundID);
	SelectCmb (&m_cmbProcID,	pInfo->m_dwProcID);

	m_ctlDrawList.ResetContent ();
	nCount = pInfo->m_anDrawList.GetSize ();
	if (nCount == 0) {
		for (i = 0; i < 4; i ++) {
			anTmp.Add (i);
		}
	} else {
		for (i = 0; i < nCount; i ++) {
			anTmp.Add (pInfo->m_anDrawList[i]);
		}
	}
	nCount = anTmp.GetSize ();
	for (i = 0; i < nCount; i ++) {
		m_ctlDrawList.InsertString (i, paszName[anTmp[i]]);
		m_ctlDrawList.SetItemData (i, anTmp[i]);
	}
}

/* Copyright(C)URARA-works 2007 */
