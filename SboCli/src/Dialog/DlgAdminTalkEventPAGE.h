/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventPAGE.h									 */
/* ���e			:��b�C�x���g�ݒ�(�y�[�W�؂�ւ�)�_�C�A���O�N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminTalkEventNONE.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventPAGE : public CDlgAdminTalkEventNONE
{
public:
			CDlgAdminTalkEventPAGE(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminTalkEventPAGE();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:


protected:



public:
	//{{AFX_DATA(CDlgAdminTalkEventPAGE)
	enum { IDD = IDD_TALKEVENT_PAGE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventPAGE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventPAGE)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminTalkEventPAGE, *PCDlgAdminTalkEventPAGE;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
