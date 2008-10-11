/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharAccountInfo.h									 */
/* ���e			:�A�J�E���g���_�C�A���O�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/06													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharAccountInfo : public CDlgAdminBase
{
public:
			CDlgAdminCharAccountInfo(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharAccountInfo();						/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	Renew		(void);											/* �X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:
	DWORD	m_dwAccountID;			/* �ҏW���̃A�J�E���gID */



public:
	//{{AFX_DATA(CDlgAdminCharAccountInfo)
	enum { IDD = IDD_CHAR_ACCOUNTINFO };
	CString	m_strCharName;
	CString	m_strAccountID;
	CString	m_strAccount;
	CString	m_strPassword;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharAccountInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharAccountInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharAccountInfo, *PCDlgAdminCharAccountInfo;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
