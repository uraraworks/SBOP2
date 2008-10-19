/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemTypeNewLIGHT.h									 */
/* ���e			:�A�C�e�����(����)�ݒ�_�C�A���O�N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/19													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoItemTypeBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminItemTypeNewLIGHT : public CDlgAdminBase
{
public:
			CDlgAdminItemTypeNewLIGHT(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminItemTypeNewLIGHT();						/* �f�X�g���N�^ */

	void	Set		(CInfoItemTypeBase *pSrc);							/* �ҏW���e��ݒ� */
	void	Get		(CInfoItemTypeBase *&pDst);							/* �ҏW���e���擾 */


public:
	//{{AFX_DATA(CDlgAdminItemTypeNewLIGHT)
	enum { IDD = IDD_ITEMTYPE_NEW_LIGHT };
	int m_nValue1;
	int m_nValue2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemTypeNewLIGHT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemTypeNewLIGHT)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemTypeNewLIGHT, *PCDlgAdminItemTypeNewLIGHT;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
