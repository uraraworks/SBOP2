/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapParts.cpp										 */
/* ���e			:�}�b�v�p�[�c�ҏW�_�C�A���O�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/25													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "LibInfoMapParts.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_RENEWMAPPARTS.h"
#include "PacketADMIN_MAP_DELETEPARTS.h"
#include "PacketADMIN_MAP_COPYPARTS.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "DlgAdminMapPartsEdit.h"
#include "DlgAdminMapParts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminMapParts::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapParts)
	DDX_Control(pDX, IDC_MAPPARTS, m_ctlMapParts);
	DDX_Control(pDX, IDC_SCROLL, m_ctlScroll);
	DDX_Control(pDX, IDC_TYPE, m_cbType);
	DDX_CBIndex(pDX, IDC_TYPE, m_nTypeSelect);
	DDX_Text(pDX, IDC_ID, m_strID);
	DDX_Text(pDX, IDC_MSG, m_strMsg);
	DDX_Text(pDX, IDC_ATTR, m_strAttr);
	DDX_Text(pDX, IDC_ATTR2, m_strAttr2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapParts, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapParts)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WNDCLOSE, OnWndClose)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::CDlgAdminMapParts							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/04/25														 */
/* ========================================================================= */

CDlgAdminMapParts::CDlgAdminMapParts(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapParts::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapParts)
	m_nTypeSelect = 0;
	m_strID = _T("");
	m_strMsg = _T("");
	m_strAttr = _T("");
	m_strAttr2 = _T("");
	//}}AFX_DATA_INIT

	m_nSelectType		= 0;
	m_nState			= 0;
	m_dwSelectPartsID	= 0;

	m_pMgrGrpData		= NULL;
	m_pImgParts			= new CImg32;
	m_pLibInfoMapParts	= NULL;
	m_pSock				= NULL;

	m_ptMoveSrc.x = m_ptMoveSrc.y = 0;
	m_ptMoveDst.x = m_ptMoveDst.y = 0;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::~CDlgAdminMapParts							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/04/25														 */
/* ========================================================================= */

