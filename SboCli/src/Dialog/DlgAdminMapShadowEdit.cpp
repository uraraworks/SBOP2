/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapShadowEdit.cpp									 */
/* ���e			:�}�b�v�e�ҏW�_�C�A���O�N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/06													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapShadow.h"
#include "WndMapShadowAnimeList.h"
#include "WndMapPartsGrp.h"
#include "Img32.h"
#include "MgrGrpData.h"
#include "MgrData.h"
#include "DlgAdminMapShadowEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�ݒ�																 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapShadowEdit)
	DDX_Control(pDX, IDC_SLIDER, m_Slider);
	DDX_Text(pDX, IDC_COUNT, m_strCount);
	DDX_Text(pDX, IDC_NOW, m_strNow);
	DDX_Text(pDX, IDC_LEVEL, m_nLevel);
	DDX_Text(pDX, IDC_VIEWTIME, m_nViewTime);
	DDX_CBIndex(pDX, IDC_GRPNO, m_nGrpNo);
	DDX_Control(pDX, IDC_GRPNO, m_cbGrpNo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapShadowEdit, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapShadowEdit)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_LEVEL, OnChangeLevel)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_VIEWTIME, OnChangeViewtime)
	ON_WM_GETMINMAXINFO()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_CBN_SELCHANGE(IDC_GRPNO, OnSelchangeGrpNo)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WNDCLOSE, OnWndClose)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::CDlgAdminMapShadowEdit					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

CDlgAdminMapShadowEdit::CDlgAdminMapShadowEdit(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapShadowEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapShadowEdit)
	m_strCount = _T("");
	m_strNow = _T("");
	m_nLevel = 0;
	m_nViewTime = 0;
	m_nGrpNo = 0;
	//}}AFX_DATA_INIT

	m_nSelectType			= 0;
	m_nAnimeNo				= 0;
	m_wShadowGrpID			= 0;
	m_dwTimeLastAnime		= 0;
	m_pMgrData				= NULL;
	m_pMgrGrpData			= NULL;
	m_pWndMapShadowAnimeList	= NULL;

	m_sizeWindow.cx = m_sizeWindow.cy = 0;

	m_pInfoMapShadow			= new CInfoMapShadow;
	m_pWndMapShadowAnimeList	= new CWndMapShadowAnimeList;
	m_pWndMapShadowGrp			= new CWndMapPartsGrp;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::~CDlgAdminMapShadowEdit				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

CDlgAdminMapShadowEdit::~CDlgAdminMapShadowEdit()
{
	SAFE_DELETE (m_pInfoMapShadow);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::DoModal								 */
/* ���e		:���[�_���_�C�A���O�\��											 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

int CDlgAdminMapShadowEdit::DoModal(CMgrData *pMgrData)
{
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData ();

	return CDlgAdminBase::DoModal ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

BOOL CDlgAdminMapShadowEdit::OnInitDialog()
{
	int i, nCount;
	CString strTmp;
	CSize sizeTmp;
	CRect rc;

	CDlgAdminBase::OnInitDialog();

	m_pWndMapShadowGrp->Create (this, m_pMgrData, IDC_PARTS, 1);
	m_pWndMapShadowAnimeList->Create (this, m_pMgrData, IDC_LIST, m_pInfoMapShadow);

	GetWindowRect (rc);
	m_sizeWindow.cx = rc.Width ();
	m_sizeWindow.cy = rc.Height ();

	GetDlgItem (IDC_PREVIEW)->SetWindowPos (NULL, 0, 0, 16, 16, SWP_NOMOVE);

	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH);
	RegisterControl (IDOK,		LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (m_pWndMapShadowGrp->GetSafeHwnd (), LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	GetDlgItem (IDC_STOP)->EnableWindow (FALSE);
	CheckRadioButton (IDC_MOVE_UP, IDC_MOVE_NONE, IDC_MOVE_NONE);
	CheckRadioButton (IDC_GRP_PARTS, IDC_GRP_SHASOW, IDC_GRP_PARTS);

	nCount = m_pMgrData->GetMgrGrpData ()->GetMapShadowCount () / 1024;
	for (i = 0; i < nCount; i ++) {
		strTmp.Format ("�摜 %d ����", i + 1);
		m_cbGrpNo.AddString (strTmp);
	}
	m_cbGrpNo.SetCurSel (0);
	OnSelchangeGrpNo ();

	SetData ();

	sizeTmp = m_pMgrData->GetDlgMapPartsEdit ();
	SetWindowPos (NULL, 0, 0, sizeTmp.cx, sizeTmp.cy, SWP_NOZORDER | SWP_NOMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnPaint								 */
