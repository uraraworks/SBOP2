/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharMotionList.h									 */
/* ���e			:�L�������[�V�������X�g�_�C�A���O�N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/28													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharMotionList : public CDlgAdminBase
{
public:
			CDlgAdminCharMotionList(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharMotionList();							/* �f�X�g���N�^ */

	void	Init	(CMgrData *pMgrData, DWORD dwMotionTypeID);			/* ������ */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */


protected:
	DWORD	m_dwMotionTypeID;		/* �ҏW���̃��[�V�������ID */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminCharMotionList)
	enum { IDD = IDD_CHAR_MOTION_LIST };
	int m_nGrpIDSub;
	CListCtrl	m_List;
	CString	m_strMotionName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharMotionList)
	protected:
	virtual void PostNcDestroy ();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharMotionList)
	virtual BOOL OnInitDialog();
	afx_msg void OnModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharMotionList, *PCDlgAdminCharMotionList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
