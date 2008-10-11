/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminEfcEffectList.h									 */
/* ���e			:�G�t�F�N�g�ꗗ�_�C�A���O�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/06													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoEffect;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminEfcEffectList : public CDlgAdminBase
{
public:
			CDlgAdminEfcEffectList(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminEfcEffectList();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	Renew		(void);											/* �ꗗ���X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */


protected:


protected:
	CLibInfoEffect	*m_pLibInfo;		/* �G�t�F�N�g��� */



public:
	//{{AFX_DATA(CDlgAdminEfcEffectList)
	enum { IDD = IDD_EFC_EFFECT_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminEfcEffectList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminEfcEffectList)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	afx_msg void OnRenew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminEfcEffectList, *PCDlgAdminEfcEffectList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
