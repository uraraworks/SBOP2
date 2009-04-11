/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharDisable.h										 */
/* ���e			:���ۈꗗ�_�C�A���O�N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/04/06													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoDisable;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharDisable : public CDlgAdminBase
{
public:
			CDlgAdminCharDisable(CWnd* pParent = NULL);			/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharDisable();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);						/* ������ */
	void	Renew		(void);										/* �ꗗ���X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:


protected:
	CWnd				*m_pWndNotify;				/* �ʒm��E�B���h�E */
	CLibInfoDisable		*m_pLibInfoDisable;			/* �ҏW���̋��ۏ�� */



public:
	//{{AFX_DATA(CDlgAdminCharDisable)
	enum { IDD = IDD_CHAR_DISABLE };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharDisable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharDisable)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRenew();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
} CDlgAdminCharDisable, *PCDlgAdminCharDisable;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2009 */
