/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharSkillNONE.h									 */
/* ���e			:�X�L��(���ݒ�)�_�C�A���O�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoSkillBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharSkillNONE : public CDlgAdminBase
{
public:
			CDlgAdminCharSkillNONE(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharSkillNONE();							/* �f�X�g���N�^ */

	virtual void	Init	(CMgrData *pMgrData);						/* ������ */
	virtual void	Set		(CInfoSkillBase *pSrc);						/* �ݒ� */
	virtual void	Get		(CInfoSkillBase *pDst);						/* �擾 */


protected:
	int		m_nResourceID;			/* �_�C�A���O���\�[�XID */



public:
	//{{AFX_DATA(CDlgAdminCharSkillNONE)
	enum { IDD = IDD_CHAR_SKILL_NONE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharSkillNONE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharSkillNONE)
	virtual BOOL OnInitDialog();
	virtual afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharSkillNONE, *PCDlgAdminCharSkillNONE;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
