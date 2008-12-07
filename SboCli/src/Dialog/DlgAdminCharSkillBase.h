/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharSkillBase.h									 */
/* ���e			:�X�L���ҏW�_�C�A���O�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoSkillBase;
class CDlgAdminCharSkillNONE;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharSkillBase : public CDlgAdminBase
{
public:
			CDlgAdminCharSkillBase(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharSkillBase();						/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData, CWnd **pWndNotify);	/* ������ */
	void	Get			(CInfoSkillBase *&pDst);					/* �ҏW���e���擾 */
	void	SetModify	(CInfoSkillBase *pSrc);						/* �ҏW���[�h�Ƃ��Đݒ� */


protected:


protected:
	int			m_nType;					/* �X�L����� */
	BOOL		m_bModeModify;				/* �ҏW���[�h���� */
	CWnd		**m_ppWndNotify;			/* �ʒm��E�B���h�E */
	CDlgAdminCharSkillNONE	*m_pDlgType;	/* �ҏW���̃C�x���g��ʃ_�C�A���O */
	CInfoSkillBase		*m_pInfo;			/* �ҏW���̃X�L����� */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminCharSkillBase)
	enum { IDD = IDD_CHAR_SKILL_BASE };
	CComboBox	m_ctlType;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharSkillBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharSkillBase)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeType();
	afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharSkillBase, *PCDlgAdminCharSkillBase;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
