/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemWeaponNew.cpp									 */
/* ���e			:�A�C�e��������ݒ�_�C�A���O�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "LibInfoEffect.h"
#include "InfoItemWeapon.h"
#include "InfoMotion.h"
#include "InfoItemTypeBase.h"
#include "MgrData.h"
#include "DlgAdminSelectEffect.h"
#include "DlgAdminItemWeaponNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminItemWeaponNew::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemWeaponNew)
	DDX_Control(pDX, IDC_WALKMOTION, m_ctlWalkMotion);
	DDX_Control(pDX, IDC_STANDMOTION, m_ctlStandMotion);
	DDX_Check(pDX, IDC_SWING, m_bSwing);
	DDX_Check(pDX, IDC_POKE, m_bPoke);
	DDX_Check(pDX, IDC_BOW, m_bBow);
	DDX_Check(pDX, IDC_BLOW, m_bBlow);
	DDX_Check(pDX, IDC_FISHING, m_bFishing);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_NORMAL, m_ctlListNormal);
	DDX_Control(pDX, IDC_LIST_CRITICAL, m_ctlListCritical);
}

BEGIN_MESSAGE_MAP(CDlgAdminItemWeaponNew, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminItemWeaponNew)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD_NORMAL, &CDlgAdminItemWeaponNew::OnBnClickedAddNormal)
	ON_BN_CLICKED(IDC_DEL_NORMAL, &CDlgAdminItemWeaponNew::OnBnClickedDelNormal)
	ON_BN_CLICKED(IDC_ADD_CRITICAL, &CDlgAdminItemWeaponNew::OnBnClickedAddCritical)
	ON_BN_CLICKED(IDC_DEL_CRITICAL, &CDlgAdminItemWeaponNew::OnBnClickedDelCritical)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::CDlgAdminItemWeaponNew					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CDlgAdminItemWeaponNew::CDlgAdminItemWeaponNew(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminItemWeaponNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminItemWeaponNew)
	m_bSwing = FALSE;
	m_bPoke = FALSE;
	m_bBow = FALSE;
	m_bBlow = FALSE;
	m_bFishing = FALSE;
	m_strName = _T("");
	//}}AFX_DATA_INIT

	m_dwMotionType		= 0;
	m_dwMotionTypeStand	= 0;
	m_dwMotionTypeWalk	= 0;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::~CDlgAdminItemWeaponNew				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CDlgAdminItemWeaponNew::~CDlgAdminItemWeaponNew()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::Set									 */
/* ���e		:�ҏW���e��ݒ�													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CDlgAdminItemWeaponNew::Set(CInfoItemWeapon *pSrc)
{
	m_dwMotionType		= pSrc->m_dwMotionType;
	m_dwMotionTypeStand	= pSrc->m_dwMotionTypeStand;
	m_dwMotionTypeWalk	= pSrc->m_dwMotionTypeWalk;
	m_strName			= pSrc->m_strName;

	m_adwEffectIDAtack.Copy (&pSrc->m_adwEffectIDAtack);
	m_adwEffectIDCritical.Copy (&pSrc->m_adwEffectIDCritical);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::Get									 */
/* ���e		:�ҏW���e���擾													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CDlgAdminItemWeaponNew::Get(CInfoItemWeapon *&pDst)
{
	pDst->m_dwMotionType		= m_dwMotionType;
	pDst->m_dwMotionTypeStand	= m_dwMotionTypeStand;
	pDst->m_dwMotionTypeWalk	= m_dwMotionTypeWalk;
	pDst->m_strName				= m_strName;

	pDst->m_adwEffectIDAtack.Copy (&m_adwEffectIDAtack);
	pDst->m_adwEffectIDCritical.Copy (&m_adwEffectIDCritical);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

BOOL CDlgAdminItemWeaponNew::OnInitDialog()
{
	LPCSTR pszTmp;
	int i, nMotionID,
		anStandMotionID[] = {
			CHARMOTIONLISTID_BATTLESTAND_UP,		/* �퓬����(��) */
			CHARMOTIONLISTID_BOWBATTLESTAND_UP,		/* ���葫(��) */
			CHARMOTIONLISTID_GLOVEBATTLESTAND_UP,	/* �|�p���葫(��) */
			CHARMOTIONLISTID_FISHINGBATTLESTAND_UP,	/* �Ō��p���葫(��) */
			-1
		},
		anWalkMotionID[] = {
			CHARMOTIONLISTID_STAND_UP,				/* ����(��) */
			CHARMOTIONLISTID_BATTLEWALK_UP,			/* ���葫(��) */
			CHARMOTIONLISTID_BOWWALK_UP,			/* �|�p���葫(��) */
			CHARMOTIONLISTID_GLOVEWALK_UP,			/* �Ō��p���葫(��) */
			CHARMOTIONLISTID_FISHINGWALK_UP,		/* �ނ�p���葫(��) */
			-1
		};
	CMgrData MgrData;

	CDlgAdminBase::OnInitDialog();

	for (i = 0; ; i ++) {
		nMotionID = anStandMotionID[i];
		if (nMotionID < 0) {
			break;
		}
		pszTmp = MgrData.GetMotionName (nMotionID);
		m_ctlStandMotion.InsertString (i, pszTmp);
		m_ctlStandMotion.SetItemData (i, (DWORD)nMotionID);
	}

	for (i = 0; ; i ++) {
		nMotionID = anWalkMotionID[i];
		if (nMotionID < 0) {
			break;
		}
		pszTmp = MgrData.GetMotionName (nMotionID);
		m_ctlWalkMotion.InsertString (i, pszTmp);
		m_ctlWalkMotion.SetItemData (i, (DWORD)nMotionID);
	}

	SetMotionType		(m_dwMotionType);
	SetMotionTypeStand	(m_dwMotionTypeStand);
	SetMotionTypeWalk	(m_dwMotionTypeWalk);

	RenewList (&m_ctlListNormal, &m_adwEffectIDAtack);
	RenewList (&m_ctlListCritical, &m_adwEffectIDCritical);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::PostNcDestroy							 */