CDlgAdminMapParts::~CDlgAdminMapParts()
{
	SAFE_DELETE (m_pImgParts);
	if (m_pMgrGrpData) {
		m_pMgrGrpData->DeleteMapPartsTmp ();
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::Init										 */
/* ���e		:������															 */
/* ���t		:2007/04/25														 */
/* ========================================================================= */

void CDlgAdminMapParts::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pMgrGrpData		= m_pMgrData->GetMgrGrpData ();
	m_pLibInfoMapParts	= m_pMgrData->GetLibInfoMapParts ();
	m_pSock				= m_pMgrData->GetUraraSockTCP ();

	m_pMgrGrpData->ReadMapPartsTmp ();

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminMapParts::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::OnAdminMsg									 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_RENEWMAPPARTS:	/* �}�b�v�p�[�c���X�V */
		MakePartsImage ();
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::OnInitDialog								 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2007/04/25														 */
/* ========================================================================= */

BOOL CDlgAdminMapParts::OnInitDialog()
{
	CWnd *pWnd;
	CRect rc, rcTmp;
	SCROLLINFO stScrollInfo;

	CDlgAdminBase::OnInitDialog();

	m_ctlMapParts.Create (this, m_pMgrData);

	m_cbType.AddString ("�}�b�v�ҏW�p�ɑI��");
	m_cbType.AddString ("�p�[�c�̒ǉ�");
	m_cbType.AddString ("�p�[�c�̕ҏW");
	m_cbType.AddString ("�p�[�c�̈ړ�");
	m_cbType.AddString ("�p�[�c�̍폜");
	m_cbType.AddString ("�p�[�c�̃R�s�[");
	m_cbType.SetCurSel (0);

	pWnd = GetDlgItem (IDC_MAPPARTS);
	pWnd->GetWindowRect (rc);
	rc.right = rc.left + 16 * 23;
	rc.bottom = rc.top + 32 * 7;
	pWnd->SetWindowPos (NULL, 0, 0, rc.Width (), rc.Height (), SWP_NOZORDER | SWP_NOMOVE);

	/* �X�N���[���o�[�̐ݒ� */
	ScreenToClient (rc);
	m_ctlScroll.GetWindowRect (rcTmp);
	m_ctlScroll.SetWindowPos (NULL, rc.right, rc.top, rcTmp.Width (), rc.Height (), SWP_NOZORDER);
	m_ctlScroll.GetScrollInfo (&stScrollInfo);
	stScrollInfo.nPage	= 1;
	stScrollInfo.nMax	= 100;
	m_ctlScroll.SetScrollInfo (&stScrollInfo);

	RegisterControl (IDC_MAPPARTS,	LH_CTRL_HEIGHT);
	RegisterControl (IDC_SCROLL,	LH_CTRL_HEIGHT);

	MakePartsImage ();
	pWnd->SetFocus ();

	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::OnSize										 */
/* ���e		:���b�Z�[�W�n���h��(WM_SIZE)									 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnSize(UINT nType, int cx, int cy)
{
	HWND hWnd;

	CDlgAdminBase::OnSize (nType, cx, cy);

	hWnd = m_ctlScroll.GetSafeHwnd ();
	if (hWnd == NULL) {
		return;
	}

	MakePartsImage ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::OnMouseMove									 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEMOVE)								 */
/* ���t		:2007/04/25														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnMouseMove(UINT nFlags, CPoint point)
{
	int nPos;
	BOOL bResult;
	CPoint ptBack;
	CRect rc;

	CDlgAdminBase::OnMouseMove(nFlags, point);

	GetDlgItem (IDC_MAPPARTS)->GetWindowRect (rc);
	ScreenToClient (rc);

	bResult = rc.PtInRect (point);
	if (bResult == FALSE) {
		return;
	}
	nPos		 = m_ctlScroll.GetScrollPos ();
	point.x		-= rc.left;
	point.y		-= rc.top;
	ptBack		 = m_ptCursor;
	m_ptCursor.x = point.x / 16;
	m_ptCursor.y = point.y / 16 + nPos;

	m_ctlMapParts.OnMouseMove (point);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::OnLButtonDown								 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bResult;
	DWORD dwTmp;
	CRect rc;
	PCInfoMapParts pInfoMapParts;

	GetDlgItem (IDC_MAPPARTS)->GetWindowRect (rc);
	ScreenToClient (rc);

	bResult = rc.PtInRect (point);
	if (bResult == FALSE) {
		return;
	}

	GetDlgItem (IDC_MAPPARTS)->SetFocus ();

	switch (m_nSelectType) {
	case 0:		/* �}�b�v�ҏW�p�ɑI�� */
	case 2:		/* �ҏW */
		m_dwSelectPartsID = GetSelectMapPartsID ();
		m_pMgrData->SetSelectMapPartsID (m_dwSelectPartsID);
		m_strID.Format ("ID:%u", m_dwSelectPartsID);
		RenewPartsInfo ();
		break;
	case 1:		/* �ǉ� */
		{
			CPacketADMIN_RENEWMAPPARTS Packet;

			m_dwSelectPartsID = GetSelectMapPartsID ();
			if (m_dwSelectPartsID) {
				RenewMessage ("�����ɂ͒ǉ��ł��܂���");
				break;
			}
			pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetNew ();
			pInfoMapParts->m_ptViewPos = m_ptCursor;
			m_pLibInfoMapParts->Add (pInfoMapParts);
			MakePartsImage ();

			Packet.Make (pInfoMapParts);
			m_pSock->Send (&Packet);
		}
		break;
	case 3:		/* �ړ� */
		{
			CPacketADMIN_RENEWMAPPARTS Packet;

			switch (m_nState) {
			case 0:			/* �ړ����̑I�� */
				m_dwSelectPartsID = GetSelectMapPartsID ();
				if (m_dwSelectPartsID == 0) {
					break;
				}
				m_ptMoveSrc	= m_ptCursor;
				m_nState	= 1;
				RenewMessage ("�ړ�����N���b�N���Ă�������");
				break;
			case 1:			/* �ړ���̑I�� */
				m_ptMoveDst = m_ptCursor;

				dwTmp = GetSelectMapPartsID ();
				pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (&m_ptMoveDst);
				if (pInfoMapParts) {
					/* �ړ���Ƀp�[�c���������ꍇ�͓��ꊷ���� */
					pInfoMapParts->m_ptViewPos = m_ptMoveSrc;
					Packet.Make (pInfoMapParts);
					m_pSock->Send (&Packet);
				}
				pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (m_dwSelectPartsID);
				if (pInfoMapParts) {
					pInfoMapParts->m_ptViewPos = m_ptMoveDst;
					Packet.Make (pInfoMapParts);
					m_pSock->Send (&Packet);
				}
				MakePartsImage ();
				OnSelchangeType ();
				break;
			}
		}
		break;
	case 4:		/* �폜 */
		{
			int nResult;
			CPacketADMIN_MAP_DELETEPARTS Packet;

			m_dwSelectPartsID = GetSelectMapPartsID ();
			if (m_dwSelectPartsID == 0) {
				break;
			}

			nResult = MessageBox ("�I�����Ă���p�[�c���폜���܂����H", "�m�F", MB_ICONQUESTION | MB_YESNO);
			if (nResult != IDYES) {
				break;
			}
			Packet.Make (m_dwSelectPartsID);
			m_pSock->Send (&Packet);
		}
		break;
	case 5:		/* �R�s�[ */
		{
			CPacketADMIN_MAP_COPYPARTS Packet;

			switch (m_nState) {
			case 0:			/* �R�s�[���̑I�� */
				m_dwSelectPartsID = GetSelectMapPartsID ();
				if (m_dwSelectPartsID == 0) {
					break;
				}
				m_ptMoveSrc	= m_ptCursor;
				m_nState	= 1;
				RenewMessage ("�R�s�[����N���b�N���Ă�������");
				break;
			case 1:			/* �ړ���̑I�� */
				dwTmp = GetSelectMapPartsID ();
				pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (&m_ptMoveDst);
				if (pInfoMapParts) {
					/* �R�s�[��Ƀp�[�c���������ꍇ�͏������Ȃ� */
					break;
				}
				Packet.Make (m_dwSelectPartsID, m_ptCursor);
				m_pSock->Send (&Packet);
				OnSelchangeType ();
				break;
			}
		}
		break;
	}

	CDlgAdminBase::OnLButtonDown(nFlags, point);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::OnRButtonDown								 */
/* ���e		:���b�Z�[�W�n���h��(WM_RBUTTONDOWN)								 */
/* ���t		:2007/09/08														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnRButtonDown(UINT nFlags, CPoint point)
{
	PCInfoMapParts pInfoMapParts;

	switch (m_nSelectType) {
	case 2:		/* �ҏW */
		{
			int nResult;
			CDlgAdminMapPartsEdit Dlg(this);
			CPacketADMIN_RENEWMAPPARTS Packet;

			m_dwSelectPartsID = GetSelectMapPartsID ();
			pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (m_dwSelectPartsID);
			if (pInfoMapParts == NULL) {
				break;
			}
			Dlg.m_pInfoMapParts->Copy (pInfoMapParts);
			nResult = Dlg.DoModal (m_pMgrData);
			if (nResult != IDOK) {
				break;
			}
			pInfoMapParts->Copy (Dlg.m_pInfoMapParts);
			MakePartsImage ();

			Packet.Make (pInfoMapParts);
			m_pSock->Send (&Packet);
		}
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::OnWndClose									 */
/* ���e		:���b�Z�[�W�n���h��(WM_WNDCLOSE)								 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

LRESULT CDlgAdminMapParts::OnWndClose(WPARAM wParam, LPARAM lParam)
{
	PCInfoMapParts pInfoMapParts;

	switch (wParam) {
	case WINDOWID_SELECTMAPPARTSGRP:	/* �}�b�v�p�[�c�摜�I�� */
		if (lParam < 0) {
			break;
		}
		pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (m_dwSelectPartsID);
		if (pInfoMapParts == NULL) {
			break;
		}
		pInfoMapParts->m_wGrpIDBase = (WORD)lParam;
		MakePartsImage ();
		break;
	}

	return 0;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::OnVScroll									 */
/* ���e		:���b�Z�[�W�n���h��(WM_VSCROLL)									 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax;
	BOOL bChange;
	UINT nPosNow;
	SCROLLINFO stScrollInfo;

	nPosNow = pScrollBar->GetScrollPos ();
	bChange = TRUE;

	m_ctlScroll.GetScrollInfo (&stScrollInfo);

	switch (nSBCode) {
	case SB_TOP:			/* 1�ԏ�܂ŃX�N���[�� */
		pScrollBar->GetScrollRange (&nMin, &nMax);
		nPos = nMin;
		break;
	case SB_BOTTOM:			/* 1�ԉ��܂ŃX�N���[�� */
		pScrollBar->GetScrollRange (&nMin, &nMax);
		nPos = nMax;
		break;
	case SB_LINEUP:			/* 1�s��փX�N���[�� */
		nPos = nPosNow;
		if (nPosNow == 0) {
			break;
		}
		nPos --;
		break;
	case SB_PAGEUP:			/* 1�y�[�W��փX�N���[�� */
		nPos = nPosNow - stScrollInfo.nPage;
		break;
	case SB_LINEDOWN:		/* 1�s���փX�N���[�� */
		nPos = nPosNow + 1;
		break;
	case SB_PAGEDOWN:		/* 1�y�[�W���փX�N���[�� */
		nPos = nPosNow + stScrollInfo.nPage;
		break;
	case SB_THUMBPOSITION:	/* ��Έʒu�փX�N���[�� */
	case SB_THUMBTRACK:		/* �h���b�O���ꂽ */
		break;
	default:
		bChange = FALSE;
		break;
	}

	if (bChange) {
		pScrollBar->SetScrollPos (nPos);
		m_ctlMapParts.SetScrollPos (nPos);
	}

	CDlgAdminBase::OnVScroll (nSBCode, nPos, pScrollBar);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::OnMouseWheel								 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEWHEEL)								 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

