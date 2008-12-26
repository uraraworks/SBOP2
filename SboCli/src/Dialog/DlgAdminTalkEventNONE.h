/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventNONE.h									 */
/* ���e			:��b�C�x���g�ݒ�(���I��)�_�C�A���O�N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoTalkEventBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventNONE : public CDlgAdminBase
{
public:
			CDlgAdminTalkEventNONE(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminTalkEventNONE();							/* �f�X�g���N�^ */

	virtual void	Init		(CMgrData *pMgrData);					/* ������ */
	virtual void	OnAdminMsg	(int nType, DWORD dwPara);				/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */
	virtual void	Set			(CInfoTalkEventBase *pSrc);				/* �ݒ肩���ʂɔ��f */
	virtual void	Get			(CInfoTalkEventBase *pDst);				/* ��ʂ���ݒ�ɔ��f */


protected:
	int		m_nResourceID;		/* �_�C�A���O���\�[�XID */


protected:



public:
	//{{AFX_DATA(CDlgAdminTalkEventNONE)
	enum { IDD = IDD_TALKEVENT_NONE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventNONE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventNONE)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminTalkEventNONE, *PCDlgAdminTalkEventNONE;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
