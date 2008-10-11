/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharModifyTypeBase.h								 */
/* ���e			:�L������� �ړ���ʂ̐ݒ�_�C�A���O�N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/12													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoCharCli;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharModifyTypeBase : public CDlgAdminBase
{
public:
			CDlgAdminCharModifyTypeBase(CWnd* pParent = NULL);						/* �R���X�g���N�^ */
			CDlgAdminCharModifyTypeBase(int nResourceID, CWnd* pParent = NULL);
	virtual ~CDlgAdminCharModifyTypeBase();											/* �f�X�g���N�^ */

	virtual void	Set	(CInfoCharCli *pSrc);					/* �ݒ� */
	virtual void	Get	(CInfoCharCli *pDst);					/* �擾 */



public:
	//{{AFX_DATA(CDlgAdminCharModifyTypeBase)
	enum { IDD = IDD_CHAR_MODIFY_TYPE_PUTNPC };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharModifyTypeBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharModifyTypeBase)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharModifyTypeBase, *PCDlgAdminCharModifyTypeBase;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
