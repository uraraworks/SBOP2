/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminEfcBalloonList.h									 */
/* ���e			:���o���ꗗ�_�C�A���O�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/24													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoEfcBalloon;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminEfcBalloonList : public CDlgAdminBase
{
public:
			CDlgAdminEfcBalloonList(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminEfcBalloonList();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	Renew		(void);											/* �ꗗ���X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */


protected:


protected:
	CLibInfoEfcBalloon	*m_pLibInfo;		/* ���o����� */



public:
	//{{AFX_DATA(CDlgAdminEfcBalloonList)
	enum { IDD = IDD_EFC_BALLOON_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminEfcBalloonList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminEfcBalloonList)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	afx_msg void OnRenew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminEfcBalloonList, *PCDlgAdminEfcBalloonList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
