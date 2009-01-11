/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharSkillMOVEATACK.h								 */
/* ���e			:�X�L��(�ړ����čU��)�_�C�A���O�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/01/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminCharSkillNONE.h"

class CInfoSkillBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharSkillMOVEATACK : public CDlgAdminCharSkillNONE
{
public:
			CDlgAdminCharSkillMOVEATACK(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharSkillMOVEATACK();						/* �f�X�g���N�^ */

	virtual void	Init	(CMgrData *pMgrData);						/* ������ */
	virtual void	Set		(CInfoSkillBase *pSrc);						/* �ݒ� */
	virtual void	Get		(CInfoSkillBase *pDst);						/* �擾 */


protected:



public:
	//{{AFX_DATA(CDlgAdminCharSkillMOVEATACK)
	enum { IDD = IDD_CHAR_SKILL_MOVEATACK };
	CComboBox	m_ctlTarget;
	CComboBox	m_ctlUp;
	CComboBox	m_ctlDown;
	CComboBox	m_ctlLeft;
	CComboBox	m_ctlRight;
	CComboBox	m_ctlPutType;
	DWORD	m_dwAliveTime;
	DWORD	m_dwWaitTime;
	DWORD	m_dwValue;
	DWORD	m_dwDistance;
	BOOL	m_bHitQuit;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharSkillMOVEATACK)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharSkillMOVEATACK)
	virtual BOOL OnInitDialog();
	virtual afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharSkillMOVEATACK, *PCDlgAdminCharSkillMOVEATACK;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2009 */
