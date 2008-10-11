/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminServerInfo.h										 */
/* ���e			:�T�[�o�[���_�C�A���O�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/08													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminServerInfo : public CDlgAdminBase
{
public:
			CDlgAdminServerInfo(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminServerInfo();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);						/* ������ */



public:
	//{{AFX_DATA(CDlgAdminServerInfo)
	enum { IDD = IDD_SERVER_INFO };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminServerInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminServerInfo)
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPlaysound();
	afx_msg void OnBnClickedRenewClientversion();
} CDlgAdminServerInfo, *PCDlgAdminServerInfo;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
