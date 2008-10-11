/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEventINITSTATUS.h								 */
/* ���e			:�}�b�v�C�x���g(�X�e�[�^�X������)�_�C�A���O�N���X ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/05													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminMapEventNONE.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminMapEventINITSTATUS : public CDlgAdminMapEventNONE
{
public:
			CDlgAdminMapEventINITSTATUS(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminMapEventINITSTATUS();						/* �f�X�g���N�^ */

	void	Set		(CInfoMapEventBase *pSrc);							/* �ݒ� */
	void	Get		(CInfoMapEventBase *pDst);							/* �擾 */


protected:
	DWORD	m_dwEffectID;			/* �G�t�F�N�gID */



public:
	//{{AFX_DATA(CDlgAdminMapEventINITSTATUS)
	enum { IDD = IDD_MAP_EVENT_INITSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEventINITSTATUS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEventINITSTATUS)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox	m_ctlEffectID;
} CDlgAdminMapEventINITSTATUS, *PCDlgAdminMapEventINITSTATUS;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
