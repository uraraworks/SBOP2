/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEventMOVE.h										 */
/* ���e			:�}�b�v�C�x���g(�}�b�v���ړ�)�_�C�A���O�N���X ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/24													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminMapEventNONE.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminMapEventMOVE : public CDlgAdminMapEventNONE
{
public:
			CDlgAdminMapEventMOVE(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminMapEventMOVE();							/* �f�X�g���N�^ */

	void	Set		(CInfoMapEventBase *pSrc);							/* �ݒ� */
	void	Get		(CInfoMapEventBase *pDst);							/* �擾 */



public:
	//{{AFX_DATA(CDlgAdminMapEventMOVE)
	enum { IDD = IDD_MAP_EVENT_MOVE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEventMOVE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEventMOVE)
	virtual BOOL OnInitDialog();
	virtual afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_nPosX;
	int m_nPosY;
} CDlgAdminMapEventMOVE, *PCDlgAdminMapEventMOVE;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
