/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharSkillList.h									 */
/* ���e			:�X�L�����X�g�_�C�A���O�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoSkill;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharSkillList : public CDlgAdminBase
{
public:
			CDlgAdminCharSkillList(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharSkillList();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */

	void	Renew		(void);											/* ���X�g�X�V */


protected:
	CLibInfoSkill		*m_pLibInfoSkill;		/* �X�L����� */



public:
	//{{AFX_DATA(CDlgAdminCharSkillList)
	enum { IDD = IDD_CHAR_SKILL_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharSkillList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CDlgAdminCharSkillList)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharSkillList, *PCDlgAdminCharSkillList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
