/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventMSG.h										 */
/* ���e			:��b�C�x���g�ݒ�(���b�Z�[�W�\��)�_�C�A���O�N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminTalkEventNONE.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventMSG : public CDlgAdminTalkEventNONE
{
public:
			CDlgAdminTalkEventMSG(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminTalkEventMSG();							/* �f�X�g���N�^ */

	void	Set	(CInfoTalkEventBase *pSrc);								/* �ݒ肩���ʂɔ��f */
	void	Get	(CInfoTalkEventBase *pDst);								/* ��ʂ���ݒ�ɔ��f */


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
public:
	CString m_strMsg;
} CDlgAdminTalkEventMSG, *PCDlgAdminTalkEventMSG;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
