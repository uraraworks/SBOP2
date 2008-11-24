/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEventMAPMOVE.h									 */
/* ���e			:�}�b�v�C�x���g(�}�b�v�Ԉړ�)�_�C�A���O�N���X ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/27													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminMapEventNONE.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminMapEventMAPMOVE : public CDlgAdminMapEventNONE
{
public:
			CDlgAdminMapEventMAPMOVE(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminMapEventMAPMOVE();						/* �f�X�g���N�^ */

	void	Set		(CInfoMapEventBase *pSrc);							/* �ݒ� */
	void	Get		(CInfoMapEventBase *pDst);							/* �擾 */



public:
	//{{AFX_DATA(CDlgAdminMapEventMAPMOVE)
	enum { IDD = IDD_MAP_EVENT_MAPMOVE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEventMAPMOVE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEventMAPMOVE)
	virtual BOOL OnInitDialog();
	virtual afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwMapID;
	int m_nPosX;
	int m_nPosY;
	int m_nDirection;
	CComboBox m_ctlDirection;
} CDlgAdminMapEventMAPMOVE, *PCDlgAdminMapEventMAPMOVE;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