/* ���e		:���b�Z�[�W�n���h��(WM_PAINT)									 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnPaint()
{
	int no;
	HDC hDC;
	CDC *pDC, *pDCTmp;
	CImg32 ImgTmp, ImgPreview, *pImg;

	CPaintDC dc(this);

	ImgTmp.Create (32, 32);
	ImgTmp.SetColorKey (RGB (255, 0, 255));
	ImgPreview.Create (32, 32);

	/* ���n */
	no = m_wShadowGrpID;
	pImg = m_pMgrGrpData->GetDibMapShadow (no / 1024);
	if (pImg == NULL) {
		return;
	}
	ImgPreview.BltFrom256 (0, 0, 16, 16, pImg, (no % 1024) % 32 * 16, (no % 1024) / 32 * 16);

	/* �v���r���[ */
	hDC		= ImgPreview.Lock ();
	pDCTmp	= dc.FromHandle (hDC);
	pDC		= GetDlgItem (IDC_PREVIEW)->GetDC ();
	pDC->StretchBlt (0, 0, 32, 32, pDCTmp, 0, 0, 16, 16, SRCCOPY);
	ImgPreview.Unlock ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnWndClose								 */
/* ���e		:���b�Z�[�W�n���h��(WM_WNDCLOSE)								 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

LRESULT CDlgAdminMapShadowEdit::OnWndClose(WPARAM wParam, LPARAM lParam)
{
	int nNo;
	PCInfoAnime pInfo;

	switch (wParam) {
	case WINDOWID_SELECTMAPPARTSGRP:	/* �}�b�v�e�摜�I�� */
		if (lParam >= 0) {
			if (m_nSelectType == 1) {
				m_wShadowGrpID = (WORD)lParam;
			}
			if (m_pInfoMapShadow->GetAnimeCount () > 0) {
				nNo = m_Slider.GetPos ();
				pInfo = m_pInfoMapShadow->GetAnimePtr (nNo);
				pInfo->m_wGrpIDBase = m_wShadowGrpID;

			} else {
				m_pInfoMapShadow->m_wGrpID = m_wShadowGrpID;
			}
			InvalidateRect (NULL);
			m_pWndMapShadowAnimeList->Renew ();
		}
		break;
	}
	m_nSelectType = 0;

	return 0;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnAdminMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/05/18														 */
/* ========================================================================= */

LRESULT CDlgAdminMapShadowEdit::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:		/* ���N���b�N�ʒm */
		m_nSelectType = 1;
		PostMessage (WM_WNDCLOSE, WINDOWID_SELECTMAPPARTSGRP, lParam);
		break;
	case ADMINMSG_SELECT_ANIME:					/* �A�j���[�V�����R�}�I�� */
		m_nAnimeNo = lParam;
		m_Slider.SetPos (m_nAnimeNo);
		OnSelectAnime ();
		break;
	}

	return 0;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnHScroll								 */
/* ���e		:���b�Z�[�W�n���h��(WM_HSCROLL)									 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	OnSelectAnime ();

	CDlgAdminBase::OnHScroll (nSBCode, nPos, pScrollBar);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnGetMinMaxInfo						 */
