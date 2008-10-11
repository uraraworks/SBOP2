/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminSelectEffect.h										 */
/* ���e			:�G�t�F�N�g�I���_�C�A���O�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "ListCtrlEffectInfo.h"

class CLibInfoEffect;
class CInfoEffect;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminSelectEffect : public CDlgAdminBase
{
public:
			CDlgAdminSelectEffect(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminSelectEffect();							/* �f�X�g���N�^ */

	DWORD	GetSelectID	(void);				/* �I�����ꂽ�G�t�F�N�gID���擾 */


protected:
	DWORD	m_dwSelectID;					/* �I�����ꂽ�G�t�F�N�gID */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminSelectEffect)
	enum { IDD = IDD_SELECT_EFFECT };
	CListCtrlEffectInfo	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminSelectEffect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminSelectEffect)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminSelectEffect, *PCDlgAdminSelectEffect;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