/* ���e		:�I������														 */
/* ���t		:2007/09/30														 */
/* ========================================================================= */

void CDlgAdminItemWeaponNew::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::OnBnClickedAddNormal					 */
/* ���e		:�{�^���n���h��(�ǉ�(�ʏ펞))									 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CDlgAdminItemWeaponNew::OnBnClickedAddNormal()
{
	int nResult;
	DWORD dwID;
	CDlgAdminSelectEffect Dlg(this);

	Dlg.Init (m_pMgrData);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}
	dwID = Dlg.GetSelectID ();
	if (dwID == 0) {
		return;
	}
	m_adwEffectIDAtack.Add (dwID);
	RenewList (&m_ctlListNormal, &m_adwEffectIDAtack);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::OnBnClickedDelNormal					 */
/* ���e		:�{�^���n���h��(�폜(�ʏ펞))									 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CDlgAdminItemWeaponNew::OnBnClickedDelNormal()
{
	int nSelect;

	nSelect = m_ctlListNormal.GetCurSel ();
	if (nSelect < 0) {
		return;
	}
	m_adwEffectIDAtack.RemoveAt (nSelect);
	RenewList (&m_ctlListNormal, &m_adwEffectIDAtack);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::OnBnClickedAddCritical					 */
/* ���e		:�{�^���n���h��(�ǉ�(�N���e�B�J����))							 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CDlgAdminItemWeaponNew::OnBnClickedAddCritical()
{
	int nResult;
	DWORD dwID;
	CDlgAdminSelectEffect Dlg(this);

	Dlg.Init (m_pMgrData);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}
	dwID = Dlg.GetSelectID ();
	if (dwID == 0) {
		return;
	}
	m_adwEffectIDCritical.Add (dwID);
	RenewList (&m_ctlListCritical, &m_adwEffectIDCritical);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::OnBnClickedDelCritical					 */
/* ���e		:�{�^���n���h��(�폜(�N���e�B�J����))							 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CDlgAdminItemWeaponNew::OnBnClickedDelCritical()
{
	int nSelect;

	nSelect = m_ctlListCritical.GetCurSel ();
	if (nSelect < 0) {
		return;
	}
	m_adwEffectIDCritical.RemoveAt (nSelect);
	RenewList (&m_ctlListNormal, &m_adwEffectIDCritical);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::OnOK									 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CDlgAdminItemWeaponNew::OnOK()
{
	UpdateData ();

	m_dwMotionType		= GetMotionType ();
	m_dwMotionTypeStand	= GetMotionTypeStand ();
	m_dwMotionTypeWalk	= GetMotionTypeWalk ();

	CDlgAdminBase::EndDialog (IDOK);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::SetMotionType							 */
