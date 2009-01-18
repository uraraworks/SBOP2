/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharModifySkill.h									 */
/* ���e			:�I���L�������ҏW[�����X�L���̐ݒ�]�_�C�A���O�N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/01/18													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoCharCli;
class CLibInfoSkill;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharModifySkill : public CDlgAdminBase
{
public:
			CDlgAdminCharModifySkill(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharModifySkill();					/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);						/* ������ */
	void	Renew		(void);										/* �X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);			/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */


protected:
	CInfoCharCli	*m_pInfoChar;					/* �ҏW���̃L������� */
	CLibInfoSkill	*m_pLibInfoSkill;				/* �X�L����񃉃C�u���� */



public:
	//{{AFX_DATA(CDlgAdminCharModifySkill)
	enum { IDD = IDD_CHAR_MODIFY_SKILL };
	CListCtrl	m_List;
	CComboBox	m_Combo;
	CString	m_strCharName;
	CString	m_strCharID;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharModifySkill)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharModifySkill)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharModifySkill, *PCDlgAdminCharModifySkill;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2009 */
