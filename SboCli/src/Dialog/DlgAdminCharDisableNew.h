/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharDisableNew.h									 */
/* ���e			:����MAC�A�h���X�̒ǉ��_�C�A���O�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/04/11													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharDisableNew : public CDlgAdminBase
{
public:
			CDlgAdminCharDisableNew(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharDisableNew();							/* �f�X�g���N�^ */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminCharDisableNew)
	enum { IDD = IDD_CHAR_DISABLE_NEW };
	CString	m_strMacAddress;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharDisableNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharDisableNew)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharDisableNew, *PCDlgAdminCharDisableNew;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2009 */
