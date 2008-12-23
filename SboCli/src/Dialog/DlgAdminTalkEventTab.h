/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventTab.h										 */
/* ���e			:��b�C�x���g�^�u�_�C�A���O�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "afxcmn.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventTab : public CDlgAdminBase
{
public:
			CDlgAdminTalkEventTab(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminTalkEventTab();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:


protected:



public:
//	void OnOK(){}
//	void OnCancel(){}
	//{{AFX_DATA(CDlgAdminTalkEventTab)
	enum { IDD = IDD_TALKEVENT_TAB };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventTab)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	afx_msg void OnBnClickedAdd();
} CDlgAdminTalkEventTab, *PCDlgAdminTalkEventTab;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