BOOL CDlgAdminMapParts::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int nTmp;

	nTmp = -1;
	if (zDelta < 0) {
		nTmp = 1;
	}
	nTmp += m_ctlScroll.GetScrollPos ();
	nTmp = max (nTmp, 0);
	m_ctlScroll.SetScrollPos (nTmp);
	m_ctlMapParts.SetScrollPos (nTmp);

	return CDlgAdminBase::OnMouseWheel (nFlags, zDelta, pt);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::OnSelchangeType								 */
/* ���e		:�C�x���g�n���h��(CBN_SELCHANGE)								 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnSelchangeType()
{
	UpdateData ();

	m_nState		= 0;
	m_nSelectType	= m_cbType.GetCurSel ();

	switch (m_nSelectType) {
	case 1:		/* �ǉ� */
		RenewMessage ("�ǉ�����ꏊ���N���b�N���Ă�������");
		break;
	case 2:		/* �ҏW */
		RenewMessage ("�ҏW����p�[�c���E�N���b�N���Ă�������");
		break;
	case 3:		/* �ړ� */
		RenewMessage ("�ړ�����p�[�c���N���b�N���Ă�������");
		break;
	case 4:		/* �폜 */
		RenewMessage ("�폜����p�[�c���N���b�N���Ă�������");
		break;
	case 5:		/* �R�s�[ */
		RenewMessage ("�R�s�[���̃p�[�c���N���b�N���Ă�������");
		break;
	default:
		RenewMessage ("");
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::RenewMessage								 */
/* ���e		:���b�Z�[�W�����X�V												 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CDlgAdminMapParts::RenewMessage(LPCSTR pszMsg)
{
	m_strMsg = pszMsg;
	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::RenewPartsInfo								 */
