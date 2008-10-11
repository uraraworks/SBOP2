/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharMotionTypeList.h								 */
/* ���e			:�L�������[�V������ʃ��X�g�_�C�A���O�N���X ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/26													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoMotionType;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharMotionTypeList : public CDlgAdminBase
{
public:
			CDlgAdminCharMotionTypeList(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharMotionTypeList();						/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */

	void	Renew		(void);											/* ���X�g�X�V */


protected:
	CLibInfoMotionType		*m_pLibInfoMotionType;		/* ���[�V������ʏ�� */



public:
	//{{AFX_DATA(CDlgAdminCharMotionTypeList)
	enum { IDD = IDD_CHAR_MOTIONTYPE_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharMotionTypeList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CDlgAdminCharMotionTypeList)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharMotionTypeList, *PCDlgAdminCharMotionTypeList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
