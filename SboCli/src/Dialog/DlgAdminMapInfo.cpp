/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapInfo.cpp										 */
/* ���e			:�}�b�v���ҏW�_�C�A���O�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "InfoMapBase.h"
#include "MgrSound.h"
#include "PacketADMIN_MAP_RENEWMAPSIZE.h"
#include "PacketADMIN_MAP_SETMAPNAME.h"
#include "MgrData.h"
#include "DlgAdminMapInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminMapInfo::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapInfo)
	DDX_Text(pDX, IDC_MAPSIZE, m_strMapSize);
	DDX_Text(pDX, IDC_VALUE, m_nValue);
	DDX_Text(pDX, IDC_MAPID, m_strMapID);
	DDX_Text(pDX, IDC_MAPNAME, m_strMapName);
	DDX_Check(pDX, IDC_DARK, m_bDark);
	DDX_Control(pDX, IDC_BGMID, m_cmbBGMID);
	DDX_Control(pDX, IDC_WEATHERTYPE, m_cmbWeatherType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapInfo, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapInfo)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_CHANGEMAPNAME, OnChangemapname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminMapInfo::CDlgAdminMapInfo								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

CDlgAdminMapInfo::CDlgAdminMapInfo(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapInfo)
	m_strMapSize = _T("");
	m_nValue = 0;
	m_strMapID = _T("");
	m_strMapName = _T("");
	m_bDark = FALSE;
	//}}AFX_DATA_INIT

	m_pInfoMap = NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapInfo::~CDlgAdminMapInfo							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

CDlgAdminMapInfo::~CDlgAdminMapInfo()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapInfo::Init											 */
/* ���e		:������															 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

void CDlgAdminMapInfo::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pInfoMap = m_pMgrData->GetMap ();

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminMapInfo::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapInfo::Renew										 */
/* ���e		:�X�V															 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

void CDlgAdminMapInfo::Renew(void)
{
	if (m_pInfoMap == NULL) {
		return;
	}

	m_strMapSize.	Format ("(%d, %d)", m_pInfoMap->m_sizeMap.cx, m_pInfoMap->m_sizeMap.cy);
	m_strMapID.		Format ("%u", m_pInfoMap->m_dwMapID);

	m_strMapName = m_pInfoMap->m_strMapName;

	SelectCmb (&m_cmbBGMID,			m_pInfoMap->m_dwBGMID);
	SelectCmb (&m_cmbWeatherType,	m_pInfoMap->m_dwWeatherType);
	m_bDark = (m_pInfoMap->m_byLevel == 0) ? FALSE : TRUE;			/* ���邳���x�� */

	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapInfo::OnAdminMsg									 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

void CDlgAdminMapInfo::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_RENEWMAPINFO:	/* �}�b�v���X�V */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapInfo::OnInitDialog									 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

BOOL CDlgAdminMapInfo::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog ();

	CheckRadioButton (IDC_UP, IDC_RIGHT, IDC_UP);

	m_cmbBGMID.InsertString (0, "����");
	m_cmbBGMID.InsertString (1, "daichi_s.ogg");
	m_cmbBGMID.SetItemData	(1, BGMID_DAICHI_S);
	m_cmbBGMID.InsertString (2, "hisyou.ogg");
	m_cmbBGMID.SetItemData	(2, BGMID_HISYOU);
	m_cmbBGMID.InsertString (3, "suisha.ogg");
	m_cmbBGMID.SetItemData	(3, BGMID_SUISHA);
	m_cmbBGMID.InsertString (4, "fairytale.ogg");
	m_cmbBGMID.SetItemData	(4, BGMID_FAIRYTALE);
	m_cmbBGMID.InsertString (5, "tabla_image.ogg");
	m_cmbBGMID.SetItemData	(5, BGMID_TABLA_IMAGE);

	m_cmbWeatherType.InsertString (0, "����");
	m_cmbWeatherType.InsertString (1, "�_");
	m_cmbWeatherType.SetItemData  (1, WEATHERTYPE_CLOUD);
	m_cmbWeatherType.InsertString (2, "��");
	m_cmbWeatherType.SetItemData  (2, WEATHERTYPE_MISTY);

	Renew ();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapInfo::OnAdd										 */
/* ���e		:�{�^���n���h��(���₷)											 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

void CDlgAdminMapInfo::OnAdd()
{
	int nResult, nDirection;
	CPacketADMIN_MAP_RENEWMAPSIZE Packet;

	if (m_pInfoMap == NULL) {
		return;
	}

	UpdateData ();
	if (m_nValue <= 0) {
		return;
	}

	nDirection = 0;
	nResult = GetCheckedRadioButton (IDC_UP, IDC_RIGHT);
	switch (nResult) {
	case IDC_UP:
		nDirection = 0;
		break;
	case IDC_DOWN:
		nDirection = 1;
		break;
	case IDC_LEFT:
		nDirection = 2;
		break;
	case IDC_RIGHT:
		nDirection = 3;
		break;
	default:
		return;
	}
	Packet.Make (m_pInfoMap->m_dwMapID, nDirection, m_nValue);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapInfo::OnDel										 */
/* ���e		:�{�^���n���h��(���炷)											 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

void CDlgAdminMapInfo::OnDel()
{
	int nResult, nDirection;
	CPacketADMIN_MAP_RENEWMAPSIZE Packet;

	if (m_pInfoMap == NULL) {
		return;
	}

	UpdateData ();
	if (m_nValue <= 0) {
		return;
	}

	nDirection = 0;
	nResult = GetCheckedRadioButton (IDC_UP, IDC_RIGHT);
	switch (nResult) {
	case IDC_UP:
		nDirection = 0;
		break;
	case IDC_DOWN:
		nDirection = 1;
		break;
	case IDC_LEFT:
		nDirection = 2;
		break;
	case IDC_RIGHT:
		nDirection = 3;
		break;
	default:
		return;
	}
	Packet.Make (m_pInfoMap->m_dwMapID, nDirection, m_nValue * -1);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapInfo::OnChangemapname								 */
/* ���e		:�{�^���n���h��(�ύX)											 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

void CDlgAdminMapInfo::OnChangemapname()
{
	int nSelect;
	CPacketADMIN_MAP_SETMAPNAME Packet;

	if (m_pInfoMap == NULL) {
		return;
	}

	UpdateData ();

	nSelect = m_cmbBGMID.GetCurSel ();
	m_pInfoMap->m_dwBGMID = m_cmbBGMID.GetItemData (nSelect);				/* BGMID */
	nSelect = m_cmbWeatherType.GetCurSel ();
	m_pInfoMap->m_dwWeatherType = m_cmbWeatherType.GetItemData (nSelect);	/* �V�C��� */
	m_pInfoMap->m_byLevel = (m_bDark) ? 100 : 0;							/* ���邳���x�� */

	Packet.Make (m_pInfoMap->m_dwMapID, m_pInfoMap->m_dwBGMID, m_pInfoMap->m_dwWeatherType, m_pInfoMap->m_byLevel, (LPCSTR)m_strMapName);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2007 */
