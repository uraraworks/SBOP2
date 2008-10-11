/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapObjectEdit.h									 */
/* ���e			:�}�b�v�I�u�W�F�N�g�ҏW�_�C�A���O�N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/09/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CDlgAdminMapObjectEdit : public CDlgAdminBase
{
public:
			CDlgAdminMapObjectEdit(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual	~CDlgAdminMapObjectEdit();						/* �f�X�g���N�^ */

	int DoModal (CMgrData *pMgrData);								/* ���[�_���_�C�A���O�\�� */


protected:


public:



public:
	//{{AFX_DATA(CDlgAdminMapObjectEdit)
	enum { IDD = IDD_MAP_OBJECT_EDIT };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapObjectEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapObjectEdit)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
