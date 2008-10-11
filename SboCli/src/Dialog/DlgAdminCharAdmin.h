/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharAdmin.h										 */
/* ���e			:�Ǘ��Ҍ����̐ݒ�_�C�A���O�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/05													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharAdmin : public CDlgAdminBase
{
public:
			CDlgAdminCharAdmin(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharAdmin();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);						/* ������ */
	void	Renew		(void);										/* �X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:
	int				m_nAdminLevel;					/* �Ǘ��҃��x�� */
	DWORD			m_dwAccountID;					/* �ҏW���̃A�J�E���gID */



public:
	//{{AFX_DATA(CDlgAdminCharAdmin)
	enum { IDD = IDD_CHAR_ADMIN };
	CComboBox	m_ctAdminLevel;
	CString	m_strCharName;
	CString	m_strAccountID;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharAdmin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharAdmin)
	virtual BOOL OnInitDialog();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharAdmin, *PCDlgAdminCharAdmin;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
