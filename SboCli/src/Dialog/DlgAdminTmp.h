/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTmp.h												 */
/* ���e			:�_�C�A���O�N���X ��`�t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminTmp : public CDlgAdminBase
{
public:
			CDlgAdminTmp(CWnd* pParent = NULL);					/* �R���X�g���N�^ */
	virtual ~CDlgAdminTmp();									/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:


protected:



public:
	//{{AFX_DATA(CDlgAdminTmp)
	enum { IDD = IDD_ };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTmp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTmp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminTmp, *PCDlgAdminTmp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
