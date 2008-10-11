/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemTypeNewHP.h									 */
/* ���e			:�A�C�e�����(HP����)�ݒ�_�C�A���O�N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/15													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoItemTypeBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminItemTypeNewHP : public CDlgAdminBase
{
public:
			CDlgAdminItemTypeNewHP(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminItemTypeNewHP();							/* �f�X�g���N�^ */

	void	Set		(CInfoItemTypeBase *pSrc);							/* �ҏW���e��ݒ� */
	void	Get		(CInfoItemTypeBase *&pDst);							/* �ҏW���e���擾 */


protected:
	void	SetTarget	(BYTE byTarget);			/* �g�p�Ώۂ�ݒ� */
	BYTE	GetTarget	(void);						/* �g�p�Ώۂ��擾 */
	void	SetArea		(BYTE byArea);				/* �g�p�͈͂�ݒ� */
	BYTE	GetArea		(void);						/* �g�p�͈͂��擾 */


protected:
	BYTE	m_byTarget;				/* �g�p�Ώ� */
	BYTE	m_byArea;				/* �g�p�͈� */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminItemTypeNewHP)
	enum { IDD = IDD_ITEMTYPE_NEW_HP };
	CComboBox	m_ctlTarget;
	CComboBox	m_ctlArea;
	int m_nValue1;
	int m_nValue2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemTypeNewHP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemTypeNewHP)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemTypeNewHP, *PCDlgAdminItemTypeNewHP;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
