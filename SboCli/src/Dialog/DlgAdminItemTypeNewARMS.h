/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemTypeNewARMS.h									 */
/* ���e			:�A�C�e�����(������)�ݒ�_�C�A���O�N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/08													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoItemTypeBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminItemTypeNewARMS : public CDlgAdminBase
{
public:
			CDlgAdminItemTypeNewARMS(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminItemTypeNewARMS();						/* �f�X�g���N�^ */

	void	Set		(CInfoItemTypeBase *pSrc);							/* �ҏW���e��ݒ� */
	void	Get		(CInfoItemTypeBase *&pDst);							/* �ҏW���e���擾 */


protected:
	void	SetWeaponType	(DWORD dwWeaponInfoID);		/* �����ʂ�ݒ� */
	DWORD	GetWeaponType	(void);						/* �����ʂ��擾 */


protected:
	DWORD	m_dwWeaponInfoID;				/* ������ID */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminItemTypeNewARMS)
	enum { IDD = IDD_ITEMTYPE_NEW_ARMS };
	CComboBox	m_ctlWeaponType;
	DWORD m_dwValue;
	DWORD m_dwMoveWait;
	DWORD m_dwMoveCount;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemTypeNewARMS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemTypeNewARMS)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemTypeNewARMS, *PCDlgAdminItemTypeNewARMS;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