/* ���e		:���b�Z�[�W�n���h��(WM_GETMINMAXINFO)							 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	if (m_sizeWindow.cx > 0) {
		lpMMI->ptMinTrackSize.x = m_sizeWindow.cx;
		lpMMI->ptMinTrackSize.y = m_sizeWindow.cy;
	}
	
	CDlgAdminBase::OnGetMinMaxInfo (lpMMI);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnMouseMove							 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEMOVE)								 */
/* ���t		:2007/05/16														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	CDlgAdminBase::OnMouseMove(nFlags, point);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnLButtonDown							 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONUP)								 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDlgAdminBase::OnLButtonDown(nFlags, point);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnMouseWheel							 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEWHEEL)								 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

BOOL CDlgAdminMapShadowEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return CDlgAdminBase::OnMouseWheel (nFlags, zDelta, pt);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnTimer								 */
/* ���e		:���b�Z�[�W�n���h��(WM_TIMER)									 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnTimer(UINT nIDEvent)
{
	int nMin, nMax;
	DWORD dwTime;
	PCInfoAnime pInfo;

	m_Slider.GetRange (nMin, nMax);

	pInfo = m_pInfoMapShadow->GetAnimePtr (m_nAnimeNo);
	if (pInfo == NULL) {
		return;
	}
	dwTime = timeGetTime () - m_dwTimeLastAnime;
	if (dwTime < (DWORD)(pInfo->m_byWait * 10)) {
		return;
	}
	m_dwTimeLastAnime = timeGetTime ();

	m_nAnimeNo ++;
	if (m_nAnimeNo > nMax) {
		m_nAnimeNo = 0;
	}

	m_Slider.SetPos (m_nAnimeNo);
	OnSelectAnime ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnChangeLevel							 */
/* ���e		:�C�x���g�n���h��(ENM_CHANGE)									 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnChangeLevel()
{
	int nNo;
	PCInfoAnime pInfo;

	UpdateData ();
	InvalidateRect (NULL);

	if (m_pInfoMapShadow->GetAnimeCount () > 0) {
		nNo					= m_Slider.GetPos ();
		pInfo				= m_pInfoMapShadow->GetAnimePtr (nNo);
		pInfo->m_byLevel	= m_nLevel;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnChangeViewtime						 */
/* ���e		:�C�x���g�n���h��(ENM_CHANGE)									 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnChangeViewtime()
{
	int nNo;
	PCInfoAnime pInfo;

	UpdateData ();
	InvalidateRect (NULL);

	if (m_pInfoMapShadow->GetAnimeCount () > 0) {
		nNo				= m_Slider.GetPos ();
		pInfo			= m_pInfoMapShadow->GetAnimePtr (nNo);
		pInfo->m_byWait	= m_nViewTime;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnAdd									 */
/* ���e		:�{�^���n���h��(�ǉ�)											 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnAdd()
{
	int nCount;

	m_pInfoMapShadow->AddAnime ();
	nCount = m_pInfoMapShadow->GetAnimeCount ();
	m_strCount.Format ("%d", nCount);
	m_Slider.SetRange (0, nCount - 1, TRUE);
	m_Slider.SetPos (nCount - 1);

	OnSelectAnime ();
	RenewButtonState ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnDel									 */
/* ���e		:�{�^���n���h��(�폜)											 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnDel()
{
	int nTmp;

	if (m_pInfoMapShadow->GetAnimeCount () > 0) {
		m_pInfoMapShadow->DeleteAnime (m_Slider.GetPos ());
		m_strCount.Format ("%d", m_pInfoMapShadow->GetAnimeCount ());
		m_Slider.SetRange (0, m_pInfoMapShadow->GetAnimeCount () - 1, TRUE);
	}

	nTmp = m_Slider.GetPos () + 1;
	if (m_pInfoMapShadow->GetAnimeCount () == 0) {
		nTmp = 0;
	}

	OnSelectAnime ();
	RenewButtonState ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnPlay									 */
/* ���e		:�{�^���n���h��(�Đ�)											 */
/* ���t		:2007/05/28														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnPlay()
{
	GetDlgItem (IDC_PLAY)->EnableWindow (FALSE);
	GetDlgItem (IDC_STOP)->EnableWindow (TRUE);

	SetTimer (0, 10, NULL);

	m_nAnimeNo			= 0;
	m_dwTimeLastAnime	= timeGetTime ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnStop									 */
