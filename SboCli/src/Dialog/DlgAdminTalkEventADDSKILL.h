/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventADDSKILL.h								 */
/* ���e			:��b�C�x���g�ݒ�(�X�L���ǉ�)�_�C�A���O�N���X ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/31													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminTalkEventNONE.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventADDSKILL : public CDlgAdminTalkEventNONE
{
public:
			CDlgAdminTalkEventADDSKILL(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminTalkEventADDSKILL();						/* �f�X�g���N�^ */

	void	Set	(CInfoTalkEventBase *pSrc);								/* �ݒ肩���ʂɔ��f */
	void	Get	(CInfoTalkEventBase *pDst);								/* ��ʂ���ݒ�ɔ��f */


protected:


protected:



public:
	//{{AFX_DATA(CDlgAdminTalkEventADDSKILL)
	enum { IDD = IDD_TALKEVENT_ADDSKILL };
	CComboBox m_Combo;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventADDSKILL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventADDSKILL)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminTalkEventADDSKILL, *PCDlgAdminTalkEventADDSKILL;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
