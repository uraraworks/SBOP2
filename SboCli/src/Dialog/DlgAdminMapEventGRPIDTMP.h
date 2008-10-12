/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEventGRPIDTMP.h									 */
/* ���e			:�}�b�v�C�x���g(�ꎞ�摜�ݒ�)�_�C�A���O�N���X ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/12													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminMapEventNONE.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminMapEventGRPIDTMP : public CDlgAdminMapEventNONE
{
public:
			CDlgAdminMapEventGRPIDTMP(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminMapEventGRPIDTMP();						/* �f�X�g���N�^ */

	void	Set		(CInfoMapEventBase *pSrc);							/* �ݒ� */
	void	Get		(CInfoMapEventBase *pDst);							/* �擾 */



public:
	//{{AFX_DATA(CDlgAdminMapEventGRPIDTMP)
	enum { IDD = IDD_MAP_EVENT_GRPIDTMP };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEventGRPIDTMP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEventGRPIDTMP)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox	m_ctlIDMain;
	int m_nSelect;
	DWORD m_dwIDSub;
} CDlgAdminMapEventGRPIDTMP, *PCDlgAdminMapEventGRPIDTMP;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
