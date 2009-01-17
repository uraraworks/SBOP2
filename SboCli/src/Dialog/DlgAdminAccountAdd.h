/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminAccountAdd.h										 */
/* ���e			:�A�J�E���g�ǉ��_�C�A���O�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/01/17													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminAccountAdd : public CDlgAdminBase
{
public:
			CDlgAdminAccountAdd(CWnd* pParent = NULL);			/* �R���X�g���N�^ */
	virtual ~CDlgAdminAccountAdd();								/* �f�X�g���N�^ */

	void	Init	(CMgrData *pMgrData);								/* ������ */



public:
	//{{AFX_DATA(CDlgAdminAccountAdd)
	enum { IDD = IDD_ACCOUNT_ADD };
	CString	m_strAccount;
	CString	m_strPassword;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminAccountAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminAccountAdd)
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminAccountAdd, *PCDlgAdminAccountAdd;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2009 */
