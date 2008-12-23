/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventMSG.h										 */
/* ���e			:��b�C�x���g�ݒ�(���b�Z�[�W�\��)�_�C�A���O�N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventMSG : public CDlgAdminBase
{
public:
			CDlgAdminTalkEventMSG(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminTalkEventMSG();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:


protected:



public:
	//{{AFX_DATA(CDlgAdminTalkEventMSG)
	enum { IDD = IDD_TALKEVENT_MSG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventMSG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventMSG)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminTalkEventMSG, *PCDlgAdminTalkEventMSG;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
