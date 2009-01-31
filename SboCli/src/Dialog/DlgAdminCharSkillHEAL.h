/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharSkillHEAL.h									 */
/* ���e			:�X�L��(��)�_�C�A���O�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/01/22													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminCharSkillNONE.h"

class CInfoSkillBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharSkillHEAL : public CDlgAdminCharSkillNONE
{
public:
			CDlgAdminCharSkillHEAL(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharSkillHEAL();							/* �f�X�g���N�^ */

	virtual void	Init	(CMgrData *pMgrData);						/* ������ */
	virtual void	Set		(CInfoSkillBase *pSrc);						/* �ݒ� */
	virtual void	Get		(CInfoSkillBase *pDst);						/* �擾 */



public:
	//{{AFX_DATA(CDlgAdminCharSkillHEAL)
	enum { IDD = IDD_CHAR_SKILL_HEAL };
	CComboBox	m_ctlArea;
	CComboBox	m_ctlHealType;
	CComboBox	m_ctlHitEffect;
	DWORD	m_dwValue1;
	DWORD	m_dwValue2;
	DWORD	m_dwDistance;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharSkillHEAL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharSkillHEAL)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharSkillHEAL, *PCDlgAdminCharSkillHEAL;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2009 */
