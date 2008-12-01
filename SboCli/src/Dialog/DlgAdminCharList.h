/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharList.h											 */
/* ���e			:�L�����ꗗ�_�C�A���O�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/01													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoCharCli;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharList : public CDlgAdminBase
{
public:
			CDlgAdminCharList(CWnd* pParent = NULL);			/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharList();								/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	Renew		(void);											/* �ꗗ���X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:


protected:
	CWnd				*m_pWndNotify;				/* �ʒm��E�B���h�E */
	CLibInfoCharCli		*m_pLibInfoChar;			/* �ҏW���̃L������� */



public:
	//{{AFX_DATA(CDlgAdminCharList)
	enum { IDD = IDD_CHAR_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharList)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRenew();
} CDlgAdminCharList, *PCDlgAdminCharList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
