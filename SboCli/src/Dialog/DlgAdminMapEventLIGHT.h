/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEventLIGHT.h									 */
/* ���e			:�}�b�v�C�x���g(����)�_�C�A���O�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminMapEventNONE.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminMapEventLIGHT : public CDlgAdminMapEventNONE
{
public:
			CDlgAdminMapEventLIGHT(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminMapEventLIGHT();							/* �f�X�g���N�^ */

	void	Set		(CInfoMapEventBase *pSrc);							/* �ݒ� */
	void	Get		(CInfoMapEventBase *pDst);							/* �擾 */



public:
	//{{AFX_DATA(CDlgAdminMapEventLIGHT)
	enum { IDD = IDD_MAP_EVENT_LIGHT };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEventLIGHT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEventLIGHT)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_nSelect;
	DWORD m_dwTime;
} CDlgAdminMapEventLIGHT, *PCDlgAdminMapEventLIGHT;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
