/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemWeaponNew.h									 */
/* ���e			:�A�C�e��������ݒ�_�C�A���O�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoItemWeapon;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminItemWeaponNew : public CDlgAdminBase
{
public:
			CDlgAdminItemWeaponNew(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminItemWeaponNew();							/* �f�X�g���N�^ */

	void	Set		(CInfoItemWeapon *pSrc);							/* �ҏW���e��ݒ� */
	void	Get		(CInfoItemWeapon *&pDst);							/* �ҏW���e���擾 */


protected:
	void	SetMotionType		(DWORD dwMotionType);					/* �g�p�\�ȍU�����[�V������ݒ� */
	DWORD	GetMotionType		(void);									/* �g�p�\�ȍU�����[�V�������擾 */
	void	SetMotionTypeStand	(DWORD dwMotionID);						/* �퓬���[�h���̗������[�V������ݒ� */
	DWORD	GetMotionTypeStand	(void);									/* �퓬���[�h���̗������[�V�������擾 */
	void	SetMotionTypeWalk	(DWORD dwMotionID);						/* �퓬���[�h���̂��葫���[�V������ݒ� */
	DWORD	GetMotionTypeWalk	(void);									/* �퓬���[�h���̂��葫���[�V�������擾 */
	void	RenewList			(CListBox *pList, ARRAYDWORD *pSrc);	/* ���X�g���X�V */


protected:
	DWORD		m_dwMotionType,			/* �g�p�\�ȍU�����[�V���� */
				m_dwMotionTypeStand,	/* �퓬���[�h���̗������[�V���� */
				m_dwMotionTypeWalk;		/* �퓬���[�h���̂��葫���[�V���� */
	ARRAYDWORD	m_adwEffectIDAtack,		/* �ʏ�U�����̃G�t�F�N�gID */
				m_adwEffectIDCritical;	/* �N���e�B�J�����̃G�t�F�N�gID */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminItemWeaponNew)
	enum { IDD = IDD_ITEMWEAPONINFO_NEW };
	CComboBox	m_ctlStandMotion;
	CComboBox	m_ctlWalkMotion;
	BOOL	m_bSwing;
	BOOL	m_bPoke;
	BOOL	m_bBow;
	BOOL	m_bBlow;
	BOOL	m_bFishing;
	CString	m_strName;
	CListBox m_ctlListNormal;
	CListBox m_ctlListCritical;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemWeaponNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemWeaponNew)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddNormal();
	afx_msg void OnBnClickedDelNormal();
	afx_msg void OnBnClickedAddCritical();
	afx_msg void OnBnClickedDelCritical();
} CDlgAdminItemWeaponNew, *PCDlgAdminItemWeaponNew;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