/* ���e		:�g�p�\�ȍU�����[�V������ݒ�									 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CDlgAdminItemWeaponNew::SetMotionType(DWORD dwMotionType)
{
	m_bSwing	= FALSE;
	m_bPoke		= FALSE;
	m_bBow		= FALSE;
	m_bBlow		= FALSE;	
	m_bFishing	= FALSE;
	if (dwMotionType & INFOITEMARMS_MOTION_SWING)	{	m_bSwing	= TRUE;	}
	if (dwMotionType & INFOITEMARMS_MOTION_POKE)	{	m_bPoke		= TRUE;	}
	if (dwMotionType & INFOITEMARMS_MOTION_BOW)		{	m_bBow		= TRUE;	}
	if (dwMotionType & INFOITEMARMS_MOTION_BLOW)	{	m_bBlow		= TRUE;	}
	if (dwMotionType & INFOITEMARMS_MOTION_FISHING)	{	m_bFishing	= TRUE;	}

	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::GetMotionType							 */
/* ���e		:�g�p�\�ȍU�����[�V�������擾									 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

DWORD CDlgAdminItemWeaponNew::GetMotionType(void)
{
	DWORD dwRet;

	dwRet = 0;
	UpdateData ();

	if (m_bSwing)	{	dwRet |= INFOITEMARMS_MOTION_SWING;		}
	if (m_bPoke)	{	dwRet |= INFOITEMARMS_MOTION_POKE;		}
	if (m_bBow)		{	dwRet |= INFOITEMARMS_MOTION_BOW;		}
	if (m_bBlow)	{	dwRet |= INFOITEMARMS_MOTION_BLOW;		}
	if (m_bFishing)	{	dwRet |= INFOITEMARMS_MOTION_FISHING;	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::SetMotionTypeStand						 */
/* ���e		:�퓬���[�h���̗������[�V������ݒ�								 */
/* ���t		:2007/12/09														 */
/* ========================================================================= */

void CDlgAdminItemWeaponNew::SetMotionTypeStand(DWORD dwMotionID)
{
	int i, nCount, nSelect;
	DWORD dwItemData;

	nSelect = 0;

	nCount = m_ctlStandMotion.GetCount ();
	for (i = 0; i < nCount; i ++) {
		dwItemData = m_ctlStandMotion.GetItemData (i);
		if (dwItemData != dwMotionID) {
			continue;
		}
		nSelect = i;
		break;
	}
	m_ctlStandMotion.SetCurSel (nSelect);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::GetMotionTypeStand						 */
/* ���e		:�퓬���[�h���̗������[�V�������擾								 */
/* ���t		:2007/12/09														 */
/* ========================================================================= */

DWORD CDlgAdminItemWeaponNew::GetMotionTypeStand(void)
{
	int nSelect;
	DWORD dwRet;

	dwRet = 0;

	nSelect = m_ctlStandMotion.GetCurSel ();
	if (nSelect < 0) {
		goto Exit;
	}

	dwRet = m_ctlStandMotion.GetItemData (nSelect);
Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::SetMotionTypeWalk						 */
/* ���e		:�퓬���[�h���̂��葫���[�V������ݒ�							 */
/* ���t		:2007/12/09														 */
/* ========================================================================= */

void CDlgAdminItemWeaponNew::SetMotionTypeWalk(DWORD dwMotionID)
{
	int i, nCount, nSelect;
	DWORD dwItemData;

	nSelect = 0;

	nCount = m_ctlWalkMotion.GetCount ();
	for (i = 0; i < nCount; i ++) {
		dwItemData = m_ctlWalkMotion.GetItemData (i);
		if (dwItemData != dwMotionID) {
			continue;
		}
		nSelect = i;
		break;
	}
	m_ctlWalkMotion.SetCurSel (nSelect);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::GetMotionTypeWalk						 */
/* ���e		:�퓬���[�h���̂��葫���[�V�������擾							 */
/* ���t		:2007/12/09														 */
/* ========================================================================= */

DWORD CDlgAdminItemWeaponNew::GetMotionTypeWalk(void)
{
	int nSelect;
	DWORD dwRet;

	dwRet = 0;

	nSelect = m_ctlWalkMotion.GetCurSel ();
	if (nSelect < 0) {
		goto Exit;
	}

	dwRet = m_ctlWalkMotion.GetItemData (nSelect);
Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponNew::RenewList								 */
/* ���e		:���X�g���X�V													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CDlgAdminItemWeaponNew::RenewList(CListBox *pList, ARRAYDWORD *pSrc)
{
	int i, nCount;
	DWORD dwEffectID;
	PCLibInfoEffect pLibInfoEffect;
	CmyString strTmp;

	pLibInfoEffect = m_pMgrData->GetLibInfoEffect ();
	pList->ResetContent ();

	nCount = pSrc->GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwEffectID = pSrc->GetAt (i);
		pLibInfoEffect->GetName (dwEffectID, strTmp);
		pList->InsertString (i, (LPCSTR)strTmp);
		pList->SetItemData (i, dwEffectID);
	}
}

/* Copyright(C)URARA-works 2008 */