/* ���e		:�{�^���n���h��(��~)											 */
/* ���t		:2007/05/28														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnStop()
{
	GetDlgItem (IDC_PLAY)->EnableWindow (TRUE);
	GetDlgItem (IDC_STOP)->EnableWindow (FALSE);

	KillTimer (0);

	m_nAnimeNo			= 0;
	m_dwTimeLastAnime	= 0;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnSelchangeGrpNo						 */
/* ���e		:�C�x���g�n���h��(CBN_SELCHANGE)								 */
/* ���t		:2008/10/11														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnSelchangeGrpNo()
{
	UpdateData ();

	m_pWndMapShadowGrp->SetNo (m_nGrpNo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnOK									 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnOK()
{
	CRect rcTmp;
	SIZE sizeTmp;

	GetWindowRect (rcTmp);
	sizeTmp.cx = rcTmp.Width ();
	sizeTmp.cy = rcTmp.Height ();
	m_pMgrData->SetDlgMapPartsEdit (sizeTmp);

	GetData ();
	CDlgAdminBase::OnOK();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::SetData								 */
/* ���e		:�f�[�^����ʂɔ��f												 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::SetData(void)
{
	PCInfoAnime pAnime;

	m_strCount.Format ("%d", m_pInfoMapShadow->m_byAnimeCount);
	m_Slider.SetRange (0, m_pInfoMapShadow->m_byAnimeCount - 1, TRUE);
	if (m_pInfoMapShadow->m_byAnimeCount > 0) {
		pAnime = m_pInfoMapShadow->GetAnimePtr (0);
		m_nLevel		= pAnime->m_byLevel;
		m_nViewTime		= pAnime->m_byWait;
		m_wShadowGrpID	= pAnime->m_wGrpIDBase;
		m_strNow.Format ("%d", 1);

	} else {
		m_nLevel		= m_pInfoMapShadow->m_byLevel;
		m_nViewTime		= 0;
		m_wShadowGrpID	= m_pInfoMapShadow->m_wGrpID;
		m_strNow.Format ("%d", 0);
	}

	UpdateData (FALSE);
	RenewButtonState ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::GetData								 */
/* ���e		:��ʂ���f�[�^���擾											 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::GetData(void)
{
	OnChangeViewtime ();

	m_pInfoMapShadow->m_byLevel = m_nLevel;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::OnSelectAnime							 */
/* ���e		:�R�}�I������													 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::OnSelectAnime(void)
{
	int nNo;
	PCInfoAnime pInfo;

	nNo		= m_Slider.GetPos ();
	pInfo	= m_pInfoMapShadow->GetAnimePtr (nNo);

	if (pInfo) {
		m_wShadowGrpID	= pInfo->m_wGrpIDBase;
		m_nLevel		= pInfo->m_byLevel;
		m_nViewTime		= pInfo->m_byWait;
	} else {
		nNo = -1;
	}

	m_strNow.Format ("%d", nNo + 1);
	UpdateData (FALSE);
	InvalidateRect (NULL);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::RenewButtonState						 */
/* ���e		:�{�^����Ԃ̐ݒ�												 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::RenewButtonState(void)
{
	BOOL bEnable;
	int nMin, nMax;

	bEnable = FALSE;
	nMin	= nMax = 0;

	m_Slider.GetRange (nMin, nMax);
	if (nMax >= 0) {
		bEnable = TRUE;
	}

	GetDlgItem (IDC_DEL)->		EnableWindow (bEnable);
	GetDlgItem (IDC_VIEWTIME)->	EnableWindow (bEnable);
	GetDlgItem (IDC_SLIDER)->	EnableWindow (bEnable);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapShadowEdit::PostNcDestroy							 */
/* ���e		:�I������														 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadowEdit::PostNcDestroy()
{
}

/* Copyright(C)URARA-works 2007 */
