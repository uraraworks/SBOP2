/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemWeaponList.h									 */
/* ���e			:�A�C�e��������ꗗ�_�C�A���O�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/10													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoItemWeapon;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminItemWeaponList : public CDlgAdminBase
{
public:
			CDlgAdminItemWeaponList(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminItemWeaponList();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	Renew		(void);											/* �ꗗ���X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */


protected:


protected:
	CWnd				*m_pWndNotify;				/* �ʒm��E�B���h�E */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;		/* �A�C�e�������� */



public:
	//{{AFX_DATA(CDlgAdminItemWeaponList)
	enum { IDD = IDD_ITEMWEAPONINFO_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemWeaponList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemWeaponList)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnCopy();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemWeaponList, *PCDlgAdminItemWeaponList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
