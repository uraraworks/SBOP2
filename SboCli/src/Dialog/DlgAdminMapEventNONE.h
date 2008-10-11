/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEventNONE.h										 */
/* ���e			:�}�b�v�C�x���g(���ݒ�)�_�C�A���O�N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/26													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoMapEventBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminMapEventNONE : public CDlgAdminBase
{
public:
			CDlgAdminMapEventNONE(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminMapEventNONE();							/* �f�X�g���N�^ */

	virtual void	Init	(CMgrData *pMgrData);						/* ������ */
	virtual void	Set		(CInfoMapEventBase *pSrc);					/* �ݒ� */
	virtual void	Get		(CInfoMapEventBase *pDst);					/* �擾 */


protected:
	int		m_nResourceID;			/* �_�C�A���O���\�[�XID */



public:
	//{{AFX_DATA(CDlgAdminMapEventNONE)
	enum { IDD = IDD_MAP_EVENT_NONE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEventNONE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEventNONE)
	virtual BOOL OnInitDialog();
	virtual afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminMapEventNONE, *PCDlgAdminMapEventNONE;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