/* ���e		:�p�[�c�����X�V												 */
/* ���t		:2007/06/11														 */
/* ========================================================================= */

void CDlgAdminMapParts::RenewPartsInfo(void)
{
	PCInfoMapParts pInfoMapParts;

	m_strAttr  = "����:";
	m_strAttr2 = "�i�߂Ȃ�����:";

	pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (m_dwSelectPartsID);
	if (pInfoMapParts == NULL) {
		return;
	}

	if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_BLOCK) {
		m_strAttr += "�Ԃ��� ";
	}
	if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_PILE) {
		m_strAttr += "�d�Ȃ� ";
	}
	if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_PILEBACK) {
		m_strAttr += "���n���d�˂� ";
	}

	if (pInfoMapParts->m_byBlockDirection & BIT_PARTSBLOCK_UP) {
		m_strAttr2 += "�� ";
	}
	if (pInfoMapParts->m_byBlockDirection & BIT_PARTSBLOCK_DOWN) {
		m_strAttr2 += "�� ";
	}
	if (pInfoMapParts->m_byBlockDirection & BIT_PARTSBLOCK_LEFT) {
		m_strAttr2 += "�� ";
	}
	if (pInfoMapParts->m_byBlockDirection & BIT_PARTSBLOCK_RIGHT) {
		m_strAttr2 += "�� ";
	}

	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::MakePartsImage								 */
/* ���e		:�p�[�c�ꗗ�摜���쐬											 */
/* ���t		:2007/04/25														 */
/* ========================================================================= */

void CDlgAdminMapParts::MakePartsImage(void)
{
	SCROLLINFO stScrollInfo;
	CRect rc;

	m_ctlMapParts.Renew ();
	m_ctlMapParts.GetWindowRect (rc);

	m_ctlScroll.GetScrollInfo (&stScrollInfo);
	stScrollInfo.nPage	= (rc.Height () / 16) - 1;
	stScrollInfo.nMax	= 100;
	m_ctlScroll.SetScrollInfo (&stScrollInfo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapParts::GetSelectMapPartsID							 */
/* ���e		:�I�𒆂̃}�b�v�p�[�cID���擾									 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

DWORD CDlgAdminMapParts::GetSelectMapPartsID(void)
{
	DWORD dwRet;
	PCInfoMapParts pInfoMapParts;

	dwRet = 0;

	pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (&m_ptCursor);
	if (pInfoMapParts == NULL) {
		goto Exit;
	}

	dwRet = pInfoMapParts->m_dwPartsID;
Exit:
	return dwRet;
}

/* Copyright(C)URARA-works 2007 */
